#ifndef CATFLAGS_H
#define CATFLAGS_H
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int NumberNonBlank;
  int SymbolEnd;
  int ALLnumber;
  int Squeezeblank;
  int Tab;
  int PrintNonPrintable;
} Flags;

typedef struct {
  int count_string;
  int count_string_s;
  int newLine;
  int blank_flag;
  int All;
} Param;

void parser_cat(int argc, char **argv, Flags *flags);
void print_cat_flags(char **argv, Flags *flags);
void numbernonblank(char symbol, Param *param);
void number(char symbol, Param *param);

#endif