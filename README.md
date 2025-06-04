# GrafosDijkstra-
Este projeto tem como objetivo criar um sistema em linguagem C que modele um mapa de cidades conectadas por rotas, utilizando grafos e aplicando o algoritmo de Dijkstra para calcular o menor caminho.

 --> Funcionalidades <--
Cadastro de Cidades: Adicione novas cidades ao mapa.
Cadastro de Rotas: Conecte duas cidades existentes com uma rota, especificando a distância (peso) entre elas. O grafo é considerado não-direcionado, ou seja, se você adicionar uma rota de A para B, a rota de B para A com a mesma distância também é criada.
Visualização: Visualize todas as cidades cadastradas e as rotas que partem de cada uma delas.
Cálculo do Menor Caminho: Utilize o algoritmo de Dijkstra para encontrar o caminho de menor custo (menor distância) entre duas cidades, mostrando a distância total e o percurso.
Menu Interativo: Uma interface de linha de comando permite a interação com o sistema.
Estrutura de Dados

O sistema utiliza as seguintes estruturas:

Cidade: Representa um nó no grafo, contendo o nome da cidade e um ponteiro para sua lista de adjacências.
Rota: Representa uma aresta no grafo, conectando uma cidade a outra (destino) e armazenando o peso (distância) dessa conexão.
Grafo: Contém um array de Cidades e o número total de cidades cadastradas.
