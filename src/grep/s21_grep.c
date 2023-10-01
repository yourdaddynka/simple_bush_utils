#include "s21_grep.h"

int main(int argc, char *argv[]) {
  Flags flags = {0};
  Param param = {0};
  char text[BUFFER] = {0};
  char pattern[BUFFER] = {0};
  parser_grep(argc, argv, &flags, pattern);
  if (argc >= 3) {
    int file_counter = argc - optind;
    int optarg_count = optind;
    while (optarg_count < argc) {
      grep_flag_print(argv, &flags, text, file_counter, pattern, optarg_count,
                      &param);
      optarg_count++;
    }
  }
  return 0;
}
void parser_grep(int argc, char **argv, Flags *flags, char *pattern) {
  int current_flag = 0;
  char buffer_f[BUFFER] = {0};
  while ((current_flag = getopt_long(argc, argv, shortopt, LongOtions, NULL)) !=
         -1) {
    switch (current_flag) {
      case 'e':
        flags->e = 1;
        pattern += snprintf(pattern, BUFFER, "%s|", optarg);
        break;
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'f':
        flags->f = 1;
        snprintf(buffer_f, BUFFER, "%s", optarg);
        break;
      case 'o':
        flags->o = 1;
        break;
      case '?':
        fprintf(stderr, "unknown --directories option\n");
        current_flag = -1;
        break;
    }
  }
  flags_e_f(flags, argv, buffer_f, pattern);
  if (flags->l || flags->c) {
    flags->o = 0;
    flags->n = 0;
    flags->f = 0;
    flags->e = 0;
  }
}

void flags_e_f(Flags *flags, char **argv, char *buffer_f, char *pattern) {
  int i = 0;
  if (flags->e && !flags->f) {
    int a = strlen(pattern);
    strcat(pattern, "|");
    if (pattern[a - 1] == '|') pattern[a - 1] = '\0';
  }
  if (!flags->e && !flags->f) {
    snprintf(pattern, BUFFER, "%s", argv[optind]);
    optind += 1;
  }
  if (flags->f) {
    FILE *f_file;
    char symbol = '0';
    f_file = fopen(buffer_f, "r");
    if (f_file != NULL) {
      while ((symbol = fgetc(f_file)) != EOF) {
        if (symbol == 10 || symbol == 13) pattern[i++] = '|';
        if (symbol != 10 && symbol != 13) pattern[i++] = symbol;
      }
      pattern[i++] = '|';
      fclose(f_file);
    } else {
      if (flags->s == 0) {
        fprintf(stderr, "%s: %s: No such file or directory", argv[0], pattern);
      }
    }
  }
  if (flags->f && (pattern[i - 1] == '|')) pattern[i - 1] = '\0';
}
void grep_flag_print(char **argv, Flags *flags, char *text, int file_counter,
                     char *pattern, int optarg_count, Param *param) {
  int open_result = 0;
  FILE *filename = fopen(argv[optarg_count], "r");
  if (filename != NULL) {
    regex_t regex;
    regmatch_t match[1];
    int flag_v = flags->v ? 1 : 0;
    int reg_err =
        regcomp(&regex, pattern, (flags->i == 1 ? REG_ICASE : REG_EXTENDED));
    if (reg_err == 0) {
      while (fgets(text, BUFFER, filename) != NULL) {
        param->match_flag = regexec(&regex, text, 1, match, 0);
        if (strchr(text, '\n') == NULL) strcat(text, "\n");
        job(text, flags, param, argv, optarg_count, file_counter, regex, match,
            flag_v);
      }
      param->line_count = 0;
      if (flags->l || flags->c) {
        if (flags->c && !flags->l) {
          if (!flags->h && (file_counter > 1))
            printf("%s:", argv[optarg_count]);
          printf("%d\n", param->match_line_count);
        } else if (flags->l && !flags->c) {
          if (param->match_line_count > 0) printf("%s\n", argv[optarg_count]);
        } else if (flags->c && flags->l) {
          if (param->match_line_count > 0) {
            param->match_line_count = 1;
            if (!flags->h && (file_counter > 1))
              printf("%s:", argv[optarg_count]);
            printf("%d\n", param->match_line_count);
            printf("%s\n", argv[optarg_count]);
          } else if (param->match_line_count == 0) {
            if (!flags->h && (file_counter > 1))
              printf("%s:", argv[optarg_count]);
            printf("%d\n", param->match_line_count);
          }
        }
      }
    }
    param->match_line_count = 0;
    regfree(&regex);
    fclose(filename);
  } else {
    if (flags->s == 0) {
      fprintf(stderr, "%s: %s: No such file or directory", argv[0],
              argv[optarg_count]);
    }
  }
}

void job(char *text, Flags *flags, Param *param, char **argv, int optarg_count,
         int file_counter, regex_t regex, regmatch_t *match, int flag_v) {
  if (!flags->l && !flags->c) {
    if (file_counter > 1 && !flags->h && param->match_flag == flag_v)
      printf("%s:", argv[optarg_count]);
  }
  param->line_count++;
  if (param->match_flag == flag_v) {
    param->match_line_count++;
    if (!flags->l && !flags->c) {
      if (flags->n) printf("%d:", param->line_count);
      if (flags->o && !flags->l) {
        char *o_flags = text;
        for (; !param->match_flag && match[0].rm_eo != match[0].rm_so;) {
          printf("%.*s\n", (int)(match[0].rm_eo - match[0].rm_so),
                 o_flags + match[0].rm_so);
          o_flags += match[0].rm_eo;
          param->match_flag = regexec(&regex, o_flags, 1, match, 0);
        }
      }
      if (!flags->o && !flags->l) printf("%s", text);
    }
  }
}
