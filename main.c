#include <stdio.h>
#include <stdlib.h>
#include "record.h"

void initializeFile(FILE *f, int m)
{
  printf("Initializing \"database.dat\" file...\n");

  Record r;
  r.occupied = false;
  r.ptr = -1;
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
  printf("%-15s%-15s%-15s\n", "Addr", "Reg", "Ptr");
  for (int i = 0; i < m; i++)
  {
    fread(&r, sizeof(Record), 1, f);
    if (r.occupied)
    {
      printf("%-15d%-15d", r.pos, r.value);
      if (r.ptr != -1)
      {
        printf("%-15d\n", r.ptr);
      }
      else
      {
        printf("%-15s\n", "null");
      }
    }
    else
    {
      printf("%-15d%-15s%-15s\n", r.pos, "null", "null");
    }
  }
}

void insertRecord(FILE *f, int pos, int val, int ptr)
{
  Record r;
  r.occupied = true;
  r.pos = pos;
  r.value = val;
  r.ptr = ptr;

  // printf("Storing record in file ...\n");
  fseek(f, pos * sizeof(Record), SEEK_SET);
  if (fwrite(&r, sizeof(Record), 1, f) == 1)
  {
    // printf("Record stored successfully\n");
  }
  else
  {
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

void linearProbing(FILE *f, FILE *d, int m)
{
  while (!(feof(d)))
  {
    int n = getNextIntFromFile(d);
    int h = n % m;
    while (checkRecord(f, h))
    {
      h = h + 1 > m - 1 ? h - m : h + 1;
    }
    insertRecord(f, h, n, -1);
  }
}

void doubleHashing(FILE *f, FILE *d, int m)
{
  while (!(feof(d)))
  {
    int n = getNextIntFromFile(d);
    int h = n % m;
    int j = n >= m ? n / m : 1;
    while (checkRecord(f, h))
    {
      h = h + j > m - 1 ? h + j - m : h + j;
    }
    insertRecord(f, h, n, -1);
  }
}

int findEmptyPosition(FILE *f, int m)
{
  int pos = m - 1;
  while (checkRecord(f, pos))
  {
    pos -= 1;
  }
  return pos;
}

Record getRecord(FILE *f, int pos)
{
  Record r;
  fseek(f, pos * sizeof(Record), SEEK_SET);
  fread(&r, sizeof(Record), 1, f);
  return r;
}

Record getLastRecord(FILE *f, int pos)
{
  Record r = getRecord(f, pos);
  while (r.ptr != -1)
  {
    r = getRecord(f, r.ptr);
  }
  return r;
}

Record getRecordWithPtr(FILE *f, int pos, int ptr)
{
  Record r;
  r = getRecord(f, pos);
  while (r.ptr != ptr)
  {
    r = getRecord(f, r.ptr);
  }
  return r;
}

void explicitChaining(FILE *f, FILE *d, int m)
{
  while (!(feof(d)))
  {
    int n = getNextIntFromFile(d);
    int h = n % m;
    if (checkRecord(f, h))
    {
      Record r = getLastRecord(f, h);
      int ptr = findEmptyPosition(f, m);
      insertRecord(f, r.pos, r.value, ptr);
      h = findEmptyPosition(f, m);
    }
    insertRecord(f, h, n, -1);
  }
}

void explicitChainingNoJoining(FILE *f, FILE *d, int m)
{
  while (!(feof(d)))
  {
    int n = getNextIntFromFile(d);
    int h = n % m;
    if (checkRecord(f, h))
    {
      Record r = getRecord(f, h);
      int ptr = findEmptyPosition(f, m);
      if (r.value % m == r.pos)
      {
        r = getLastRecord(f, h);
        insertRecord(f, r.pos, r.value, ptr);
        h = ptr;
      }
      else
      {
        Record p = getRecordWithPtr(f, r.value % m, r.pos);
        insertRecord(f, p.pos, p.value, ptr);
        insertRecord(f, ptr, r.value, r.ptr);
      }
    }
    insertRecord(f, h, n, -1);
  }
}

int countIntegersInFile(FILE *d)
{
  int total = 0;
  rewind(d);
  while (!(feof(d)))
  {
    getNextIntFromFile(d);
    total++;
  }
  return total - 1;
}

int computeChainedHashingAccessCount(FILE *f, int j, int m, int n)
{
  Record r;
  int accesses = 0, h = n % m;
  do
  {
    r = getRecord(f, h);
    h = h + j > m - 1 ? h + j - m : h + j;
    accesses++;
  } while (r.value != n);
  return accesses;
}

int evaluateChainedHashingTotalAccesses(FILE *f, FILE *d, char type)
{
  rewind(d);
  int sum = 0, m = getNextIntFromFile(d);
  while (!(feof(d)))
  {
    int n = getNextIntFromFile(d);
    int j;
    if (type == 'd')
    {
      j = n >= m ? n / m : 1;
    }
    else
    {
      j = 1;
    }
    sum += computeChainedHashingAccessCount(f, j, m, n);
  }
  return sum;
}

int computeOpenAddressingAccessCount(FILE *f, int m, int n)
{
  Record r;
  int accesses = 0, h = n % m;
  do
  {
    r = getRecord(f, h);
    h = r.ptr;
    accesses++;
  } while (r.value != n);
  return accesses;
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
  // linearProbing(f, d, m);
  // doubleHashing(f, d, m);
  // explicitChaining(f, d, m);
  explicitChainingNoJoining(f, d, m);
  readRecords(f, m); // printa os m registos

  fclose(f);
  fclose(d);
  return 0;
}
