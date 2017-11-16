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
typedef enum { false, true } bool;

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
// struct vertice firstNeighbor(vertice v) { return v->adjList->vertexName; }
vertice nextVertex(list adjList) { return adjList->next; }
unsigned int listLength( list adjList) { return adjList->length; }

list createList() {
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
    graph->vertices[i].adjList = createList(); 
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

void printVerticesByInputNeighborhood(grafo g) {
  grafo graphToOperate;
  graphToOperate = copyGraph(graph);
  
  for (int i = 0; i < graphToOperate->num_vertices; ++i) {
    graphToOperate = copyGraph(graph);
  
  }
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
    
    struct vertice vertexNeighbor = (struct vertice) graphToOperate->vertices[i];
    struct vertice vertexNeighborNext = (struct vertice) graphToOperate->vertices[i];
    vertice u = vertice_nome("c", g);
    vertice v = vertice_nome(vertexNeighborNext.name, g);
    
    vertice skyIsANeighborhood = primeiro_vizinho(&vertexNeighbor, -1, graphToOperate);
    if (skyIsANeighborhood != NULL)
      printf("my skyIsANeighborhood |%s| -> \n\n", skyIsANeighborhood->name);
    
    vertice skyIsNextNeighborhood = proximo_vizinho(u, v, -1, graphToOperate);
    if (skyIsNextNeighborhood != NULL)
      printf("my skyIsANeighborhood, but NEXT |%s| -> \n\n", skyIsNextNeighborhood->name);
    
    graphToOperate = copyGraph(graph);
    
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
  
  // insertsNode(char *name, list adjList, int qttdEdges) 

  list set = createList();
  insertsNode("a", set, 0);
  insertsNode("b", set, 1);
  insertsNode("c", set, 2);
  ret = clique(set, graph);

  printf("\n\nDeu clique?? ->> %d\n\n", ret);

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
    if (s != NULL){
      if (!strcmp(s, graphToOperate->vertices[i].name)) 
        return (vertice)&graphToOperate->vertices[i];
    }
    
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
  
  grafo graphToOperate = copyGraph(g);
  
  if ((direcao == 1) || (direcao == 0)) {
    return vertice_nome(v->adjList->vertexName, graphToOperate);
  }
  
  else if (direcao == -1) {
   
    for (int i = 0; i < g->num_vertices; ++i) {
      
      while (graphToOperate->vertices[i].adjList != NULL) {
        if (graphToOperate->vertices[i].adjList->vertexName) {
          if (!(strcmp(graphToOperate->vertices[i].adjList->vertexName, v->name))) /*se te elto na lista de adj*/
            return vertice_nome(graphToOperate->vertices[i].name, graphToOperate);
        }
        graphToOperate->vertices[i].adjList = graphToOperate->vertices[i].adjList->next;
      }
    }
  }
 
  return NULL;
}

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

vertice proximo_vizinho(vertice u, vertice v, int direcao, grafo g) {

  grafo graphToOperate = copyGraph(g);
  
  if ((direcao == 1) || (direcao == 0)) {
    while (v->adjList != NULL) { /* percorre lista de adjacencia de v*/
      
      if (v->adjList->vertexName != NULL){ 
        if (!strcmp(v->adjList->vertexName, u->name)){ /* se encontrar o vertice u na lista de v */
          if (v->adjList->next != NULL)
            return vertice_nome(v->adjList->next->vertexName, graphToOperate); /* retorna o proximo vizinho de v */
          return NULL;
        } 
      }
      v->adjList = v->adjList->next;  
    }
  } else if (direcao == -1) {
    if (vertice_nome(nome_vertice(u), graphToOperate)) { /* se existe o vertice de nome u no grafo*/
      for (int i = 0; i < g->num_vertices; ++i) {
        
        if (!strcmp(graphToOperate->vertices[i].name, u->name)) { /* se o vertice corrente é o u: procura v na lista de ajd*/
          while (graphToOperate->vertices[i].adjList != NULL) { /* enquanto houver vértices*/
        
            if (graphToOperate->vertices[i].adjList->vertexName) {
              if (!(strcmp(graphToOperate->vertices[i].adjList->vertexName, v->name))){/*se te v->name na lista de adj*/
                // procura prox vértice onde v->name está na lista de adj*/
                for (int j = i + 1; j < graphToOperate->num_vertices; ++j) { /* a partir de u, procura prox guy q tem mano brow */
                  while (graphToOperate->vertices[j].adjList != NULL) { /* enquanto houver vértices*/
                    
                    if (graphToOperate->vertices[j].adjList->vertexName) {
                      if (!(strcmp(graphToOperate->vertices[j].adjList->vertexName, v->name)) /*se te v->name na lista de adj*/
                       && (strcmp(graphToOperate->vertices[j].name, graphToOperate->vertices[i].name))) /*e é diferente do proprio v*/
                        return vertice_nome(graphToOperate->vertices[j].name, graphToOperate);
                    }
                    graphToOperate->vertices[j].adjList = graphToOperate->vertices[j].adjList->next;
                  }          
                }
              } 
            }
            graphToOperate->vertices[i].adjList = graphToOperate->vertices[i].adjList->next;
          }
        }
      }
    } 

  }
  
  return NULL;
}



list createNeighborhood(vertice v, int direcao, grafo g) {
  
  list neighborhood = createList();
  int listLength = 0;

  for (vertice neighbor = primeiro_vizinho(v, direcao, g); neighbor != NULL; neighbor = proximo_vizinho(neighbor, v, direcao, g)) {
    insertsNode(neighbor->name, neighborhood, listLength);
    ++listLength;
  }
  return neighborhood;
}

int hasAllVerticesInNeighborhood(list l, list outputNeighborhood, list inputNeighborhood) {
  
  int foundInList = 0;
  list outputL = l;/* equals to set, just separe to not replace data*/
  list inputL = l;

  printf("\n\nMy OUTPUTNeighborhood list is: ");
  while (outputNeighborhood != NULL) {
    while (outputL != NULL) {
      if ((outputNeighborhood->vertexName != NULL) && (outputL->vertexName != NULL)) {
          if (!strcmp(outputNeighborhood->vertexName, outputL->vertexName)){
            printf("\n: %s ", outputNeighborhood->vertexName);
            ++foundInList;
            printf("\nfoundList: %d\n", foundInList);
            
            break;
          }
        }
        outputL = outputL->next;
        if (foundInList) continue;
      }
      outputNeighborhood = outputNeighborhood->next;
      outputL = l;
    }

  // foundInList = 0;
  printf("\n\nMy INPUTNeighborhood list is: ");
  while (inputNeighborhood != NULL) {  /* enquanto houver vizinhanca de entrada ou conjunto de entrada*/
    while (inputL != NULL) {
      if (inputNeighborhood->vertexName && inputL->vertexName) {
        if (!strcmp(inputNeighborhood->vertexName, inputL->vertexName)) {
          printf("\n: %s ", inputNeighborhood->vertexName);
          ++foundInList;
          break;
        }
        inputL = inputL->next;
      }
      if (foundInList) break;
    }
    inputNeighborhood = inputNeighborhood->next;
  }
  return foundInList;
}
//------------------------------------------------------------------------------
// devolve 1, se o conjunto dos vertices em l é uma clique em g, ou
//         0, caso contrário
//
// um conjunto C de vértices de um grafo é uma clique em g 
// se todo vértice em C é vizinho de todos os outros vértices de C em g

    // faz um loop em cima de cada elemento da lista l      
    // para cada elemento pega sua vizinhaca
    // se a vizinhaca conter todos os elementos da lista l continua
    // se não conter retorna 0
    
    /*{ list = ["a", "b", "c"] }*/
int clique(list l, grafo g) {
  
  list inputNeighborhood = createList();
  list outputNeighborhood = createList();
  int isClique = 0;
  int lLength = 0;
  
  if ((!inputNeighborhood) || (!outputNeighborhood)) return -1;

  printf("My set list is: ");
  while (l != NULL) {
    printf("\n\nVertex%s ", l->vertexName);
    vertice vertexOfl = vertice_nome(l->vertexName, g);
    
    outputNeighborhood = createNeighborhood(vertexOfl, 1, g);
    inputNeighborhood = createNeighborhood(vertexOfl, -1, g);
    
    isClique += hasAllVerticesInNeighborhood(l, outputNeighborhood, inputNeighborhood);
    
    //se em output ou input tem todos os elementos da list set
    l = l->next;
    ++lLength;
  }

  printf("\nisClique++ = %d\n",isClique);
  printf("\nlength++ = %d\n",lLength);
  
  if (isClique >= lLength)
    return 1;
  
  printf("\n\n");
    
  return 0;
}

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