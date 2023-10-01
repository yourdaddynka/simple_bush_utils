GC = gcc
CFL = -Wall -Werror -Wextra
BUILD = -o ../build
BUILD_FROM = -o ../../build
ALL = clang s21_grep
CLANG = clang-format
LEAKS = leaks -atExit -- 
GR_LS = | grep  LEAK:
CP_CHEK = cppcheck --enable=all --suppress=missingIncludeSystem 

all: rebuild s21_grep

clean: 
	rm -rf s21_grep

rebuild: clean s21_grep

s21_cat:
	$(GC) $(CFL)  s21_grep.c -o s21_grep

clang: 
	$(CLANG) -i s21_grep.c
	$(CLANG) -i s21_grep.h

cl_ch:
	$(CLANG) -n s21_grep.c
	$(CLANG) -n s21_grep.h