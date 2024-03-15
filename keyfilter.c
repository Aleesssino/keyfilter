/* ==========================================================================*/
/*                                                                           */
/*   Program name:        keyfilter.c                                        */
/*   Program description: emulation a virtual navigation keyboard            */
/*   Author:              Aleš Gábor                                         */
/*   Date of change:      28-10-2023                                         */
/*                                                                           */
/* ==========================================================================*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 102 // max string length
#define NUM_CHARS 26   // number of English alphabet characters

// function to add a character to the hash set
void addToHashSet(char c, char *hashSet) {
  if (isalpha(c)) {
    hashSet[toupper(c) - 'A'] = 1;
  }
}

// function to print enabled characters
void printEnabledChars(const char *hashSet) {
  printf("Enable: ");
  int hasUppercase = 0;

  for (int i = 0; i < NUM_CHARS; i++) {
    if (hashSet[i]) {
      putchar(i + 'A');
      hasUppercase = 1;
    }
  }

  if (!hasUppercase) {
    putchar(' ');
  }

  putchar('\n');
}

// function to convert a string to uppercase and check for Start of Header
int convertToUppercase(char *prefix) {
  int isHeader = 0;

  for (int i = 0; prefix[i] != '\0'; i++) {
    if (prefix[i] == 1) {
      isHeader = 1;
      break;
    }
    prefix[i] = toupper(prefix[i]);
  }

  return isHeader;
}

// function to check if a prefix matches the start of a string
int isPrefixMatch(const char *prefix, const char *str) {
  return strncmp(prefix, str, strlen(prefix)) == 0;
}

// printing result
void printResult(int isHeader, int enableCount, const char *hashSet,
                 const char *found) {
  if (!isHeader) {
    if (enableCount > 1) {
      printEnabledChars(hashSet);
    } else if (enableCount == 1) {
      printf("Found: %s\n", found);
    } else {
      printf("Not found\n");
    }
  } else if (enableCount >= 1) {
    printEnabledChars(hashSet);
  }
}

// function to validate input and set the prefix
int validateInput(int argc, char *argv, char *prefix) {
  if (argc == 1) {
    prefix[0] = '\0';
  } else if (argc > 2) {
    fprintf(stderr, "ERROR: Too many arguments\n");
    return 1;
  } else {
    if (strlen(argv) <= MAX_LENGTH) {
      strncpy(prefix, argv, MAX_LENGTH);
      prefix[MAX_LENGTH - 1] = '\0';
    } else {
      fprintf(stderr, "ERROR: Argument is greater than %d characters\n",
              MAX_LENGTH);
      return 1;
    }
  }

  return convertToUppercase(prefix);
}

int main(int argc, char *argv[]) {
  char str[MAX_LENGTH];          // input string
  char hashSet[NUM_CHARS] = {0}; // initialize the hash set to all zeros
  char prefix[MAX_LENGTH];       // prefix string
  char found[MAX_LENGTH];        // found string
  int enableCount = 0;           // number of enabled characters
  int isHeader = 0;              // start of header
  int i = 0;

  if (validateInput(argc, argv[1], prefix)) {
    return EXIT_FAILURE;
  }

  while (fgets(str, sizeof(str), stdin)) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
      str[len - 1] = '\0';
    }

    if (convertToUppercase(str)) {
      fprintf(stderr, "IGNORED: %s (is not alphabetic)\n", str);
      continue;
    } else if (isPrefixMatch(prefix, str)) {
      if (!isHeader) {
        addToHashSet(str[strlen(prefix)], hashSet);
        enableCount++;
        strncpy(found, str, MAX_LENGTH);
        found[MAX_LENGTH - 1] = '\0';
      }
    }
    i++;
  }

  if (i == 1 && str[0] == 1) {
    fprintf(stderr, "ERROR: Textfile is empty\n");
    return EXIT_FAILURE;
  }
  printResult(isHeader, enableCount, hashSet, found);
  return 0;
}
