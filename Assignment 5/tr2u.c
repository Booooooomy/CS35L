#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int findMatch(const char* from, char letter, size_t n);

int main(int argc, char* argv[]) {
  //Check if there are 2 arguments
  if (argc != 3) {
    char errMsg[] = "Error: Wrong number of operands. Please input 2.\n";
    write(STDERR_FILENO, errMsg, sizeof(errMsg));
    exit(1);
  }

  const char* from = argv[1];
  const char* to = argv[2];
  const int fromLength = strlen(from);
  const int toLength = strlen(to);

  //Check if from and to are the same length
  if (fromLength != toLength) {
    char errMsg[] = "Error: Operands do have the same length.\n";
    write(STDERR_FILENO, errMsg, sizeof(errMsg));
    exit(1);
  }

  //Check if from has any duplicates
  for (size_t i = 0; i < fromLength; i++) {
    for (size_t j = i+1; j < fromLength; j++) {
      if (from[i] == from[j]) {
        char errMsg[] = "Error: From has duplicate bytes.\n";
        write(STDERR_FILENO, errMsg, sizeof(errMsg));
      	exit(1);
      }
    }
  }

  //Transliterate bytes from STDIN
  char currentChar[2];
  int matchExists;
  while (read(STDIN_FILENO, currentChar, 1) > 0) {
    int index = findMatch(from, currentChar[0], fromLength);
    if (index == -1) {
      write(STDOUT_FILENO, currentChar, 1);
    } else {
      currentChar[0] = to[index];
      write(STDOUT_FILENO, currentChar, 1);
    }
  }

  return 0;
}

int findMatch(const char* from, char letter, size_t n) {
  for (size_t i = 0; i < n; i++) {
    if (from[i] == letter) {
      return i;
    }
  }
  return -1;
}
