#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define PERCENT 50
#define SIZE 11

int main()
{
  FILE *d;
  printf("Acessing \"file.txt\"...\n");
  if ((d = fopen("file.txt", "w+")))
  {
    printf("Files accessed successfully.\n");
  }
  else
  {
    printf("Error accessing files.\n");
    exit(-1);
  }
  
  int numbers = SIZE * PERCENT / 100.0;
  fprintf(d, "%d\n", SIZE);
  srand(time(NULL));
  for (int i = 0; i < numbers; i++)
  {
    int n = rand();
    fprintf(d, "%d", n);
    if (i < numbers - 1)
    {
      fprintf(d, "\n");
    }
  }
  
  fclose(d);
  return 0;
}