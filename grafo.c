#include <stdio.h>
#include "grafo.h"
#include <graphviz/cgraph.h>
#include <string.h>
#include <stdlib.h>

typedef struct list *list;

int loadVertices(Agraph_t *agraph, grafo graph);
struct vertice* createVertice(char *name);
char *nome_vertice(vertice v);
int vertexNumber = -1; 
int edgeNumber = 0;

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
  char *name;
  list adjList;
  unsigned int degree;
};

struct list {
  unsigned int length;
  list next;
  char *vertexName; // pointer to head node of list
};

struct grafo {
  int num_edges;
  int num_vertices;
  char *name;
  bool directed;
  vertice vertices;
};

char *vertexName(vertice v) { return v->name; }
vertice nextVertex(list adjList) { return adjList->next; }
unsigned int listLength( list adjList) { return adjList->length; }

list createList(vertice v) {
  list adjList = (struct list*) malloc(sizeof(struct list));
  
  if (!adjList) return NULL;
  
  adjList->length = 0;
  adjList->next = NULL;
  adjList->vertexName = NULL;

  return adjList;
}


void insertsNode(char *name, list adjList, int qttdEdges) {
  list current = adjList;
  
  if (qttdEdges == 0) {
    adjList->length++;
    adjList->vertexName = name;
    adjList->next = NULL;

    return;
  }

  while (adjList->next != NULL) //Travels the list to the end
    adjList = adjList->next;

  /*Now we can add a new variable*/
  adjList->next = malloc(sizeof(list));
  adjList->next->length++;
  adjList->next->vertexName = name;
  adjList->next->next = NULL;

  return;
}


int loadVerticesAndEdges(Agraph_t *agraph, struct grafo *graph){
  Agnode_t *aNode;
  Agedge_t *e;
  char *nameSrc;
  char *nameDst;
  char *nodeName;
  int ret, i = 0;
  int qttdEdges;
  struct vertice* v = malloc(sizeof(struct vertice));
  
  //create node and his edges
  for (aNode = agfstnode(agraph); aNode; aNode = agnxtnode(agraph, aNode)){
    nodeName = agnameof(aNode);
    v = createVertice(nodeName);
    graph->vertices[i].name = v->name;
    graph->vertices[i].degree = agdegree(agraph, aNode, TRUE, TRUE);
    graph->vertices[i].adjList = createList(v); 
    qttdEdges = 0;   

    for (e = agfstout(agraph, aNode); e; e = agnxtout(agraph, e)) {
      nameDst = agnameof(aghead(e));    
      insertsNode(nameDst, graph->vertices[i].adjList, qttdEdges);
      qttdEdges++;
    }
    ++i;
  }
}


struct vertice* createVertice(char *name) { 
    struct vertice* newVertex = (struct vertice*) malloc(sizeof(struct vertice));
    newVertex->name = name;
    newVertex->adjList = NULL;
    return newVertex;
}

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
  
  return g->num_vertices;
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


grafo copyGraph(grafo graph){
  grafo grafo_copy = malloc(sizeof(struct grafo));
  grafo_copy->name = nome_grafo(graph);
  grafo_copy->directed = direcionado(graph);
  grafo_copy->num_vertices = graph->num_vertices;
  grafo_copy->vertices = malloc(graph->num_vertices * sizeof(struct vertice));

  for(int i=0; i< graph->num_vertices; ++i){
    grafo_copy->vertices[i] = graph->vertices[i];
  }
  
  return grafo_copy;
}

void  printVertices(grafo g) {
  grafo graphToOperate;
  graphToOperate = copyGraph(graph);
  
  for (int i = 0; i < graphToOperate->num_vertices; ++i) {
    graphToOperate = copyGraph(graph);
    
    printf("\n%s-->", graphToOperate->vertices[i].name);
    
    struct vertice vertexDegree = (struct vertice) graphToOperate->vertices[i];
    
    unsigned int degree = grau(&vertexDegree, 0, graphToOperate);
      
    printf("\nmy Degree|%d| -> \n", degree);
    
    while (graphToOperate->vertices[i].adjList != NULL) {
      if (graphToOperate->vertices[i].adjList->vertexName != NULL)
        printf("|%s|-|->", graphToOperate->vertices[i].adjList->vertexName);
        graphToOperate->vertices[i].adjList = graphToOperate->vertices[i].adjList->next;
    }
    printf("\n");
  }
}

grafo createGraph(int num_vertices) {
  graph = malloc(sizeof(struct grafo));
  graph->vertices = malloc(num_vertices * sizeof(struct vertice));

  for (int i = 0; i < num_vertices; ++i) {
    // graph->vertices[i] = NULL;
  }
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
  agraph = agread(input, 0);
  grafo graphToOperate;
  
  if (!agraph) return NULL;

  int num_vertices = agnnodes(agraph);
  graph = malloc(sizeof(struct grafo));

  
  // unsigned int degree = grau(&vertexDegree, -1, g);
  
  
  graph->vertices = malloc(num_vertices * sizeof(struct vertice));
  
  if (!graph){
    printf("Can't malloc my graph\n");
    return NULL;
  }
  
  //read from g ( * to Agraph_t structure ), and 
  //populate our structure grafo (* to grafo) the necessary parameters
  graph->name = agnameof(agraph);
  graph->directed = agisdirected(agraph);
  graph->num_vertices = agnnodes(agraph);
  graph->num_edges = agnedges(agraph);
  
  nomeGrafo = nome_grafo(graph);
  
  ret = loadVerticesAndEdges(agraph, graph);

  graphToOperate = copyGraph(graph);

  printVertices(graphToOperate);


  // printGraph(graph);
  if (ret < 0) return NULL;

  printf("Name of my graph *grafo: %s\n", nomeGrafo);
  printf("My dot graph written: \n");
  agraph = agwrite(agraph, stdout);

  return graph;
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
//CUIDADO - retirando vertice ??
vertice vertice_nome(char *s, grafo g) {
  
  grafo graphToOperate;
  graphToOperate = copyGraph(graph);

  for (int i = 0; i < g->num_vertices; i++){
    
    if (!strcmp(s, graphToOperate->vertices[i].name)) 
      return (vertice)&graphToOperate->vertices[i];
    
  }
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

  if (direcao == 1) {

    while (v->adjList != NULL) {

      if (v->adjList->vertexName != NULL)
        ++degree;
      v->adjList = v->adjList->next;
    }
  } else if (direcao == -1) {

    for (int i = 0; i < g->num_vertices; ++i) {
        
      while (g->vertices[i].adjList != NULL) {
        if (g->vertices[i].adjList->vertexName) {
          if (!(strcmp(g->vertices[i].adjList->vertexName, v->name))) /*se te elto na lista de adj
                                                                        igual ao vertice procurado*/
            ++degree;
        }
        g->vertices[i].adjList = g->vertices[i].adjList->next;
      }
    }
  } else
    return v->degree; /*get from structure that was populate in loadVerticesAndEdges()*/
    
  return degree;
}


// //------------------------------------------------------------------------------
// // devolve o "primeiro" vizinho de v em g,
// //         ou
// //         NULL se v é vértice isolado em g
// //
// // se g é direcionado, e 
// //                       direcao =  1, o vizinho devolvido é de saída   ->to
// //                       direcao = -1, o vizinho devolvido é de entrada ->from
// //                
// // caso contrário o valor de direcao é ignorado e será retornado o vizinho de entrada.                  
// // se o vizinho foi encontrado no to, devolve from e vice-versa
// vertice primeiro_vizinho(vertice v, int direcao, grafo g) {

//   for (int i = 0; i < edgeNumber; ++i) {
//     if (g->edges[i].to.name != NULL && g->edges[i].from.name != NULL)
//       printf("\ng->edges[i].to.name %s  |   g->edges[i].from.name %s  \n",\
//             g->edges[i].to.name, g->edges[i].from.name);

//     if (direcao == -1) {
//       if (g->edges[i].from.name == nome_vertice(v)) {
//         return (vertice)&g->edges[i].to;
//       }
//     }
//     if (direcao = 1) {
//       if (g->edges[i].to.name == nome_vertice(v)) {
//         return (vertice)&g->edges[i].from;
//       }
//     } 
//     else {
//       if (((g->edges[i].to.name == nome_vertice(v)) || g->edges[i].from.name == nome_vertice(v))){
//         if (g->edges[i].to.name != NULL) return  g->edges[i].from.name;
//         return g->edges[i].to.name;
//       }
//     }
//   }
//   return NULL;
// }
// //------------------------------------------------------------------------------
// // devolve o "próximo" vizinho de v em g após u,
// //         ou
// //         NULL se u é o "último" vizinho de v em g
// //
// // se g é direcionado, e 
// //                       direcao =  1, o vizinho devolvido é de saída
// //                       direcao = -1, o vizinho devolvido é de entrada
// //                
// // caso contrário o valor de direcao é ignorado.                  

// vertice proximo_vizinho(vertice u, vertice v, int direcao, grafo g) {

// return NULL;
// }
// //------------------------------------------------------------------------------
// // devolve 1, se v é um vértice simplicial em g, 
// //         ou
// //         0, caso contrário
// //
// // um vértice é simplicial no grafo se sua vizinhança é uma clique

// int simplicial(vertice v, grafo g) {

// return 0;
// }

// //------------------------------------------------------------------------------
// // devolve 1, se g é um grafo bipartido, 
// //         ou
// //         0, caso contrário

// int bipartido( grafo g) {

//   return 0;
// }
// //------------------------------------------------------------------------------
// // devolve em c um caminho mínimo de u a v no grafo não direcionado g, 
// //              de forma que
// //
// //                  c[0]=u, ..., c[n]=v, onde n é o tamanho do caminho
// //
// //              ou
// // 
// //              NULL se não existe tal caminho, 
// //
// // em qualquer caso, devolve a distância de u a v em g

// int caminho_minimo(vertice *c, vertice u, vertice v, grafo g) {

//   return 0;
// }
// //------------------------------------------------------------------------------
// // devolve o diâmetro do grafo g

// int diametro(grafo g) {

//   return 0;
// }
// //------------------------------------------------------------------------------