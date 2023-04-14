#include <stdio.h>
#include <stdlib.h>
#include "record.h"

void initializeFile(FILE *f, int m)
{
  printf("Initializing the file...\n");

  Record r;
  r.occupied = false;
  for (int i = 0; i < m; i++)
  {
    r.pos = i;
    fwrite(&r, sizeof(Record), 1, f);
  }
}

void readRecords(FILE *f, int m)
{
  Record r;
  fseek(f, 0, SEEK_SET);
  for (int i = 0; i < m; i++)
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

void insertRecord(FILE *f, int pos, int val)
{
  Record r;
  r.occupied = true;
  r.pos = pos;
  r.value = val;

  // printf("Storing record in file ...\n");
  fseek(f, pos * sizeof(Record), SEEK_SET);
  if (fwrite(&r, sizeof(Record), 1, f) == 1) {
    // printf("Record stored successfully\n");
  } else {
    // printf("Error storing the record\n");
  }
}

bool checkRecord(FILE *f, int pos)
{
  Record r;
  fseek(f, pos * sizeof(Record), SEEK_SET);
  fread(&r, sizeof(Record), 1, f);
  return r.occupied == true;
}

int getNextIntFromFile(FILE *d)
{
  char line[100];
  int m;
  fgets(line, 100, d);
  sscanf(line, "%d", &m);
  return m;
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

  int m = getNextIntFromFile(d);

  initializeFile(f, m); // cria arquivo com m registros
  readRecords(f, m); // printa os m registos

  fclose(f);
  fclose(d);
  return 0;
}
