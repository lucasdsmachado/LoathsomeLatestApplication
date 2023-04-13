#include <stdio.h>
#include <stdlib.h>
#include "record.h"

void initializeFile(FILE *f)
{
  printf("Initializing the file...\n");

  Record r;
  r.occupied = false;
  for (int i = 0; i < 11; i++)
  {
    r.pos = i;
    fwrite(&r, sizeof(Record), 1, f);
  }
}

void readRecords(FILE *f)
{
  Record r;
  fseek(f, 0, SEEK_SET);
  for (int i = 0; i < 11; i++)
  {
    fread(&r, sizeof(Record), 1, f);
    printf("Record: %d\n", r.pos);
    if (r.occupied == true)
    {
      printf("Occupied: true\n");
      printf("Value: %d\n", r.value);
    }
    else
    {
      printf("Ocuppied: false\n");
    }
    printf("\n");
  }
}

void insertRecord(FILE *f)
{
  int pos;
  Record r;

  printf("position: ");
  scanf("%d", &pos);
  printf("value: ");
  scanf("%d", &(r.value));
  r.occupied = true;
  r.pos = pos;

  printf("Storing record in file ...\n");
  fseek(f, pos * sizeof(Record), SEEK_SET);
  if (fwrite(&r, sizeof(Record), 1, f) == 1)
    printf("Record stored successfully\n");
  else
    printf("Error storing the record\n");
}

bool checkRecord(FILE *f)
{
  Record r;
  int pos;

  printf("position: ");
  scanf("%d", &pos);

  fseek(f, pos * sizeof(Record), SEEK_SET);
  fread(&r, sizeof(Record), 1, f);
  if (r.occupied == true)
  {
    printf("Record: %d\n", r.pos);
    printf("Value: %d\n", r.value);
    printf("Ocuppied: true\n");

    return true;
  }
  else
  {
    return false;
  }
}

int main()
{
  FILE *f, *d;

  printf("Acessing \"database.dat\" and \"file.txt\"...\n");
  if ((f = fopen("database.dat", "r+")) && (d = fopen("file.txt", "r+")))
  {
    printf("Files accessed successfully.\n");
  }
  else
  {
    printf("Error accessing files.\n");
    exit(-1);
  }

  initializeFile(f); // cria arquivo com 11 registros
  readRecords(f);    // printa os 11 registos
  insertRecord(f);   // insere registro em uma determinada posicao
  readRecords(f);    // printa denovo para ver ser gravou no arquivo
  checkRecord(f);    // checa determinada posição do arquivo

  fclose(f);
  fclose(d);
  return 0;
}
