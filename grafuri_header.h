//GRECU STEFNIA 313CB
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>


typedef struct celarc 
{ char *dest;           /* destinatie arc */
  int c;            /* cost arc */
  struct celarc *urm; /* adresa urmatorul adiacent */
} TCelArc, *AArc;

typedef struct 
{ int n;     /* numar noduri */
  char **nod_s; //vector in care sa retin numele nodului din care se pleaca
  AArc* x;    /* vector adrese liste arce */
} TGL;

TGL* AlocG(int nr);
void DistrG(TGL** ag);
TGL* CitGrafNeorientat();
void AfiGrafL(TGL * g);
void dfs(TGL *g, int *visited, int ic, char start[]);
int nr_comp_conexe(TGL *g);
int mindistance(int *visited, int *distances, TGL *g);
void Dijkstra(TGL *g, char nodul[], int *visited, int *distances, char **parents, float *scor, int *nod);
int prim(TGL *g, char nod[], int *visited);
void drum_min(TGL *g, int nr_conex, FILE *fout);
int cmp (const void *a, const void *b);
TGL* CitGrafOrientat(FILE *f);
int* CitNoduri(TGL *g, FILE *f);
void task2 (TGL *g, FILE *fout, int *nod);
void afisare_noduri(char **parents, char node[], TGL *g, FILE *fout);