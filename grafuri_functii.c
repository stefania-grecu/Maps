//GRECU STEFANIA 313CB
#include <string.h>
#include "grafuri_header.h"


TGL* AlocG(int nr) /* aloca spatiu pentru descriptor graf si
	                      vector de (n+1) adrese liste arce */
{
  int i, j;
  TGL* g = (TGL*) malloc (sizeof(TGL));    /* spatiu descriptor */
  if (!g)
    return NULL;

  g->x = (AArc*) calloc ((nr+1), sizeof(AArc)); /* spatiu vector adr.liste arce */
  if(!g->x)
  {
    free(g);
    return NULL;
  }

  g->nod_s = (char**) malloc ((nr + 1) * sizeof(char*)); 
  /* spatiu vector de stringuri */
  if(!g->nod_s)
  {
      free(g->x);
      free(g);
      return NULL;
  }

  for(i = 0; i <= nr; i++) {
      /*alocare spatiu pentru fiecare string*/
      g->nod_s[i] = (char *) calloc (20, sizeof(char));
      if (!g->nod_s[i])
      {
        for(j = 0; j < i; j++)
          free(g->nod_s[j]);
        free(g->nod_s);
        free(g->x);
        free(g);
        return NULL;
      }
      /*punem 0 pentru a fi mai usor de identificat cand nu avem string in el*/
      strcpy(g->nod_s[i], "0");
  }
  g->n = nr;
  return g;
}


void DistrG(TGL** ag)
{ 
  int i;
  AArc p, aux;
  for(i = 0; i <= (*ag)->n; i++)
  { 
    p = (*ag)->x[i];
    while(p)
    { aux = p;
      p = p->urm;
      free(aux->dest); /*eliberare destinatie*/
      free(aux);        /*eliberare celula*/
    }
    free((*ag)->nod_s[i]);  /*eliberare string*/
  }
  free((*ag)->nod_s);     /*eliberare vector de stringuri*/
  free((*ag)->x);
  free(*ag);
  *ag = NULL;
}


TGL* CitGrafNeorientat()
{
  FILE *f;
  TGL* g = NULL;
  char *s, *d;
  int cost;
  AArc *p, aux, *q, ant;
  int n, m, ok, i;

  f = fopen ("tema3.in", "rt");
  if (!f)
    return NULL;
  
  //citire m si n
  fscanf(f, "%d %d", &n, &m);

  //alocare spatiu graf
  g = AlocG(n);
  if (!g)
    return 0;

  //alocare spatiu pentru 2 stringuri
  s = (char *) malloc (sizeof(char) * 20);
  d = (char *) malloc (sizeof(char) * 20);

  int k = 0;
  while (k < m)
  {
    //citirea fiecarei linii
    fscanf(f, "%s %s %d\n", s, d, &cost);
    k++;

    ok = 0;

    /*daca ok e 1 atunci mai exista sursa in vectorul de stringur,
    iar daca este 2 atunci am dat de o sursa (nod din care se pleaca) noua*/
    for (i = 1; i <= n; i++)
    {
      //daca mai gasim sursa
      if (strcmp(g->nod_s[i], s) == 0)
      {
        ok = 1;
        break;
      }
      //pentru o sursa noua
      if (strcmp(g->nod_s[i], "0") == 0)
      {
        ok = 2;
        break;
      }
    }

    p = g->x + i;
    ant = NULL;

    /*in ant o sa fie ultima celula din lista de adiacenta
    a nodului sursa*/
    while ((*p) != NULL){
      ant = *p;
      p = &(*p)->urm;
    }

    //alocare celula
    aux = (TCelArc*) calloc (sizeof(TCelArc), 1);
    if (!aux)
    {
      DistrG(&g);
      return NULL;
    }

    if (ok == 2)
      g->nod_s[i] = strdup(s);  //pentru o sursa noua

    /*punem in noua celula destinatia si o legam
    la ant daca acesta exista sau la p daca este prima celula*/
    aux->urm = NULL;
    aux->dest = strdup(d);
    aux->c = cost;
    if (ant != NULL)
      ant->urm = aux;
    else
      *p = aux;

    //si drumul invers
    /*exact la fel ca mai sus doar ca acum destinatia este 
    nodul de plecare si sursa noudul in care ajungem*/
    ok = 0;

    for (i = 1; i <= n; i++)
    {
      if (strcmp(g->nod_s[i], d) == 0)
      {
        ok = 1;
        break;
      }
      if (strcmp(g->nod_s[i], "0") == 0)
      {
        ok = 2;
        break;
      }
    }

    q = g->x + i;
    ant = NULL;

    while ((*q) != NULL){
      ant = *q;
      q = &(*q)->urm;
    }

    aux = (TCelArc*) calloc (sizeof(TCelArc), 1);
    if (!aux)
    {
      DistrG(&g);
      return NULL;
    }

    if (ok == 2)
      g->nod_s[i] = strdup(d);

    aux->urm = NULL;
    aux->dest = strdup(s);
    aux->c = cost;
    if (ant != NULL)
      ant->urm = aux;
    else
      *q = aux;

    
  }
  //eliberare s, d si inchiderea fisierului
  free(s);
  free(d);
  fclose(f);
  return g;
}

/*la fel ca citirea unui graf neorientat, doar ca nu mai aveam si drumul invers*/
TGL* CitGrafOrientat(FILE *f)
{
  TGL* g = NULL;
  char *s, *d;
  int cost;
  AArc *p, aux, *q, ant;
  int n, m, ok, i;

  fscanf(f, "%d %d", &n, &m);

  g = AlocG(n);
  if (!g)
    return 0;

  s = (char *) malloc (sizeof(char) * 20);
  d = (char *) malloc (sizeof(char) * 20);

  int k = 0;
  while (k < m)
  {
    fscanf(f, "%s %s %d\n", s, d, &cost);
    k++;

    ok = 0;

    for (i = 1; i <= n; i++)
    {
      if (strcmp(g->nod_s[i], s) == 0)
      {
        ok = 1;
        break;
      }
      if (strcmp(g->nod_s[i], "0") == 0)
      {
        ok = 2;
        break;
      }
    }

    p = g->x + i;
    ant = NULL;

    while ((*p) != NULL){
      ant = *p;
      p = &(*p)->urm;
    }

    aux = (TCelArc*) calloc (sizeof(TCelArc), 1);
    if (!aux)
    {
      DistrG(&g);
      return NULL;
    }

    if (ok == 2)
      g->nod_s[i] = strdup(s);

    aux->urm = NULL;
    aux->dest = strdup(d);
    aux->c = cost;
    if (ant != NULL)
      ant->urm = aux;
    else
      *p = aux;
  }

  free(s);
  free(d);
  return g;
}

//citirea valorilor nodurilor
int* CitNoduri(TGL *g, FILE *f)
{
  //alocare spatiu pentru un vectori de valori ale nodurilor
  int *nod = (int *) calloc (g->n + 1, sizeof(int));
  if (!nod)
    return NULL;

  char *s = (char *) malloc (sizeof(char) * 20);
  
  int a, k = 0, i;
  
  while (k < g->n)
  {
    /*luam valori din fisier si le punem pe pozitia din vectorul nod
    unde se gaseste numele in vectorul de stringuri*/
    fscanf(f, "%s %d", s, &a);
    for (i = 1; i <= g->n; i++)
    {
      if(strcmp(g->nod_s[i], s) == 0)
        nod[i] = a;
    }
    k++;
  }
  int greutate;
  fscanf(f, "%d", &greutate);
  /*greutatea o punem pe prima pozitia pentru 
  nu o sa folosim pozitia aceasta pentru a o lega unui nod*/
  nod[0] = greutate;

  free(s);
  return nod;
}



void AfiGrafL(TGL * g)
{
  AArc l;
  int i;

  for(i = 1; i <= g->n; i++){
    l = g->x[i];
    if(!l)
      printf("%s: - \n", g->nod_s[i]);
    else
    {
      printf("%s: ", g->nod_s[i]);
      while(l){
        printf(" %s (%d)", l->dest, l->c);
        l = l->urm;
      }
      printf("\n");
    }
  }
}


void dfs(TGL *g, int *visited, int ic, char start[]) {
    int ok, i;
    char d[20];

    //cautam indexul nodului primit ca parametru
    for (i = 1; i <= g->n; i++)
      if(strcmp(g->nod_s[i], start) == 0)
        break;
    
    //il marcam ca vizitat
    visited[i] = ic;

    //parcurgem graful
    AArc l = g->x[i];
    while (l)
    {
      strcpy(d, l->dest);
      //cautam indexul nodului d
      for (i = 1; i <= g->n; i++)
        if(strcmp(g->nod_s[i], d) == 0)
          break;

      //daca acesta nu este vizitat apelam recursiv dfs pentru noul nod
      if (visited[i] == 0)
      {
        dfs(g, visited, ic, d);
      }
      l = l->urm;
    }

}

//numarul componentelor conexe
int nr_comp_conexe(TGL *g)
{
  int ic = 1, i;
  int *visited = calloc((g->n + 1), sizeof(int));

  //apelam dfs pentru primul nod
  dfs(g, visited, ic, g->nod_s[1]);

  //parcurgem graful pentru a verifica daca toate nodurile au fost vizitate
  for(i = 1; i <= g->n; i++)
  {
    if (visited[i] == 0){
      //in cazul in care nu am trecut printr-un nod plusam ic si apelam dfs
      ic++;
      dfs(g, visited, ic, g->nod_s[i]);
    }
  }
  free(visited);
  //ic = numarul de componente conexe
  return ic;
}

//functie care returneaza nodul cu cea mai mica distanta
//distantele sunt de tip int
int mindistance(int *visited, int *distances, TGL *g) {
  int min = INT_MAX;
  int node = 0, i;

  for (i = 1; i <= g->n; i++)
    if(visited[i] == 0 && distances[i] < min) {
      min = distances[i];
      node = i;
    }
  return node;
}

//functie care returneaza nodul cu cea mai mica distanta
//distantele sunt de tip float
int minscor(int *visited, float *scor, TGL *g) {
  float min = INT_MAX;
  int node = 0, i;

  for (i = 1; i <= g->n; i++)
    if(visited[i] == 0 && scor[i] < min) {
      min = scor[i];
      node = i;
    }
  return node;
}


void Dijkstra(TGL *g, char nodul[], int *visited, int *distances, char **parents, float *scor, int *nod)
{
  int node, i, j;
  float scor1;

  //cautam indexul nodului primit ca parametru
  for (i = 1; i <= g->n; i++)
    if(strcmp(g->nod_s[i], nodul) == 0)
      break;

  //indexul il retimen in x
  //actualizam valorile din cei 3 vectori pentru nodul x
  int x = i;
  distances[x] = 0;
  scor[x] = 0;
  parents[x] = strdup(g->nod_s[x]);

  //parcurgem graful
  for (i = 1; i <= g->n; i++) {
    //cautam nodul cu scor minim
    int node = minscor(visited, scor, g);
    //il marcam ca viiztat
    visited[node] = 1;

    //parcurgem lista lui de adiacenta
    AArc l = g->x[node], p;
    for (p = l; p != NULL; p = p->urm)
    {
      //cautam indexul nodului p->dest
      for (j = 1; j <= g->n; j++)
        if(strcmp(g->nod_s[j], p->dest) == 0)
          break;
      
      //calculam scorul pentru acesta
      scor1 = p->c * 1.0 / nod[j];

      /*daca nodul nu este vizitat si 
      are tot scorul de pana acum mai mic decat scorul actual
      actualizam cei 3 vectori*/
      if (visited[j] == 0 && scor[node] + scor1 < scor[j])
      {
        distances[j] = p->c + distances[node];
        scor[j] = scor1 + scor[node];
        parents[j] = strdup (g->nod_s[node]);
      }
    }
  }
}

//functie ce afiseaza nodulule prin care trecem
void afisare_noduri(char **parents, char node[], TGL *g, FILE *fout)
{
  int i;
  //cautarea indexului nodului primit ca parametru
  for(i = 0; i <= g->n; i++)
    if(strcmp(g->nod_s[i], node) == 0)
      break;
  //daca dam de nodul "Insula" ca parinte ii afisam copilul si ne oprim
  if (strcmp(parents[i], "Insula") == 0)
  {
    fprintf(fout, "%s ", node);
    return;
  }

  //apelare recursiva pentru celelalte noduri
  afisare_noduri(parents, parents[i], g, fout);
  fprintf(fout, "%s ", node);
}

//cerinta 2
void task2 (TGL *g, FILE *fout, int *nod)
{
  int corabie = -1, insula = -1, i;

  /*retinem indicii din vectorul de stringuri a nodurilor
  Insula si Corabie*/
  for(i = 1; i <= g->n; i++)
  {
    if(strcmp(g->nod_s[i], "Insula") == 0)
      insula = i;
    if(strcmp(g->nod_s[i], "Corabie") == 0)
      corabie = i;
  }

  /*intitializam ce 4 vectori*/
  int *distances =  malloc(sizeof(int) * (g->n + 1));
  for(i = 1; i <= g->n ; i++)
    distances[i] = INT_MAX;

  float *scor =  malloc(sizeof(int) * (g->n + 1));
  for(i = 1; i <= g->n ; i++)
    scor[i] = INT_MAX;

  int *visited = calloc(g->n + 1, sizeof(int));
  char **parents = (char **) malloc (sizeof(char*) * (g->n + 1));
  for(i = 0; i <= g->n; i++){
    parents[i] = (char *) calloc (20, sizeof(char));
    //parents[i] = strdup("0");
  }

  /*daca nu gasim in vectorul de stringuri nodul "Corabie"
  inseamna ca el nu este nod din care se pleaca*/
  if(corabie == -1)
    fprintf(fout, "Echipajul nu poate ajunge la insula");
  else
  {
    Dijkstra(g, g->nod_s[insula], visited, distances, parents, scor, nod);
    /*daca nodul corabie nu este vizitat inseamna nu exista drum 
    de la insula la corabie*/
    if(visited[corabie] == 0)
      fprintf(fout, "Echipajul nu poate transporta comoara inapoi la corabie");
    else
    {
      int k, min;

      /*initializam min cu valoarea nodului ce are ca parinte pe "Insula*/
      for(i = 1; i <= g->n; i++)
        if(strcmp(parents[i], "Insula") == 0 && i != insula)
          min = nod[i];

      //afisarea in fisier a drumului parcurs
      fprintf(fout, "Insula ");
      afisare_noduri(parents, g->nod_s[corabie], g, fout);

      //carirea greutatii maxime pe care o poate lua comoara
      k = corabie;
      while(strcmp(g->nod_s[k], "Insula") != 0)
      {
        /*cautam indexul nodului copilului de pe pozitia k 
        in vectorul de parinti*/
        for (i = 1; i <= g->n; i++)
          if(strcmp(g->nod_s[i], parents[k]) == 0)
            break;
        //valoarea minima daca nodul este diferit de "Insula"
        if(min > nod[i] && strcmp(g->nod_s[i], "Insula") != 0)
        {
          min = nod[i];
        }
        k = i;
      }

      //afisarea in fisier a costului drumului de la insula la corabie
      //a valorii minime
      //si a numarului de drumuri
      fprintf(fout, "\n%d\n%d\n", distances[corabie], min);
      int nr_drumuri = nod[0] / min;
      fprintf(fout, "%d", nr_drumuri);
    }
  }
  //eliberarea memoriei
  free(distances);
  free(visited);
  for(i = 0; i <= g->n; i++)
    free(parents[i]);
  free(parents);
  free(scor);
}



//drum minim pentru fiecare componenta

int prim(TGL *g, char nod[], int *visited) {
  int i, j;

  //alocare memorie
  int *distances =  calloc(g->n + 1, sizeof(int));
  for(i = 1; i <= g->n ; i++)
    distances[i] = INT_MAX;

  int *parents = calloc(g->n + 1, sizeof(int));
  
  //cautarea indexului nodului primit ca parametru
  for (i = 1; i <= g->n; i++)
    if(strcmp(g->nod_s[i], nod) == 0)
      break;

  int x = i, s = 0;
  distances[x] = 0;
  parents[x] = x;

  //parcurgerea arborelui
  for (i = 1; i <= g->n; i++) {
    //aflarea nodului cu distanta minima
    int node = mindistance(visited, distances, g);
    //marcare ca vizitat
    visited[node] = 1;

    AArc l = g->x[node], p;

    //parcurgerea listei de adiacenta a acestuia
    for (p = l; p != NULL; p = p->urm)
    {
      for (j = 1; j <= g->n; j++)
        if(strcmp(g->nod_s[j], p->dest) == 0)
          break;

      //daca nodul nu este vizitat si are costul mai mic decat valoarea actuala
      if (visited[j] == 0 && p->c < distances[j]) {
        //actualizam vectorii
        distances[j] = p->c;
        parents[j] = node;
      }
    }
  }

  //parcurgerea vectorul de parinti si aflarea sumei drumului minim
  for (i = 1; i <= g->n; i++)
  {
    if(parents[i] != 0)
      s = s + distances[i];
  }

  free(distances);
  free(parents);

  return s;
}

//functie de comparare a doua int-uri
int cmp (const void *a, const void *b)
{
  int x = *(int*)a;
  int y = *(int*)b;
  return x - y;
}

//drumul_minim
void drum_min(TGL *g, int nr_conex, FILE *fout)
{
  int *visited = calloc(g->n + 1, sizeof(int));

  //in drum_min o sa retinem toate sumele pentru fiecare componenta conexa
  int *drum_min, k = 0, s, i;
  drum_min = (int *) calloc (nr_conex, sizeof(int));

  //aflarea sumei pornind din primul nod
  s = prim(g, g->nod_s[1], visited);
  drum_min[k] = s;
  k++;

  //verificam daca am trecut prim toate nodurile
  for (i = 1; i <= g->n; i++)
  {
    if (visited[i] == 0)
    {
      s = prim(g, g->nod_s[i], visited);
      drum_min[k] = s;
      k++;
    }
  }

  //ordonam vectorul de sume
  qsort(drum_min, nr_conex, sizeof(int), cmp);

  //il afisam
  for(i = 0; i < nr_conex; i++)
    fprintf(fout, "%d\n", drum_min[i]);

  free(drum_min);
  free(visited);
}