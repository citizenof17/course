#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

long long size = 0;

typedef struct Treap
{
  int key, prior;
  struct Treap *l;
  struct Treap *r;
} Treap;

void split(Treap *t, int k, Treap **l, Treap **r)
{
  if (t == NULL){
    *l = *r = NULL;
  }else{
    if (k < t->key){
      split(t->l, k, l, &(t->l));
      *r = t;
    }else{
      split(t->r, k, &(t->r), r);
      *l = t;
    }
  }
}

void insert(Treap **t, Treap* it)
{
  size += sizeof(it);
  if (*t == NULL){
    *t = it;
  }else{
    if (it->prior > (**t).prior){
      split(*t, it->key, &it->l, &it->r);
      *t = it;
    }else{
      insert(it->key < (**t).key ? &((**t).l) : &((**t).r), it);
    }
  }
}

void merge(Treap **t, Treap *l, Treap *r)
{
  // free(t);
  size -= sizeof(*t);
  if (l == NULL || r == NULL)
    *t = l ? l : r;
  else{
    if (l->prior > r->prior){
      merge(&(l->r), l->r, r);
      *t = l;
    }else{
      merge(&(r->l), l, r->l);
      *t = r;
    }
  }
}

void delete(Treap **t, int key)
{
  if (*t == NULL){
    return;
  }
  if ((**t).key == key)
    merge(t, (**t).l, (**t).r);
  else
    delete(key < (**t).key ? &(**t).l : &(**t).r, key);
}

Treap* unite(Treap *l, Treap *r)
{
  if (!l || !r)  return l ? l : r;
  if (l->prior < r->prior){
    // Treap* aux = (Treap*)malloc(sizeof(Treap));
    Treap* aux = l;
    l = r;
    r = aux;
    // free(aux);
  }
  Treap *lt, *rt;
  split(r, l->key, &lt, &rt);
  l->l = unite(l->l, lt);
  l->r = unite(l->r, rt);
  return l;
}

Treap* makeNewTreap(int k)
{
  Treap* t = (Treap*)malloc(sizeof(Treap));
  t->l = t->r = NULL;
  t->key = k;
  t->prior = rand();
  return t;
}

void Print(Treap *q, long n) ////auxiliary function for testing
{
   long i;
   if (q)
   {
      Print(q->r, n+5);
      for (i = 0; i < n; i++) 
         printf(" ");
      printf("%d %d\n", q->key, q->prior);
      Print(q->l, n+5);
   }
}

Treap* treapSearch(Treap *t, int key)
{
  if(t == NULL || key == t->key)
    return t;
  if (key < t->key)
    return treapSearch(t->l, key);
  else
    return treapSearch(t->r, key);
}

Treap* treapMinimum(Treap *t)
{
  while(t->l){
    t = t->l;
  }
  return t;
}

long mtime()
{
  //http://dkhramov.dp.ua/Comp/TimeCount
  struct timeval t;

  gettimeofday(&t, NULL);
  long mt = (long)t.tv_sec * 1000 + t.tv_usec / 1000;
  return mt;
}

void test(char* name_in, char* name_out)
{
  FILE *fin;
  fin = fopen(name_in, "r");
  FILE *fout;
  fout = fopen(name_out, "w");

  printf("OK\n");
  int c;

  Treap *tr = NULL;

  fprintf(fout, "The size of the empty tree: %lli bytes\n", size);
  printf("OK\n");

  int i = 0;

  while((c = fgetc(fin)) != EOF){
    printf("%c\n", c);
    if (c == 'r'){
      int n = 0;
      fscanf(fin, "%d", &n);

      fprintf(fout, "Used memory before adding: %lli bytes\n", size);
      long t = mtime();
      for (int i = 0; i < n; i++){
        int a;
        fscanf(fin, "%d", &a);
        Treap *t = makeNewTreap(a);
        insert(&tr, t);
      }
      t = mtime() - t;

      fprintf(fout, "Used memory after adding: %lli bytes\n", size);
      fprintf(fout, "Adding %d elements in %li milliseconds\n\n", n, t);
    }

    if (c == 'd'){
      int n = 0;
      fscanf(fin, "%d", &n);
      long t = mtime();

      fprintf(fout, "Used memory before deleting: %lli bytes\n", size);

      for (int i = 0; i < n; i++){
        int a;
        fscanf(fin, "%d", &a);
        delete(&tr, a);
      }
      t = mtime() - t;

      fprintf(fout, "Used memory after deleting: %lli bytes\n", size);

      fprintf(fout, "Deleting %d in %li: milliseconds\n\n", n, t);
    }

    if (c == 's'){
      int n = 0;
      fscanf(fin, "%d", &n);
      long t = mtime();

      for (int i = 0; i < n; i++){
        int a;
        fscanf(fin, "%d", &a);

        treapSearch(tr, a);
      }

      t = mtime() - t;
      fprintf(fout, "Time for searching %d elements: %li milliseconds.\n\n", n, t);
    }

    if (c == 'm'){
      Treap *x = treapMinimum(tr);
      if (x != NULL)
        fprintf(fout, "Minimum: %d\n\n", x->key);
      else
        fprintf(fout, "Empty tree\n\n");
    }
  }

  int fclose(FILE *fin);
  int fclose(FILE *fout);
}

int main(void)
{

  srand(time(NULL));
  test("test.txt", "output.txt");


  // Treap* tr = NULL;
  // Treap* tr2 = NULL;

  // for (int i = 0; i < 10; i++){
  //   Treap *t = makeNewTreap(i);
  //   insert(&tr, t);
  // }

  // for (int i = 0; i < 5; i++){
  //   Treap *t = makeNewTreap(i);
  //   insert(&tr2, t);
  // }

  // Print(tr, 0);
  // Print(tr2, 0);

  // Treap* res = unite(tr, tr2);

  // Print(res, 0);

  // return 0;
}