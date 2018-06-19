#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void checkIOError();
int findMatch(const char* from, char letter, size_t n);

int main(int argc, char* argv[]) {
  //Check if there are 2 arguments
  if (argc != 3) {
    fprintf(stderr, "Error: Wrong number of operands. Please input 2.\n");
    exit(1);
  }

  const char* from = argv[1];
  const char* to = argv[2];
  const int fromLength = strlen(from);
  const int toLength = strlen(to);

  //Check if from and to are the same length
  if (fromLength != toLength) {
    fprintf(stderr, "Error: Operands do have the same length.\n");
    exit(1);
  }

  //Check if from has any duplicates
  for (size_t i = 0; i < fromLength; i++) {
    for (size_t j = i+1; j < fromLength; j++) {
      if (from[i] == from[j]) {
	fprintf(stderr, "Error: From has duplicate bytes.\n");
	exit(1);
      }
    }
  }

  //Transliterate bytes from STDIN
  char currentChar = getchar();
  checkIOError();
  int matchExists = 0;
  while (currentChar != EOF) {
    int index = findMatch(from, currentChar, fromLength);
    if (index == -1) {
      putchar(currentChar);
      checkIOError();
    } else {
      putchar(to[index]);
      checkIOError();
      matchExists = 1;
    }

    currentChar = getchar();
    checkIOError();
    matchExists = 0;
  }

  return 0;
}

void checkIOError() {
  if (ferror(stdin) != 0) {
    fprintf(stderr, "Error with I/O.");
    exit(1);
  }
}

int findMatch(const char* from, char letter, size_t n) {
  for (size_t i = 0; i < n; i++) {
    if (from[i] == letter) {
      return i;
    }
  }
  return -1;
}
