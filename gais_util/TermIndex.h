#ifndef H_TERMINDEX
#define H_TERMINDEX

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define BUCKET_SIZE		2500

typedef struct sHIdxNode
{
	char			*czTerm;
	uint			uiOffset;
	uint			uiHashValue;
}SHIdxNode;

typedef struct sRTHINode
{
	char			*czTerm;
	uint			uiDF;
	uint			uiOffset;
	uint			uiHashValue;
}SRTHINode;


typedef struct sHashInfo
{
	uint		uiEnd;
	uint		uiHashSize;
}SHashInfo;

int CreateHashIndex(SHIdxNode *spHIN, int iHidxSize, int iBucketNo, char *pszFileName);
SHashInfo *LoadHashInfo(char *pszFileName);
#endif
