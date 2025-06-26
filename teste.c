#include <stdlib.h>
#include <stdio.h>
#include "grafo.h"


//------------------------------------------------------------------------------

int main(int argc, char **argv) {

  FILE *inpunt = fopen(argv[1], "r");
  grafo g = le_grafo(inpunt);
  int d = diametro(g);

  if ( !g )

    return 1;

  printf("nome: %s\n", nome_grafo(g));
  printf("%d vértices\n", numero_vertices(g));
  printf("%d arestas\n", numero_arestas(g));
  printf("%sdirecionado\n", direcionado(g) ? "" : "não ");
  printf("%sbipartido\n", bipartido(g) ? "" : "não ");
  
  if (grafo_nome("testGraph") != NULL)
    printf("grafo_nome esta certo? sim!");

  if (d == infinito)
    printf("diâmetro = oo\n");
  else
    printf("diâmetro = %d\n", d);

  escreve_grafo(stdout, g);

  return ! destroi_grafo(g);
}
