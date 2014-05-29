#include "Main.h"
#include <stdio.h>
#include <unistd.h>			// For getopt()
#include <stdlib.h>			// For atoi()
#include <string.h>			// For strtok()
#include <ctype.h>
#include "FileUtil.h"
#include "Global.h"
#include "MSort.h"
#include "TimeUtil.h"


u_int		g_uiFNum, g_uiFNSize, g_uiCFNum;
u_int		g_uiMSize;
char		*g_pszOFName;
char		*g_ptrFNBuf, **g_ptraFNames;
char		*g_ptrBuf;

void Usage(char *czAName)
{
	fprintf( stderr, "%s [OPTIONS] Directories\n",czAName);
	fprintf( stderr, "\t-o Output file name.\n",czAName);
	fprintf( stderr, "\t-m Memory size.\n",czAName);
	fprintf( stderr, "\t-h <This page>\n",czAName);
	exit(1);
}

int main(int iArgc, char **cpArgv)
{
	char	cOption;
	int		i;
	FILE	*fpOut;

	TU_Init(10);
	while( (cOption=getopt(iArgc,cpArgv,"hp:o:m:")) != -1 )
	{
		switch( cOption )
		{
			case 'm':
				g_uiMSize = atoi( optarg );
				break;
			case 'o':
				g_pszOFName = optarg ;
				break;
			case 'h':
			default:
				Usage(cpArgv[0]);
				break;
		}
	}
	
	if( g_uiMSize < MIN_MEM_SIZE )
		g_uiMSize = MIN_MEM_SIZE;
	g_uiMSize=g_uiMSize*1024*1024;
	
	g_uiFNum=0;g_uiFNSize=0;
	for( i = optind; i < iArgc; i++)
		TraverseDir(cpArgv[i], calculateFSize, ALL_DIRECTORY_LEVEL);

	g_ptrFNBuf=(char *)malloc(g_uiFNSize * sizeof(char));
	g_ptraFNames=(char **)malloc(g_uiFNum * sizeof(char *));
	g_ptrBuf=(char *)malloc( g_uiMSize * sizeof(char) );
	if( g_ptrFNBuf == NULL || g_ptraFNames == NULL || g_ptrBuf == NULL )
	{
		fprintf( stdout, "Error while malloc.(%d, %d, %d)\n", g_uiFNSize * sizeof(char),  g_uiFNum * sizeof(char *), g_uiMSize * sizeof(g_uiMSize) );
		exit(1);
	}
	g_uiCFNum=0;
	for( i = optind; i < iArgc; i++)
		TraverseDir(cpArgv[i],processFile,ALL_DIRECTORY_LEVEL);
	if( g_pszOFName == NULL )
		fpOut=stdout;
	else
	{
		if( (fpOut=fopen( g_pszOFName, "w" )) == NULL )
		{
			fprintf( stderr, "Output file open error(%s).\n", g_pszOFName );
			exit(1);
		}
	}
	TU_Set(0);
	msort( (const char **)g_ptraFNames, (const uint)g_uiFNum, g_ptrBuf, g_uiMSize, fpOut , getpid());
	TU_Interval("Total Time", 0, 0);
	return 1;
}

int processFile(const char *czPath, const char *czFName, const char *czName)
{
	int		iLen;
	
	iLen=strlen(czName) + 1;
	g_ptraFNames[g_uiCFNum]=g_ptrFNBuf;
	memcpy( g_ptrFNBuf, czName, iLen );
	g_ptrFNBuf+=iLen;
	g_uiCFNum++;
	return true;
}

int calculateFSize(const char *czPath, const char *czFName, const char *czName)
{
	g_uiFNSize+=strlen( czName ) + 1;
	g_uiFNum++;
	return true;
}
