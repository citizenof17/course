#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{

  srand(time(NULL));

  FILE *f;
  f = fopen("test.txt", "w");

  int stages = rand() % 15;
  char letters[] = {'r', 'd', 's', 'm'};

  for (int i = 0; i < stages; i++)
  {
    char c = letters[rand() % 4];
    if (c == 'm'){
      fprintf(f, "m\n");
      continue;
    }
    
    int elements_number = rand() % 1000000;

    fprintf(f, "%c\n%d\n", c, elements_number);
    for (int j = 0; j < elements_number; j++)
    {
      fprintf(f, "%d\n", rand() % 1000000);
    }
  }
}