#include <stdio.h>
#include <stdlib.h>
#include "record.h"

void create(FILE *f)
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

void read(FILE *f)
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

void insert(FILE *f)
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

bool check(FILE *f)
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
  FILE *f;

  printf("Creating \"database.dat\" ...\n");
  if ((f = fopen("database.dat", "r+")))
  {
    printf("File created successfully.\n");
  }
  else
  {
    printf("Error creating \"database.dat\" file.\n");
    exit(-1);
  }

  create(f);
  read(f);
  insert(f);
  read(f);
  check(f);
  fclose(f);
}
