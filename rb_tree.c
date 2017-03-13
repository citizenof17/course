#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

long long size = 0;

typedef struct Node
{
  int key;
  char color;
  struct Node *left;
  struct Node *right;
  struct Node *p;
} Node;

typedef struct Tree
{
  Node *root;
  Node *nil;
} Tree;

Node* makeNewNode(Tree *T, int value)
{
  Node* nd = (Node*)malloc(sizeof(Node));
  nd->left = nd->right = T->nil;
  nd->p = T->nil;
  nd->key = value;

  return nd;
}

void Print(Node *q, long n);

Node* treeSearch(Tree *T, Node *x, int k)  //x is a root of the subtree
{
  if (x == T->nil || k == x->key)
    return x;

  if (k < x->key)
    return treeSearch(T, x->left, k);  //find in the left child
  else
    return treeSearch(T, x->right, k);  //find in the right child
}

Node* treeMinimum(Tree *T, Node *x)  
{
  while(x != T->nil && x->left != T->nil)   
    x = x->left;
  return x;
}

Node* treeMaximum(Tree *T, Node *x)
{
  while(x != T->nil && x->right != T->nil)
    x = x->right;
  return x;
}

void leftRotate(Tree *T, Node *x)
{
  Node *y = x->right;   
  x->right = y->left;   //transfer left y son to right x's

  if (y->left != T->nil)   
    y->left->p = x;    //change y's left son's parent 
  y->p = x->p;    //change y's parent

  if (x->p == T->nil)  
    T->root = y;   //new tree root
  else{
    if (x == x->p->left)  
      x->p->left = y;   //x's parent's left son is y 
    else
      x->p->right = y;  //x's parent's right son is y 
  }

  y->left = x; 
  x->p = y;
}

void rightRotate(Tree *T, Node *x) //same as left rotate but all lefts changed to rights
{
  Node *y = x->left;
  x->left = y->right;

  if (y->right != T->nil)
    y->right->p = x;
  y->p = x->p;

  if (x->p == T->nil)
    T->root = y;
  else{
    if (x == x->p->right)
      x->p->right = y;
    else
      x->p->left = y;
  }

  y->right = x;
  x->p = y;
}

void insertFixup(Tree *T, Node *z)
{
  while (z->p->color == 'r'){
    if (z->p == z->p->p->left){
      Node *y = z->p->p->right;
      if (y->color == 'r'){
        z->p->color = 'b';
        y->color = 'b';
        z->p->p->color = 'r';
        z = z->p->p;
      }else{
        if (z == z->p->right){
          z = z->p;
          leftRotate(T, z);
        }
        z->p->color = 'b';
        z->p->p->color = 'r';
        rightRotate(T, z->p->p);
      }
    }else{
      Node *y = z->p->p->left;
      if (y->color == 'r'){
        z->p->color = 'b';
        y->color = 'b';
        z->p->p->color = 'r';
        z = z->p->p;
      }else{
        if (z == z->p->left){
          z = z->p;
          rightRotate(T, z);
        }
        z->p->color = 'b';
        z->p->p->color = 'r';
        leftRotate(T, z->p->p);
      }
    }
  }
  T->root->color = 'b';
}

void insert(Tree *T, int val)  //rb-insert
{
  Node *y = T->nil;
  Node *x = T->root;
  Node *z = makeNewNode(T, val);

  size += sizeof(z);

  while (x != T->nil){
    y = x;
    if (z->key < x->key)
      x = x->left;
    else
      x = x->right;
  }

  z->p = y;

  if (y == T->nil)
    T->root = z;
  else{
    if (z->key < y->key)
      y->left = z;
    else
      y->right = z;
  }
  z->left = T->nil;
  z->right = T->nil;
  z->color = 'r';

  insertFixup(T, z);
}

void transplant(Tree *T, Node *u, Node *v)
{
  if (u->p == T->nil)
    T->root = v;
  else{
    if (u == u->p->left)
      u->p->left = v;
    else
      u->p->right = v;
  }
  v->p = u->p;
}

void deleteFixup(Tree *T, Node *x)
{
  while (x != T->root  && x->color == 'b'){
    if (x == x->p->left){
      Node *w = x->p->right;
      if (w->color == 'r'){
        w->color = 'b';
        x->p->color = 'r';
        leftRotate(T, x->p);
        w = x->p->right;
      }
      if (w->left->color == 'b' && w->right->color == 'b'){
        w->color = 'r';
        x = x->p;
      }else{
        if (w->right->color == 'b'){
          w->left->color = 'b';
          w->color = 'r';
          rightRotate(T, w);
          w = x->p->right;
        }
        w->color = x->p->color;
        x->p->color = 'b';
        w->right->color = 'b';
        leftRotate(T, x->p);
        x = T->root;
      }
    }else{
      Node *w = x->p->left;
      if (w->color == 'r'){
        w->color = 'b';
        x->p->color = 'r';
        rightRotate(T, x->p);
        w = x->p->left;
      }
      if (w->right->color == 'b' && w->left->color == 'b'){
        w->color = 'r';
        x = x->p;
      }else{
        if (w->left->color == 'b'){
          w->right->color = 'b';
          w->color = 'r';
          leftRotate(T, w);
          w = x->p->left;
        }
        w->color = x->p->color;
        x->p->color = 'b';
        w->left->color = 'b';
        rightRotate(T, x->p);
        x = T->root;
      }
    }
  }
  x->color = 'b';
}

void delete(Tree *T, int val){
  Node *z = treeSearch(T, T->root, val);
  Node *x = NULL;
  x = (Node*)malloc(sizeof(Node));

  if (z != T->nil){
    Node *y = z;
    char yColor = y->color;
    if (z->left == T->nil){
      x = z->right;
      transplant(T, z, z->right);
    }else{
      if (z->right == T->nil){
        x = z->left;
        transplant(T, z, z->left);
      }else{
        y = treeMinimum(T, z->right);
        yColor = y->color;
        x = y->right;
        if (y->p == z)
          x->p = y;
        else{
          transplant(T, y, y->right);
          y->right = z->right;
          y->right->p = y;
        }
        transplant(T, z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
      }
    }
  
    if (yColor == 'b')
      deleteFixup(T, x);

    size -= sizeof(z);
    free(z);
  }
}

// void Print(Node *q, long n) ////auxiliary function for testing
// {
//    long i;
//    if (q)
//    {
//       Print(q->right, n+5);
//       for (i = 0; i < n; i++) 
//          printf(" ");
//       printf("%d%c\n", q->key, q->color);
//       Print(q->left, n+5);
//    }
// }

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

  int c;

  Tree *tr = NULL;
  tr = (Tree*)malloc(sizeof(Tree));
  tr->nil = makeNewNode(tr, 0);
  tr->root = tr->nil;

  size += sizeof(tr);

  fprintf(fout, "The size of the empty tree: %lli bytes\n", size);

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
        insert(tr, a);
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
        delete(tr, a);
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

        treeSearch(tr, tr->root , a);
      }

      t = mtime() - t;
      fprintf(fout, "Time for searching %d elements: %li milliseconds.\n\n", n, t);
    }

    if (c == 'm'){
      Node *x = treeMinimum(tr, tr->root);
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

  // char* a = "test.txt";
  // char* b = "output.txt";
  // FILE *f;
  // f = fopen(a, "r");

  // int t;
  // fscanf(f, "%d", &t);

  // FILE *o;
  // o = fopen(b, "w");
  // fprintf(o, "%d", t);

  // Tree *tr = NULL;
  // tr = (Tree*)malloc(sizeof(Tree));
  // tr->nil = makeNewNode(tr, 0);
  // tr->root = tr->nil;

  // int n;
  // scanf("%d", &n);

  // int p = 0;

  // for (int i = 0; i < n; i++){
  //   int a;
  //   scanf("%d", &a);
  //   insert(tr, a);
  // }


  // // Print(tr->root, 0);

  // for (int i = 0; i < n; i++){
  //   int a; 
  //   scanf("%d", &a);
  //   delete(tr, a);
  //   // Print(tr->root, 0);

  // }

  // // Print(tr->root, 0);

  // gcc -std=c99 rb_tree.c -lm
  return 0;
}