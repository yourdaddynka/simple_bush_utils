#include "s21_cat.h"
struct option longOptions[] = {{"number-nonblank", 0, NULL, 'b'},
                               {"number", 0, NULL, 'n'},
                               {"squeeze-blank", 0, NULL, 's'},
                               {NULL, 0, NULL, 0}};
int main(int argc, char *argv[]) {
  Flags flags = {0};
  parser_cat(argc, argv, &flags);
  while (optind < argc) {
    print_cat_flags(argv, &flags);
    optind++;
  }
  return 0;
}

void parser_cat(int argc, char **argv, Flags *flags) {
  int current_flag = 0;
  while ((current_flag =
              getopt_long(argc, argv, "+bevEnstT", longOptions, NULL)) != -1) {
    switch (current_flag) {
      case 'b':
        flags->NumberNonBlank = 1;
        break;
      case 'e': {
        flags->SymbolEnd = 1;
        flags->PrintNonPrintable = 1;
        break;
      }
      case 'v':
        flags->PrintNonPrintable = 1;
        break;
      case 'E':
        flags->SymbolEnd = 1;
        break;
      case 'n':
        flags->ALLnumber = 1;
        break;
      case 's':
        flags->Squeezeblank = 1;
        break;
      case 't': {
        flags->PrintNonPrintable = 1;
        flags->Tab = 1;
        break;
      }
      case 'T':
        flags->Tab = 1;
        break;
      case '?': {
        fprintf(stderr, "usage: ./s21_cat [-benstvET] [file ...]\n");
        current_flag = -1;
        break;
      }
    }
    if (flags->NumberNonBlank == 1 && flags->ALLnumber == 1) {
      flags->ALLnumber = 0;
    }
  }
}

void print_cat_flags(char **argv, Flags *flags) {
  Param param = {0};
  param.All = 1;
  char symbol = '0';
  char last_sym = '\n';
  FILE *filename = fopen(argv[optind], "r");
  if (filename != NULL) {
    while ((symbol = fgetc(filename)) != EOF) {
      if (flags->Squeezeblank && symbol == '\n' && last_sym == '\n') {
        param.count_string_s++;
        if (param.count_string_s > 1) {
          continue;
        }
      } else {
        param.count_string_s = 0;
      }
      if (flags->NumberNonBlank) {
        numbernonblank(symbol, &param);
      }
      if (flags->ALLnumber) {
        number(symbol, &param);
      }

      if (flags->SymbolEnd) {
        if (symbol == '\n') printf("$");
      }
      if (flags->Tab) {
        if (symbol == '\t') {
          symbol = 'I';
          printf("^");
        }
      }
      if (flags->PrintNonPrintable) {
        if (symbol < 32 && symbol != 9 && symbol != 10) {
          symbol = symbol + 64;
          printf("^");
        } else if (symbol == 127) {
          symbol = symbol - 64;
          printf("^");
        }
      }
      printf("%c", symbol);
      last_sym = symbol;
    }
    fclose(filename);
  } else
    fprintf(stderr, "%s: %s: No such file or directory", argv[0], argv[optind]);
}

void numbernonblank(char symbol, Param *param) {
  if (symbol == '\n' && param->newLine == 1 && param->count_string == 0) {
    param->newLine = 0;
    param->blank_flag = 0;
  }
  if (symbol != '\n') {
    if (param->blank_flag == 0) {
      printf("%6d\t", ++param->count_string);
      param->blank_flag = 1;
    }
  } else {
    param->blank_flag = 0;
  }
}

void number(char symbol, Param *param) {
  if (param->newLine == 1) {
    param->newLine = 0;
    printf("%6d\t", param->All++);
  }
  if (param->All == 1) {
    printf("%6d\t", param->All);
    param->All++;
  }
  if (symbol == '\n' && param->All != 0) {
    param->newLine = 1;
  }
}