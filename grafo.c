#include <stdio.h>
#include "grafo.h"
#include <graphviz/cgraph.h>


//------------------------------------------------------------------------------
// o valor que representa "infinito"

const int infinito = 0;

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
// 
// o grafo pode ser direcionado ou não
// 
// o grafo tem um nome, que é uma "string"

struct grafo {
  bool directed;
  char *name;
  int num_nodes;
  int num_edges;
};

struct vertice {
  char *name;
};

//------------------------------------------------------------------------------
// devolve o nome do grafo g

char *nome_grafo(grafo g) {
  
  return g->name;
}
//------------------------------------------------------------------------------
// devolve um grafo de nome s,
//         ou 
//         NULL, caso não exista tal grafo

grafo grafo_nome(char *s) {
  
  return NULL;
}
//------------------------------------------------------------------------------
// devolve 1, se g é direcionado,
//         ou 
//         0, caso contrário

int direcionado(grafo g) {

  return g->directed ? 1 : 0;
}
//------------------------------------------------------------------------------
// devolve o número de vértices do grafo g

unsigned int numero_vertices(grafo g) {
  
  return g->num_nodes;
}
//------------------------------------------------------------------------------
// devolve o número de arestas/arcos do grafo g

unsigned int numero_arestas(grafo g) {
  
  return g->num_edges;
}
//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
// 
// devolve 1 em caso de sucesso,
//         ou 
//         0, caso contrário

int destroi_grafo(grafo g) {
  
  free(g);
  g = NULL;
  return !g ? 1 : 0;
}

//------------------------------------------------------------------------------
// lê um grafo no formato dot de input
// 
// devolve o grafo lido,
//         ou 
//         NULL, em caso de erro 

grafo le_grafo(FILE *input) {
  
  Agraph_t *g;
  struct grafo *grafo;
  char *nomeGrafo;
  int num_nodes, num_edges = 0;

  grafo = malloc(sizeof(struct grafo));
  
  if (!grafo){
    printf("Can't malloc my graph\n");
    return NULL;
  }
  
  //assing to g ( * to Agraph_t structure ), reading by cgraph library
  //with input in dot description language passed in "input"
  g = agread(input, 0);

  //read from g ( * to Agraph_t structure ), and 
  //populate our structure grafo (* to grafo) the necessary parameters
  grafo->name = agnameof(g);
  grafo->directed = agisdirected(g);
  grafo->num_nodes = agnnodes(g);
  grafo->num_edges = agnedges(g);

  nomeGrafo = nome_grafo(grafo);
  num_nodes = numero_vertices(grafo);
  num_edges = numero_arestas(grafo);

  printf("Name of my graph *grafo: %s\n", nomeGrafo);
  printf("My dot graph written: \n");
  g = agwrite(g, stdout);

  return grafo;
}
//------------------------------------------------------------------------------
// escreve o grafo g em output usando o formato dot.
//
// devolve o grafo escrito,
//         ou 
//         NULL, em caso de erro 

grafo escreve_grafo(FILE *output, grafo g) {
  
  return 0;
}
//------------------------------------------------------------------------------
// (apontador para) estrutura de dados que representa um vértice do grafo
// 
// cada vértice tem um nome que é uma "string"

// struct vertice {};

//------------------------------------------------------------------------------
// devolve o nome do vertice v

char *nome_vertice(vertice v){

  return NULL;
}
//------------------------------------------------------------------------------
// devolve um vertice de nome s no grafo g,
//         ou 
//         NULL caso não exista em g um vertice de nome s

vertice vertice_nome(char *s, grafo g) {
  
  return NULL;
}
//------------------------------------------------------------------------------
// devolve o grau do vértice v no grafo g
// 
// se g é direcionado, e 
//                       direcao =  1, o grau devolvido é o de saída
//                       direcao = -1, o grai devolvido é o de entrada
//                
// caso contrário o valor de direcao é ignorado.                  

unsigned int grau(vertice v, int direcao, grafo g) {

  return 0;
}
//------------------------------------------------------------------------------
// devolve o "primeiro" vizinho de v em g,
//         ou
//         NULL se v é vértice isolado em g
//
// se g é direcionado, e 
//                       direcao =  1, o vizinho devolvido é de saída
//                       direcao = -1, o vizinho devolvido é de entrada
//                
// caso contrário o valor de direcao é ignorado.                  

vertice primeiro_vizinho(vertice v, int direcao, grafo g) {

  return NULL;
}
//------------------------------------------------------------------------------
// devolve o "próximo" vizinho de v em g após u,
//         ou
//         NULL se u é o "último" vizinho de v em g
//
// se g é direcionado, e 
//                       direcao =  1, o vizinho devolvido é de saída
//                       direcao = -1, o vizinho devolvido é de entrada
//                
// caso contrário o valor de direcao é ignorado.                  

vertice proximo_vizinho(vertice u, vertice v, int direcao, grafo g) {

return NULL;
}
//------------------------------------------------------------------------------
// devolve 1, se v é um vértice simplicial em g, 
//         ou
//         0, caso contrário
//
// um vértice é simplicial no grafo se sua vizinhança é uma clique

int simplicial(vertice v, grafo g) {

return 0;
}
//------------------------------------------------------------------------------
// devolve 1, se g é um grafo bipartido, 
//         ou
//         0, caso contrário

int bipartido( grafo g) {

  return 0;
}
//------------------------------------------------------------------------------
// devolve em c um caminho mínimo de u a v no grafo não direcionado g, 
//              de forma que
//
//                  c[0]=u, ..., c[n]=v, onde n é o tamanho do caminho
//
//              ou
// 
//              NULL se não existe tal caminho, 
//
// em qualquer caso, devolve a distância de u a v em g

int caminho_minimo(vertice *c, vertice u, vertice v, grafo g) {

  return 0;
}
//------------------------------------------------------------------------------
// devolve o diâmetro do grafo g

int diametro(grafo g) {

  return 0;
}
//------------------------------------------------------------------------------
