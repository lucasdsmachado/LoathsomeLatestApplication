#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "record.h"

#define SIZE 997

void populateFile(FILE* d, int size, int percent) {
  double numbers = size * percent / 100.0;
  fprintf(d, "%d\n", size);
  for (int i = 0; i < numbers; i++) {
    int n = rand();
    fprintf(d, "%d", n);
    if (i < numbers - 1) {
      fprintf(d, "\n");
    }
  }
  rewind(d);
}

void initializeFile(FILE* f, int m) {
  Record r;
  r.occupied = false;
  r.ptr = -1;
  for (int i = 0; i < m; i++) {
    r.pos = i;
    fwrite(&r, sizeof(Record), 1, f);
  }
}

void readRecords(FILE* f, int m) {
  Record r;
  fseek(f, 0, SEEK_SET);
  printf("%-15s%-15s%-15s\n", "Addr", "Reg", "Ptr");
  for (int i = 0; i < m; i++) {
    fread(&r, sizeof(Record), 1, f);
    if (r.occupied) {
      printf("%-15d%-15d", r.pos, r.value);
      if (r.ptr != -1) {
        printf("%-15d\n", r.ptr);
      } else {
        printf("%-15s\n", "null");
      }
    } else {
      printf("%-15d%-15s%-15s\n", r.pos, "null", "null");
    }
  }
}

void writeRecords(FILE* f, FILE* hf, int m) {
  Record r;
  rewind(f);
  fprintf(hf, "%-15s%-15s%-15s\n", "Addr", "Reg", "Ptr");
  for (int i = 0; i < m; i++) {
    fread(&r, sizeof(Record), 1, f);
    if (r.occupied) {
      fprintf(hf, "%-15d%-15d", r.pos, r.value);
      if (r.ptr != -1) {
        fprintf(hf, "%-15d\n", r.ptr);
      } else {
        fprintf(hf, "%-15s\n", "null");
      }
    } else {
      fprintf(hf, "%-15d%-15s%-15s\n", r.pos, "null", "null");
    }
  }
  fprintf(hf, "\n\n");
}

void insertRecord(FILE* f, int pos, int val, int ptr) {
  Record r;
  r.occupied = true;
  r.pos = pos;
  r.value = val;
  r.ptr = ptr;

  // printf("Storing record in file ...\n");
  fseek(f, pos * sizeof(Record), SEEK_SET);
  if (fwrite(&r, sizeof(Record), 1, f) == 1) {
    // printf("Record stored successfully\n");
  } else {
    // printf("Error storing the record\n");
  }
}

bool checkRecord(FILE* f, int pos) {
  Record r;
  fseek(f, pos * sizeof(Record), SEEK_SET);
  fread(&r, sizeof(Record), 1, f);
  return r.occupied == true;
}

int getNextIntFromFile(FILE* d) {
  int num;
  fscanf(d, "%d", &num);
  return num;
}

void linearProbing(FILE* f, FILE* d, int m) {
  while (!(feof(d))) {
    int n = getNextIntFromFile(d);
    int h = n % m;
    while (checkRecord(f, h)) {
      h = (h + 1) % m;
    }
    insertRecord(f, h, n, -1);
  }
}

void doubleHashing(FILE* f, FILE* d, int m) {
  while (!(feof(d))) {
    int n = getNextIntFromFile(d);
    int h = n % m;
    int j = (n >= m && (n / m) % m != 0) ? ((n / m) % m) : 1;
    while (checkRecord(f, h)) {
      h = (h + j) % m;
    }
    insertRecord(f, h, n, -1);
  }
}

int findEmptyPosition(FILE* f, int m) {
  int pos = m - 1;
  while (checkRecord(f, pos)) {
    pos -= 1;
  }
  return pos;
}

Record getRecord(FILE* f, int pos) {
  Record r;
  fseek(f, pos * sizeof(Record), SEEK_SET);
  fread(&r, sizeof(Record), 1, f);
  return r;
}

Record getLastRecord(FILE* f, int pos) {
  Record r = getRecord(f, pos);
  while (r.ptr != -1) {
    r = getRecord(f, r.ptr);
  }
  return r;
}

Record getRecordWithPtr(FILE* f, int pos, int ptr) {
  Record r;
  r = getRecord(f, pos);
  while (r.ptr != ptr) {
    r = getRecord(f, r.ptr);
  }
  return r;
}

void explicitChaining(FILE* f, FILE* d, int m) {
  while (!(feof(d))) {
    int n = getNextIntFromFile(d);
    int h = n % m;
    if (checkRecord(f, h)) {
      Record r = getLastRecord(f, h);
      int ptr = findEmptyPosition(f, m);
      insertRecord(f, r.pos, r.value, ptr);
      h = findEmptyPosition(f, m);
    }
    insertRecord(f, h, n, -1);
  }
}

void explicitChainingNoJoining(FILE* f, FILE* d, int m) {
  while (!(feof(d))) {
    int n = getNextIntFromFile(d);
    int h = n % m;
    if (checkRecord(f, h)) {
      Record r = getRecord(f, h);
      int ptr = findEmptyPosition(f, m);
      if (r.value % m == r.pos) {
        r = getLastRecord(f, h);
        insertRecord(f, r.pos, r.value, ptr);
        h = ptr;
      } else {
        Record p = getRecordWithPtr(f, r.value % m, r.pos);
        insertRecord(f, p.pos, p.value, ptr);
        insertRecord(f, ptr, r.value, r.ptr);
      }
    }
    insertRecord(f, h, n, -1);
  }
}

int countIntegersInFile(FILE* d) {
  int total = 0;
  while (!(feof(d))) {
    getNextIntFromFile(d);
    total++;
  }
  rewind(d);
  return total - 1;
}

int computeOpenAddressingAccessCount(FILE* f, int j, int m, int n) {
  Record r;
  int accesses = 0, h = n % m;
  do {
    r = getRecord(f, h);
    h = (h + j) % m;
    accesses++;
  } while (r.value != n);
  return accesses;
}

int evaluateOpenAddressingTotalAccesses(FILE* f, FILE* d, char type) {
  rewind(d);
  int sum = 0, m = getNextIntFromFile(d);
  while (!(feof(d))) {
    int n = getNextIntFromFile(d);
    int j;
    if (type == 'd') {
      j = (n >= m && (n / m) % m != 0) ? ((n / m) % m) : 1;
    } else {
      j = 1;
    }
    sum += computeOpenAddressingAccessCount(f, j, m, n);
  }
  return sum;
}

int computeChainedHashingAccessCount(FILE* f, int m, int n) {
  Record r;
  int accesses = 0, h = n % m;
  do {
    r = getRecord(f, h);
    h = r.ptr;
    accesses++;
  } while (r.value != n);
  return accesses;
}

int evaluateChainedHashingTotalAccesses(FILE* f, FILE* d) {
  rewind(d);
  int sum = 0, m = getNextIntFromFile(d);
  while (!(feof(d))) {
    int n = getNextIntFromFile(d);
    sum += computeChainedHashingAccessCount(f, m, n);
  }
  return sum;
}

int main() {
  FILE *f, *d;
  FILE *lp, *dh, *ec, *ecnj;

  printf("Acessing \"database.dat\" and \"file.txt\"...\n");
  if ((f = fopen("database.dat", "w+")) && (d = fopen("file.txt", "r+")) &&
      (lp = fopen("linearProbing.txt", "w")) &&
      (dh = fopen("doubleHashing.txt", "w")) &&
      (ec = fopen("explicitChaining.txt", "w")) &&
      (ecnj = fopen("explicitChainingNoJoining.txt", "w"))) {
    printf("Files accessed successfully.\n");
  } else {
    printf("Error accessing files.\n");
    exit(-1);
  }

  srand(time(NULL));

  for (int i = 2; i < 19; i++) {
    printf("Current percentage:\t%d\n", i * 5);
    fprintf(lp, "Current percentage:\t%d\n", i * 5);
    fprintf(dh, "Current percentage:\t%d\n", i * 5);
    fprintf(ec, "Current percentage:\t%d\n", i * 5);
    fprintf(ecnj, "Current percentage:\t%d\n", i * 5);
    for (int j = 1; j <= 10; j++) {
      populateFile(d, SIZE, 5 * i);
      int n = countIntegersInFile(d);
      int m = getNextIntFromFile(d);
      initializeFile(f, m);
      linearProbing(f, d, m);
      printf("LP:\t%.2f\n",
             (double)evaluateOpenAddressingTotalAccesses(f, d, 'l') / n);
      writeRecords(f, lp, m);
      rewind(d);
      rewind(f);

      getNextIntFromFile(d);
      initializeFile(f, SIZE);
      doubleHashing(f, d, m);
      printf("DB:\t%.2f\n",
             (double)evaluateOpenAddressingTotalAccesses(f, d, 'd') / n);
      writeRecords(f, dh, m);
      rewind(d);
      rewind(f);

      getNextIntFromFile(d);
      initializeFile(f, SIZE);
      explicitChaining(f, d, m);
      printf("EC:\t%.2f\n",
             (double)evaluateChainedHashingTotalAccesses(f, d) / n);
      writeRecords(f, ec, m);
      rewind(d);
      rewind(f);

      getNextIntFromFile(d);
      initializeFile(f, SIZE);
      explicitChainingNoJoining(f, d, m);
      printf("ECNJ:\t%.2f\n",
             (double)evaluateChainedHashingTotalAccesses(f, d) / n);
      writeRecords(f, ec, m);

      fflush(d);
      ftruncate(fileno(d), 0);
      rewind(f);
      rewind(d);
    }
  }

  fclose(f);
  fclose(d);
  fclose(lp);
  fclose(dh);
  fclose(ec);
  fclose(ecnj);
  return 0;
}
