//GRECU STEFANIA 313CB
#include "grafuri_header.h"

int main(int arg, char *argv[])
{
  TGL* g;
  FILE *fout;
  fout = fopen ("tema3.out", "wt");
  if (!fout)
    return 0;

  /*pentru cerinta 1*/
  if(strcmp(argv[1], "1") == 0)
  {
     g = CitGrafNeorientat(); //constrim graful
  //   if(!g) { 
  //     printf("-=- eroare la citire -=-\n"); return -1;
  //   }

  //   //AfiGrafL(g);

  //   //initializare vector de vizitat
  //   int *visited = calloc((g->n + 1), sizeof(int));

  //   //aflarea numarului de componente
  //   int x = nr_comp_conexe(g);
  //   fprintf(fout, "%d\n", x);

  //   //funxtia ce afiseaza in ordine crescatoare sumele minime
  //   drum_min(g, x, fout);

  //   free(visited);
  }
  else
  {
    //cerinta 2
    FILE *f = fopen ("tema3.in", "rt");
    if (!f)
      return 0;

    g = CitGrafOrientat(f); //construirea arborelui
    if(!g) { 
      printf("-=- eroare la citire -=-\n"); return -1;
    }

    int *nod = CitNoduri(g, f); //luam din fisier valorile nodurilor

    //AfiGrafL(g);
    //functia ce rezolva taskul 2
    task2(g, fout, nod);
    free(nod);
    fclose(f);
  }
  DistrG(&g);
  fclose(fout);
  return 0;
}