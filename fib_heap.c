#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct Node
{
  int key;
  int degree;
  char mark;
  struct Node *p;
  struct Node *left;
  struct Node *right;
  struct List *child;
} Node;

typedef struct List
{
  struct Node *nil;
} List;

Node* makeNewNode(int k)
{
  Node *n = (Node*)malloc(sizeof(Node));
  n->key = k;
  n->degree = 0;
  n->mark = 0;
  n->p = NULL;
  n->left = n;
  n->right = n;
  n->child = NULL;
  return n;
}

List* makeNewList()
{
  List *l = (List*)malloc(sizeof(List));
  l->nil = makeNewNode(0);
  return l;
}

void listDelete(List *l, Node *x)
{
  x->left->right = x->right;
  x->right->left = x->left;
}

Node* listSearch(List *l, int k)
{
  Node *x = l->nil->right;
  while(x != l->nil && x->key != k){
    x = x->right;
  }
  return x;
}

void listPrintf(List *l)
{
  Node *x = NULL;
  if (l->nil != NULL)
    x = l->nil->right;
  int i = 0;

  while(x != l->nil){
    i++;
    printf("%d-%d ", i, x->key);
    x = x->right;

  }
}

void listInsert(List *l, Node *x)
{
  x->right = l->nil->right;
  l->nil->right->left = x;
  l->nil->right = x;
  x->left = l->nil;
}

List* listUnion(List *l1, List *l2)
{
  List *l = makeNewList();
  l1->nil->left->right = l2->nil->right;
  l2->nil->right->left = l1->nil->left;
  l->nil->right = l1->nil->right;
  l1->nil->right->left = l->nil;
  l->nil->left = l2->nil->left;
  l2->nil->left->right = l->nil; 
  return l;
}

typedef struct FibHeap
{
  int n;
  Node *min;
  List* roots;
} FibHeap;

FibHeap* makeFibHeap()
{
  FibHeap* f = (FibHeap*)malloc(sizeof(FibHeap));
  f->n = 0;
  f->min = NULL;
  return f;
}

void fibHeapInsert(FibHeap *H, Node *x)
{
  if (H->min == NULL){
    List* l = makeNewList();
    listInsert(l, x);
    H->min = x;
    H->roots = l;
  }else{
    listInsert(H->roots, x);
    if (x->key < H->min->key)
      H->min = x;
  }
  H->n++;
}

Node* fibHeapMinimum(FibHeap* H)
{
  return H->min;
}

FibHeap* fibHeapUnion(FibHeap *H1, FibHeap *H2)
{
  FibHeap *H = makeFibHeap();
  H->min = H1->min;
  H->roots = listUnion(H1->roots, H2->roots);
  if (H1->min == NULL || (H2->min != NULL && H2->min->key < H1->min->key)){
    H->min = H2->min;
  }
  H->n = H1->n + H2->n;
  // free(H1);
  // free(H2);
  return H;
}

void fibHeapLink(FibHeap *H, Node *y, Node *x)
{

  listDelete(H->roots, y);
  if (x->child == NULL){
    x->child = makeNewList();
  }
  listInsert(x->child, y);
  y->p = x;
  x->degree++;
  y->mark = 0;
}

void Print(Node *q, long n) ////auxiliary function for testing
{
   long i;
   if (q)
   {
      Node *x = NULL;
      if (q->child != NULL){
        x = q->child->nil;
      }

      for (int i = 0; i < q->degree; i++){
        x = x->right;
        Print(x, n+5);
      }
      for (i = 0; i < n; i++) 
         printf(" ");

      printf("%d\n", q->key);
   }
}

void consolidate(FibHeap *H)
{
  int size = 0;
  if (H->n > 1)
    size = round(log((double)H->n - 1)/log(3.236067977/2.0));
  Node *A[size + 1];  //1

  for (int i = 0; i < size + 1; i++) //2
    A[i] = NULL;               //3

  Node* w = H->roots->nil->right;
  while(w != H->roots->nil){
    Node *x = w;

    int d = x->degree;

    while (A[d] != NULL){

      Node *y = A[d];
 
      if (x->key > y->key){
        Node *aux = y;
        y = x;
        x = aux;
      }

      fibHeapLink(H, y, x);
      A[d] = NULL;
      d++;
    }
    A[d] = x;
    w = w->right;
  }

  H->min = NULL;
  for (int i = 0; i < size; i++){
    if (A[i] != NULL){
      if (H->min == NULL){
        H->roots = makeNewList();
        listInsert(H->roots, A[i]);
        H->min = A[i];
      }else{
        listInsert(H->roots, A[i]);
        if (A[i]->key < H->min->key){
          H->min = A[i];
        }
      }
    }
  }
}

Node* fibHeapExtractMin(FibHeap *H)
{
  Node *z = H->min;

  if (z != NULL){

    if (z->child != NULL){
      Node *x = z->child->nil->right;

      while(x != z->child->nil){
        Node *aux = x->right;
        x->p = NULL;
        listInsert(H->roots, x);
        x = aux;
      }
    }

    listDelete(H->roots, z);

    if (z == z->right->right && z == z->left->left ){  //differ
      H->min = NULL;
    }else{
      if (z->right == H->roots->nil)
        H->min = z->left;
      else
        H->min = z->right;
      consolidate(H);
    }
    H->n--;
  } 
  return z;
}

void cut(FibHeap *H, Node *x, Node *y)
{
  y->degree--;

  listDelete(y->child, x);
  listInsert(H->roots, x);
  x->p = NULL;
  x->mark = 0;
}

void cascadingCut(FibHeap *H, Node *y)
{
  Node *z = y->p;
  if (z != NULL){
    if (y->mark == 0)
      y->mark = 1;
    else{
      cut(H, y, z);
      cascadingCut(H, z);
    }
  }
}

void fibHeapDecreaseKey(FibHeap *H, Node *x, int k)
{
  if (k > x->key)
    printf("New value is bigger than previous!\n");
  x->key = k;
  Node *y = x->p;

  if (y != NULL && x->key < y->key){
    cut(H, x, y);
    cascadingCut(H, y);
  }
  if (x->key < H->min->key)
    H->min = x;
}

void Print2(FibHeap *H)
{
  Node *t = H->roots->nil->right;
  while (t != H->roots->nil){
    Print(t, 0);
    t = t->right;
  }
  printf("=================\n");
}

void fibHeapDelete(FibHeap *H, Node *x)
{
  fibHeapDecreaseKey(H, x, -1e9);
  fibHeapExtractMin(H);
}

Node *fibHeapSearch(List *L, int key)
{
  Node *res = NULL;
  Node *h = L->nil->right;

  while (h != L->nil){
    if (res){
      return res;
    }
    if (h->key == key){
      res = h;
    }else{
      if (h->child){
        res = fibHeapSearch(h->child, key);
      }
    }
    h = h->right;
  }
  return res;
}

int main(void)  //// gcc -std=c99 fib_heap.c -lm
{
  srand(time(NULL));


  ///////// uncomment next part for testing

  // FibHeap* H = makeFibHeap();

  // for (int i = 0; i < 15; i++){
  //   Node *nd = makeNewNode(i);
  //   fibHeapInsert(H, nd);
  // }

  // printf("\n");
  // listPrintf(H->roots);
  // printf("\n");

  // fibHeapExtractMin(H);
  // fibHeapExtractMin(H);

  // Node *p = makeNewNode(20);

  // printf("%d\n", H->n);

  // fibHeapInsert(H, p);
  // printf("%d\n", H->n);

  // printf("=================\n");

  // Print2(H);

  // fibHeapDelete(H, p);


  // Print2(H);

  // Node *toDel = fibHeapSearch(H->roots, 10);

  // if (toDel != NULL)
  //   printf("toDel: %d\n", toDel->key);

  // fibHeapDelete(H, toDel);

  // Print2(H);

  // printf("\n");
  // listPrintf(H->roots);
  // printf("\n");

  // printf("%d\n", H->n);

  return 0;
}
