#ifndef H_TERMOFFSET
#define H_TERMOFFSET

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define HIDX_ERR			-1
#define HIDX_NONE			0
#define HIDX_OK				1
#define HASH_INFO_NAME		".HashInfo"
#define FINAL_INDEX_PREFIX	".GaisIndex"
#define BUFFER_SIZE			32*1024

typedef struct sHashInfo
{
	uint		uiEnd;
	uint		uiHashSize;
}SHashInfo;

typedef struct sHidxData
{
	char	*pszTerm;
	int		iFid;
	int		iMatchCnt;
	off_t	oOffset;	
}SHidxData;


int GetTermInfo(char **pszaIdxDirs, int IdxDirNo, SHidxData *spHidxData);
#endif