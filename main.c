#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_CIDADES 100
#define MAX_NOME_CIDADE 50

typedef struct Rota {
    int destino;
    int peso;
    struct Rota* proximo;
} Rota;

// cidade -> (vértice)
typedef struct Cidade {
    char nome[MAX_NOME_CIDADE];
    Rota* adjacencias; // Lista de rotas que saem desta cidade
} Cidade;

// Representa o grafo completo de cidades e rotas
typedef struct Grafo {
    Cidade cidades[MAX_CIDADES];
    int numCidades;
} Grafo;

void inicializarGrafo(Grafo* grafo) {
    grafo->numCidades = 0;
    for (int i = 0; i < MAX_CIDADES; i++) {
        strcpy(grafo->cidades[i].nome, "");
        grafo->cidades[i].adjacencias = NULL;
    }
}
int adicionarCidade(Grafo* grafo, const char* nome) {
    if (grafo->numCidades >= MAX_CIDADES) {
        printf("Erro: Limite maximo de cidades atingido.\n");
        return -1;
    }
    
    for (int i = 0; i < grafo->numCidades; i++) {
        if (strcmp(grafo->cidades[i].nome, nome) == 0) {
            printf("Erro: Cidade '%s' ja existe.\n", nome);
            return -1;
        }
    }

    strcpy(grafo->cidades[grafo->numCidades].nome, nome);
    grafo->numCidades++;
    printf("Cidade '%s' adicionada com sucesso (ID: %d).\n", nome, grafo->numCidades - 1);
    return grafo->numCidades - 1; // retorna id da nova cidade
}

int encontrarIdCidade(Grafo* grafo, const char* nome) {
    for (int i = 0; i < grafo->numCidades; i++) {
        if (strcmp(grafo->cidades[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

// Adiciona uma rota (aresta) entre duas cidades
void adicionarRota(Grafo* grafo, int origemId, int destinoId, int peso) {
    if (origemId < 0 || origemId >= grafo->numCidades ||
        destinoId < 0 || destinoId >= grafo->numCidades) {
        printf("Erro: IDs de cidade invalidos.\n");
        return;
    }

    Rota* novaRotaOrigem = (Rota*)malloc(sizeof(Rota));
    if (novaRotaOrigem == NULL) {
        printf("Erro de alocação de memoria para rota.\n");
        return;
    }
    novaRotaOrigem->destino = destinoId;
    novaRotaOrigem->peso = peso;
    novaRotaOrigem->proximo = grafo->cidades[origemId].adjacencias;
    grafo->cidades[origemId].adjacencias = novaRotaOrigem;

    // Se o grafo for não-direcionado, adicione a rota de destino para origem também
    Rota* novaRotaDestino = (Rota*)malloc(sizeof(Rota));
    if (novaRotaDestino == NULL) {
        printf("Erro de alocação de memoria para rota.\n");
        return;
    }
    novaRotaDestino->destino = origemId;
    novaRotaDestino->peso = peso;
    novaRotaDestino->proximo = grafo->cidades[destinoId].adjacencias;
    grafo->cidades[destinoId].adjacencias = novaRotaDestino;

    printf("Rota de '%s' para '%s' (distancia: %d) adicionada com sucesso.\n",
           grafo->cidades[origemId].nome, grafo->cidades[destinoId].nome, peso);
}

// Visualiza todas as cidades e suas rotas
void visualizarGrafo(Grafo* grafo) {
    if (grafo->numCidades == 0) {
        printf("Nenhuma cidade cadastrada.\n");
        return;
    }
    printf("\n>> Cidades e Rotas Cadastradas <<\n");
    for (int i = 0; i < grafo->numCidades; i++) {
        printf("Cidade %d (%s):\n", i, grafo->cidades[i].nome);
        Rota* atual = grafo->cidades[i].adjacencias;
        if (atual == NULL) {
            printf("  (Nenhuma rota saindo desta cidade)\n");
        } else {
            while (atual != NULL) {
                printf("  -> Para %s (ID: %d), distancia: %d\n",
                       grafo->cidades[atual->destino].nome, atual->destino, atual->peso);
                atual = atual->proximo;
            }
        }
    }
    printf("-----------------------------------\n");
}

// Função para encontrar o vértice com a menor distancia ainda não visitado
int minDistancia(int dist[], int visitado[], int numCidades) {
    int min = INT_MAX, min_index = -1;

    for (int v = 0; v < numCidades; v++) {
        if (!visitado[v] && dist[v] < min) { // Deve ser '<' para escolher o menor, não '<='
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Função que implementa o algoritmo de Dijkstra
void dijkstra(Grafo* grafo, int origemId, int destinoId) {
    if (origemId < 0 || origemId >= grafo->numCidades ||
        destinoId < 0 || destinoId >= grafo->numCidades) {
        printf("Erro: IDs de cidade invalidos para o calculo do caminho.\n");
        return;
    }
    if (origemId == destinoId) {
        printf("Origem e destino sao a mesma cidade: %s. distancia: 0.\n", grafo->cidades[origemId].nome);
        return;
    }

    int dist[MAX_CIDADES];     // dist[i] armazena a menor distancia de origem até i
    int visitado[MAX_CIDADES]; // visitado[i] é verdadeiro se o vértice ja foi incluído no SPT
    int pai[MAX_CIDADES];      // pai[i] armazena o predecessor de i no menor caminho

    // Inicializa todas as distancias como infinito e os vértices como não visitados
    for (int i = 0; i < grafo->numCidades; i++) {
        dist[i] = INT_MAX;
        visitado[i] = 0;
        pai[i] = -1;
    }

    dist[origemId] = 0; // distancia da origem para ela mesma é 0

    // Encontra o caminho mais curto para todos os vértices
    for (int count = 0; count < grafo->numCidades - 1; count++) {
        int u = minDistancia(dist, visitado, grafo->numCidades);

        // Se nenhum vértice não visitado puder ser alcançado, saia
        if (u == -1 || dist[u] == INT_MAX) {
            break;
        }

        visitado[u] = 1;

        // Itera sobre os adjacentes de u
        Rota* atual = grafo->cidades[u].adjacencias;
        while (atual != NULL) {
            int v = atual->destino;
            int peso = atual->peso;

            // Atualiza a distancia de v se:
            // - v ainda não foi visitado
            // - existe uma aresta entre u e v
            // - o novo caminho é menor que o anterior
            if (!visitado[v] && dist[u] != INT_MAX && (dist[u] + peso) < dist[v]) {
                dist[v] = dist[u] + peso;
                pai[v] = u; // Define u como pai de v
            }
            atual = atual->proximo;
        }
    }

    printf("\n--- Menor Caminho de '%s' para '%s' ---\n",
           grafo->cidades[origemId].nome, grafo->cidades[destinoId].nome);

    if (dist[destinoId] == INT_MAX) {
        printf("Nao ha caminho de '%s' para '%s'.\n",
               grafo->cidades[origemId].nome, grafo->cidades[destinoId].nome);
    } else {
        printf("Distancia total: %d\n", dist[destinoId]);
        printf("Caminho: ");

        // Reconstruct o caminho
        int caminho[MAX_CIDADES];
        int k = 0;
        int atual = destinoId;
        while (atual != -1) {
            caminho[k++] = atual;
            atual = pai[atual];
        }

        // Imprime o caminho na ordem correta
        for (int i = k - 1; i >= 0; i--) {
            printf("%s", grafo->cidades[caminho[i]].nome);
            if (i > 0) {
                printf(" -> ");
            }
        }
        printf("\n");
    }
    printf("----------------------------------------\n");
}

int main() {

    Grafo meuGrafo;
    inicializarGrafo(&meuGrafo);

    int escolha;
    char nomeOrigem[MAX_NOME_CIDADE];
    char nomeDestino[MAX_NOME_CIDADE];
    int idOrigem, idDestino;
    int pesoRota;

    do {
        printf("\n>> Sistema de Rotas de Cidades <<\n");
        printf("        1. ADD Cidade\n");
        printf("        2. ADD Rota\n");
        printf("        3. VIEW Cidades e Rotas\n");
        printf("        4. MENOR CAMINHO (Dijkstra)\n");
        printf("        0. EXIT\n");

        printf(">> Escolha uma opcao: ");
        scanf("%d", &escolha);
        getchar();

        switch (escolha) {
            case 1:
                printf("Digite o nome da nova cidade: ");
                fgets(nomeOrigem, MAX_NOME_CIDADE, stdin);
                nomeOrigem[strcspn(nomeOrigem, "\n")] = 0;
                adicionarCidade(&meuGrafo, nomeOrigem);
                break;
            case 2:
                printf("Digite o nome da cidade de origem: ");
                fgets(nomeOrigem, MAX_NOME_CIDADE, stdin);
                nomeOrigem[strcspn(nomeOrigem, "\n")] = 0;
                idOrigem = encontrarIdCidade(&meuGrafo, nomeOrigem);

                printf("Digite o nome da cidade de destino: ");
                fgets(nomeDestino, MAX_NOME_CIDADE, stdin);
                nomeDestino[strcspn(nomeDestino, "\n")] = 0;
                idDestino = encontrarIdCidade(&meuGrafo, nomeDestino);

                if (idOrigem != -1 && idDestino != -1) {
                    printf("Digite a distancia (peso) da rota: ");
                    scanf("%d", &pesoRota);
                    getchar(); // Consome o '\n'
                    if (pesoRota > 0) {
                         adicionarRota(&meuGrafo, idOrigem, idDestino, pesoRota);
                    } else {
                        printf("Erro: A distancia da rota deve ser positiva.\n");
                    }
                } else {
                    printf("Erro: Uma ou ambas as cidades não foram encontradas.\n");
                }
                break;
            case 3:
                visualizarGrafo(&meuGrafo);
                break;
            case 4:
                printf("Digite o nome da cidade de origem: ");
                fgets(nomeOrigem, MAX_NOME_CIDADE, stdin);
                nomeOrigem[strcspn(nomeOrigem, "\n")] = 0;
                idOrigem = encontrarIdCidade(&meuGrafo, nomeOrigem);

                printf("Digite o nome da cidade de destino: ");
                fgets(nomeDestino, MAX_NOME_CIDADE, stdin);
                nomeDestino[strcspn(nomeDestino, "\n")] = 0;
                idDestino = encontrarIdCidade(&meuGrafo, nomeDestino);

                if (idOrigem != -1 && idDestino != -1) {
                    dijkstra(&meuGrafo, idOrigem, idDestino);
                } else {
                    printf("Erro: Uma ou ambas as cidades nao foram encontradas.\n");
                }
                break;
            case 0:
                printf("Saindo do programa...\n");
                // Liberar memoria alocada para as rotas (opcional, mas boa prática)
                for (int i = 0; i < meuGrafo.numCidades; i++) {
                    Rota* atual = meuGrafo.cidades[i].adjacencias;
                    while (atual != NULL) {
                        Rota* temp = atual;
                        atual = atual->proximo;
                        free(temp);
                    }
                }
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (escolha != 0);

    return 0;
}