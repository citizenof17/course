#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

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
  if (l == NULL || r == NULL){
    // free(*t);
    *t = l ? l : r;
  }
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
  if ((**t).key == key){
    free(*t);
    merge(t, (**t).l, (**t).r);
  }
  else{
    delete(key < (**t).key ? &(**t).l : &(**t).r, key);
  }
}

Treap* unite(Treap *l, Treap *r)
{
  if (!l || !r)  return l ? l : r;
  if (l->prior < r->prior){
    Treap* aux = l;
    l = r;
    r = aux;
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
       printf("%d\n", q->key);

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
  while(t && t->l){
    t = t->l;
  }
  return t;
}

void TreapClear(Treap **t){
  if (*t == NULL)
    return;
  TreapClear(&(**t).l);
  TreapClear(&(**t).r);
  free(*t);
}


int main(void)
{

  srand(time(NULL));

  FILE *fout;
  fout = fopen("treap3.txt", "w");

  int Test = 500;
  const int T = Test;
  //2500000
  //900000
  int nPerTest = 2500000;
  double tests[T];
  for (int n = 1000; n <= 100000; n += 3000, nPerTest -= 48480){
    fprintf(fout, "%d %d\n", n, nPerTest);
    Test = T;
    for (int i = 0; i < T; i++){
      tests[i] = 0;
    }
    while(Test > 0){
      Test--;
      Treap *tr = NULL;

      int nElem = nPerTest;

      int p = n + 1;
      const int N = p;
      int arr[N];

      for (int i = 0; i < p; i++){
        int a = rand() % 1000000000;
        Treap *t = makeNewTreap(a);    
        insert(&tr, t);
        arr[i] = a;
      }

      clock_t ti1 = clock();

      for (int i = 0; i < nElem; i++){
        delete(&tr, arr[i % n]);
        int a = rand() % 1000000000;
        Treap *t = makeNewTreap(a);    
        insert(&tr, t);
        arr[i % n] = a;
      }

      clock_t ti2 = clock();

      TreapClear(&tr);
      double ans = (double)(ti2 - ti1) / CLOCKS_PER_SEC;
      fprintf(fout, "%f\n", ans);
      tests[Test] = ans;
    }
  
    int dis[16] = {0};

    double mx = 0;
    double mn = 10;

    for (int i = 0; i < T; i++){
      if (tests[i] > mx)
        mx = tests[i];
      if (tests[i] < mn)
        mn = tests[i];
    }

    double step = (mx - mn) / 16;

    for (int i = 0; i < T; i++){
      dis[(int)((tests[i] - mn) / step)]++;
    }

    for (int i = 0; i < 16; i++){
      fprintf(fout, "%d ", dis[i]);
    }
      fprintf(fout, "\n");
  }
}
