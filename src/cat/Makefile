GC = gcc
CFL = -Wall -Werror -Wextra
BUILD = -o ../build
BUILD_FROM = -o ../../build
ALL = clang s21_cat
CLANG = clang-format
LEAKS = leaks -atExit -- 
GR_LS = | cat  LEAK:
CP_CHEK = cppcheck --enable=all --suppress=missingIncludeSystem 

all: rebuild s21_cat

clean: 
	rm -rf s21_cat

rebuild: clean s21_cat

s21_cat:
	$(GC) $(CFL)  s21_cat.c -o s21_cat

clang: 
	$(CLANG) -i s21_cat.c
	$(CLANG) -i s21_cat.h

cl_ch:
	$(CLANG) -n s21_cat.c
	$(CLANG) -n s21_cat.h