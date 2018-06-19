#include <stdio.h>
#include <stdlib.h>

int frobcmp(char const* a, char const* b) {
  for(; *a != ' ' && *b != ' '; a++, b++) {
    if ((*a ^ 42) < (*b ^ 42) || *a == ' ') {
      return -1;
    }
    if ((*a ^ 42) > (*b ^ 42) || *b == ' ') {
      return 1;
    }
  }
  return 0;
}

void checkInputErr() {
  if (ferror(stdin) != 0) {
    fprintf(stderr, "Error with reading input!");
    exit(1);
  }
}

void checkOutputErr() {
  if (ferror(stdin) != 0) {
    fprintf(stderr, "Error with printing output!");
    exit(1);
  }
}

int compar(const void* a, const void* b) {
  return frobcmp(*(char**) a, *(char**) b);
}

int main(void) {
  char* currWord = (char*) malloc(sizeof(char));
  char** wordArr = (char**) malloc(sizeof(char*));

  char curr = getchar();
  checkInputErr();
  char next = getchar();
  checkInputErr();

  int itLetter = 0;
  int itWord = 0;

  int isEOF = feof(stdin);
  while (!isEOF) {
    currWord[itLetter] = curr;

    char* tempWord = (char*) realloc(currWord, (itLetter+2) * sizeof(char));
    if (tempWord == NULL) {
      free(currWord);
      fprintf(stderr, "Error with memory allocation!");
      exit(1);
    }
    currWord = tempWord;
    itLetter++;

    if (curr == ' ') {
      wordArr[itWord] = currWord;

      char** tempWordArr = (char**) realloc(wordArr, (itWord+2) * sizeof(char*));
      if (tempWordArr == NULL) {
      	free(wordArr);
      	fprintf(stderr, "Error with memory allocation!");
      	exit(1);
      }
      wordArr = tempWordArr;
      itWord++;
      currWord = NULL;
      currWord = (char*) malloc(sizeof(char));
      itLetter = 0;
    }

    isEOF = feof(stdin);
    if (curr == ' ' && isEOF) {
      break;
    } else if (curr == ' ' && next == ' ') {
      while (curr == ' ') {
        curr = getchar();
        checkInputErr();
      }
      next = getchar();
      checkInputErr();
      itLetter++;
      continue;
    } else if (isEOF) {
      curr = ' ';
      currWord[itLetter] = curr;
      wordArr[itWord] = currWord;

      char** tempWordArr = (char**) realloc(wordArr, (itWord+2) * sizeof(char*));
      if (tempWordArr == NULL) {
      	free(wordArr);
      	fprintf(stderr, "Error with memory allocation!");
      	exit(1);
      }
      wordArr = tempWordArr;
      itWord++;
      break;
    }

    curr = next;
    next = getchar();
    checkInputErr();
  }

  qsort(wordArr, itWord, sizeof(char*), compar);

  for(size_t i = 0; i < itWord; i++) {
  	for(size_t j = 0; ; j++) {
  		putchar(wordArr[i][j]);
      checkOutputErr();
  		if(wordArr[i][j] == ' ') {
  		  break;
  		}
	  }
  }

  for (size_t i = 0; i < itWord; i++) {
    free(wordArr[i]);
  }
  free(wordArr);

  return 0;
}
