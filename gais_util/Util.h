#ifndef H_UTIL
#define H_UTIL

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <math.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#ifndef false
#define false	0
#endif
#ifndef true
#define true	1
#endif

#define Max(a, b) ((a>b)?a:b)
#define Min(a, b) ((a<b)?a:b)

long CalculatePrime (long lTableSize);
char *strrstr(const char *big, const char *little);
char *strrstr2(const char *big, const char *little,int len_little);
char *strrstr3(const char *big,int len_big,const char *little,int len_little);
void ToLower(char *Str);
extern int	g_iStrrstrBigLen;
#endif
