#ifndef H_FILEUTIL
#define H_FILEUTIL

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <math.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#define	ALL_DIRECTORY_LEVEL		-1

#ifndef false
#define false	0
#endif
#ifndef true
#define true	1
#endif


enum {otPIPE, otSTDOUT, otFILE};

int FileRead(uint iRemain, uint iMaxRead, void *vpPBuffer, void *vpBuffer, int fdFile);
int fpFileRead(uint iRemain, uint iMaxRead, void *vpPBuffer, void *vpBuffer, FILE *fpFile);
int TraverseDir(const char *cpEntryPoint,int (*funcHandler)(const char *,const char *,const char *), int nLevel);
int TraverseFileList(const char *cpEntryPoint,int (*funcHandler)(const char *,const char *,const char *), int nLevel);
int IsDir( char *);
inline int IsExecute( char *pszFName );
int IsFile( char *FName);
off_t FileSize( char *FName);
FILE *ResultOpen(const char *czFName, const char *czMode);
void ResultClose(FILE *fpResult);
int FileLines( char *pszFN );
extern int	g_iFileEndFlag;
#endif