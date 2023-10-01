#ifndef GREPFLAGS_H
#define GREPFLAGS_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 8192

char *shortopt = "e:ivclnhsf:o";

struct option LongOtions[] = {{"regexp", required_argument, NULL, 'e'},
                              {"ignore", 0, NULL, 'i'},
                              {"invert", 0, NULL, 'v'},
                              {"count", 0, NULL, 'c'},
                              {"files-with-matches", 0, NULL, 'l'},
                              {"line-number", 0, NULL, 'n'},
                              {"no-filename", 0, NULL, 'h'},
                              {"silent", 0, NULL, 's'},
                              {"file", required_argument, NULL, 'f'},
                              {"only-matching", 0, NULL, 'o'},
                              {NULL, 0, NULL, 0}};

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} Flags;

typedef struct {
  int open_result;
  int line_count;
  int match_line_count;
  int match_flag;
} Param;
void parser_grep(int argc, char **argv, Flags *flags, char *pattern);
void grep_flag_print(char **argv, Flags *flags, char *text, int file_counter,
                     char *pattern, int optarg_count, Param *param);
void flags_e_f(Flags *flags, char **argv, char *buffer_f, char *pattern);
void job(char *text, Flags *flags, Param *param, char **argv, int optarg_count,
         int file_counter, regex_t regex, regmatch_t *match, int flag_v);

#endif
