#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ctype.h>

int fOption = 0;

int decode(char c) {
  c = c ^ 42;
  if (fOption) {
    c = toupper((unsigned char) c);
  }
  return c;
}

int frobcmp(char const* a, char const* b) {
  for(; *a != ' ' && *b != ' '; a++, b++) {
    if (decode(*a) < decode(*b) || *a == ' ') {
      return -1;
    }
    if (decode(*a) > decode(*b) || *b == ' ') {
      return 1;
    }
  }
  return 0;
}

void checkError(ssize_t ret) {
  if (ret < 0) {
    char errMsg[] = "Error with system call function.\n";
    write(STDERR_FILENO, errMsg, sizeof(errMsg));
    exit(1);
  }
}

int compar(const void* a, const void* b) {
  return frobcmp(*(char**) a, *(char**) b);
}

int main(int argc, char* argv[]) {
  if (argc == 2 || argc == 3) {
    if (!(strncmp(argv[1], "-f", 2))) {
      fOption = 1;
    }
  }

  struct stat buf;
  ssize_t ret = fstat(STDIN_FILENO, &buf);
  checkError(ret);
  size_t fileSize = buf.st_size + 1;

  char* input;
  char** words;
  size_t itWords = 0;

  if (S_ISREG(buf.st_mode)) {
    input = (char*) malloc(sizeof(char) * fileSize);
    ret = read(STDIN_FILENO, input, buf.st_size);
    checkError(ret);

    int count = 0;
    for (size_t i = 0; i < buf.st_size; i++) {
      if (i == buf.st_size - 1) {
        input[i] = ' ';
      }

      if (i == 0 && input[i] != ' ') {
        count++;
      }

      if (input[i] == ' ') {
        while (input[i] == ' ' && i < buf.st_size) {
          i++;
        }

        if (i < buf.st_size) {
          count++;
        }
      }
    }

    words = (char**) malloc(sizeof(char*) * count);
    int addedWord = 0;
    for (size_t i = 0; i < buf.st_size; i++) {
      if (!addedWord && input[i] != ' ') {
        words[itWords] = &input[i];
        itWords++;
        addedWord = 1;
      } else if (addedWord && input[i] == ' ') {
        addedWord = 0;
      }
    }
  } else {
    words = (char**) malloc(sizeof(char*));

    char* word;
    word = (char*) malloc(sizeof(char));

    char currChar[1];
    ssize_t cret = read(STDIN_FILENO, currChar, 1);
    checkError(cret);
    char nextChar[1];
    ssize_t nret = read(STDIN_FILENO, nextChar, 1);
    checkError(nret);

    long itLetter = 0;
    while (cret > 0) {
      word[itLetter] = currChar[0];

      char* temp = (char*) realloc(word, (itLetter+2)*sizeof(char));
      if (temp == NULL) {
        free(word);
        char errMsg[] = "Error with memory allocation.\n";
        write(STDERR_FILENO, errMsg, sizeof(errMsg));
        exit(1);
      }
      word = temp;
      itLetter++;

      if (currChar[0] == ' ') {
        words[itWords] = word;
        char** tempWords = (char**) realloc(words, (itWords+2)*sizeof(char*));

        if (tempWords == NULL) {
          free(words);
          char errMsg[] = "Error with memory allocation.\n";
          write(STDERR_FILENO, errMsg, sizeof(errMsg));
          exit(1);
        }
        words = tempWords;
        itWords++;
        word = NULL;
        word = (char*) malloc(sizeof(char));
        itLetter = 0;
      }

      if (currChar[0] == ' ' && nret == 0) {
        break;
      } else if (nextChar[0] == ' ' && currChar[0] == ' ') {
        while (currChar[0] == ' ') {
          cret = read(STDIN_FILENO, currChar, 1);
          checkError(cret);
        }
        nret = read(STDIN_FILENO, nextChar, 1);
        checkError(nret);
        itLetter++;
        continue;
      } else if (nret == 0) {
        currChar[0] = ' ';
        continue;
      }

      currChar[0] = nextChar[0];
      nret = read(STDIN_FILENO, nextChar, 1);
      checkError(nret);
    }
    free(word);
  }

  qsort(words, itWords, sizeof(char*), compar);

  for(size_t i = 0; i < itWords; i++) {
    long wordLen = 0;
    for(size_t j = 0; ; j++) {
      wordLen++;
      if(words[i][j] == ' ') {
        break;
      }
    }

    ret = write(STDOUT_FILENO, words[i], wordLen);
    checkError(ret);
  }

  if (S_ISREG(buf.st_mode)) {
    free(input);
  } else {
    for (size_t i = 0; i < itWords; i++) {
      free(words[i]);
    }
  }
  free(words);
  return 0;
}
