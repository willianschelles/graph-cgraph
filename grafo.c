#include <stdio.h>
#include "grafo.h"
#include <graphviz/cgraph.h>
#include <string.h>

int loadVertices(Agraph_t *agraph, grafo graph);
char *nome_vertice(vertice v);
int verticeNumber, edgeNumber = 0;

char *nomeGrafo;


//------------------------------------------------------------------------------
// o valor que representa "infinito"

const int infinito = 0;

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
// 
// o grafo pode ser direcionado ou não
// 
// o grafo tem um nome, que é uma "string"


struct grafo *graph;


struct vertice {
  int degree;
  char *name;
  bool visited;
  struct vertice  *next;
  struct edge *edges;
};

struct edge {
  struct vertice from;
  struct vertice to;
};

struct grafo {
  bool directed;
  char *name;
  int num_vertice;
  int num_edges;
  struct edge    *edges;
  struct vertice *vertices;
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

  // if zero, means that equals (and call second statement return), so return graph
  return strcmp(s, nomeGrafo) ? NULL : graph;
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
  
  return g->num_vertice;
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
  
  Agraph_t *agraph;
  int ret = 0;

  //assing to agraph ( * to Agraph_t structure ), reading by cgraph library
  //with input in dot description language passed in "input"
  agraph = agread(input, 0);

  if (!agraph) return NULL;

  graph = malloc(sizeof(struct grafo));
  
  if (!graph){
    printf("Can't malloc my graph\n");
    return NULL;
  }

  //read from g ( * to Agraph_t structure ), and 
  //populate our structure grafo (* to grafo) the necessary parameters
  graph->name = agnameof(agraph);
  graph->directed = agisdirected(agraph);
  graph->num_vertice = agnnodes(agraph);
  graph->num_edges = agnedges(agraph);

  nomeGrafo = nome_grafo(graph);
  // num_vertice = numero_vertices(graph);
  // num_edges = numero_arestas(graph);

  ret = loadVertices(agraph, graph);

  if (ret < 0) return NULL;

  printf("Name of my graph *grafo: %s\n", nomeGrafo);
  printf("My dot graph written: \n");
  agraph = agwrite(agraph, stdout);

  return graph;
}

int loadVertices(Agraph_t *agraph, grafo graph) {

  Agnode_t *v;
  char *nodename;
  int ret, i = 0;

  graph->vertices = (struct vertice *) NULL;
  graph->vertices = malloc(graph->num_vertice * sizeof(struct vertice));
  
  if (!graph->vertices) return -1;
  
  for (v = agfstnode(agraph); v; v = agnxtnode(agraph, v)){
      // int grau = agdegree(agraph, v, TRUE, TRUE);
      nodename = agnameof(v);
      graph->vertices[i].name = nodename;
      graph->vertices[i].visited = false;
      graph->vertices[i].degree = agdegree(agraph, v, TRUE, TRUE);

      /*
       * Testing if functions nome_vertice(v) and grau(v, direcao, g) is working 

       char * nomeVertice = nome_vertice(v);
      printf("\nName of my vertice: %s\n", nomeVertice);
      
      unsigned int teste = grau(v,-1,graph);
      printf("\nDegree of my vertice: %d\n", teste);
      */      

      ret = loadEdges(agraph, v, graph);
      ++i;
  }

  // printVertices(graph);
  // printEdges(graph);
  printf("\n\n");
  return 0;
}

int loadEdges(Agraph_t *agraph, Agnode_t *v, grafo graph) {

  Agedge_t *aEdge = agfstedge(agraph, v);
  Agedge_t *nextEdges = NULL;
  graph->edges = malloc(graph->num_edges * sizeof(struct edge));
  
  int degree = agdegree(agraph, v, TRUE, TRUE);
  // printf("\n***Vertice: [%s]***\n", agnameof(v));

  for (int i = 0; i < degree; ++i) {
    struct edge edgesOfV;
    edgesOfV.from.name = agnameof(agtail(aEdge));
    edgesOfV.to.name = agnameof(aghead(aEdge));
    graph->edges[edgeNumber].from.name = edgesOfV.from.name;
    graph->edges[edgeNumber].to.name = edgesOfV.to.name;
    // printf("\nvertice from: %s - vertice to: %s\n",  edgesOfV.from,  edgesOfV.to);
    // graph->edges[edgeNumber].from.name,  graph->edges[edgeNumber].to.name);
    // printEdges(graph);
    nextEdges = agnxtedge(agraph, aEdge, v);
    aEdge = nextEdges;
    ++edgeNumber;
  
  }
  return 0;
}

void printVertices(grafo graph) {

  printf("\n\nIname of my nodes from my structure\n");
  for (int i = 0; i < graph->num_vertice; i++){
    printf("%s  " ,graph->vertices[i].name);
    printf("%d  " ,graph->vertices[i].visited);
    printf("%d  \n" ,graph->vertices[i].degree);
  }
  return;
}

void printEdges(grafo graph) {

  printf("\n\nIEdges from my structure\n");
  printf("FROM: ");    
  printf("%s  " , graph->edges[edgeNumber].from.name);
  printf("TO: ");    
  printf("%s  \n", graph->edges[edgeNumber].to.name);
  return;
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

  return v->name;
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
//                       direcao = -1, o grau devolvido é o de entrada
//                
// caso contrário o valor de direcao é ignorado.                  

unsigned int grau(vertice v, int direcao, grafo g) {

  unsigned int degree = 0;
  for (int i = 0; i < edgeNumber; ++i) {
    
    if (direcao == 0 || direcao == 1) {
      g->edges[i].from.name == nome_vertice(v) ? ++degree : 0 ;
    }
    if (direcao == 0 || direcao == -1) {
      g->edges[i].to.name == nome_vertice(v) ? ++degree : 0;
    }
  }
  
  return degree;
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
