#include <stdio.h>
#include <stdlib.h>
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

int main(void)
{

  srand(time(NULL));

  Treap* tr = NULL;
  Treap* tr2 = NULL;


  // int n;
  // scanf("%d", &n);

  // for (int i = 0; i < n; i++){
  //   int a;
  //   scanf("%d", &a);
  //   Treap *t = makeNewTreap(a);
  //   // printf("%d\n", t->key);
  //   insert(&tr, t);
  //   // printf("%d %d\n", tr->key, tr->prior);

  //   if (tr == NULL)
  //     printf("NULL\n");
  // }

  // Print(tr, 0);

  // scanf("%d", &n);

  // for (int i = 0; i < n; i++){
  //   int a; 
  //   scanf("%d", &a);
  //   delete(&tr, a);
  // }


  for (int i = 0; i < 10; i++){
    Treap *t = makeNewTreap(i);
    insert(&tr, t);
  }

  for (int i = 0; i < 5; i++){
    Treap *t = makeNewTreap(i);
    insert(&tr2, t);
  }

  Print(tr, 0);
  Print(tr2, 0);

  Treap* res = unite(tr, tr2);

  Print(res, 0);

  return 0;
}
