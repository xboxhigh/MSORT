#ifndef H_HEAP
#define H_HEAP

#include <stdlib.h>
#include "MFile.h"

typedef struct sPPT
{
	MFILE *mfp;
	char  *pszLine;
//	uint  uiLen;
}SPPT;

void			DownHeap();
void			UpHeap();
int 			HeapCreate(unsigned int nHSize);
int 			HeapPush(SPPT *pNode);
SPPT			*HeapPop();
int				IsEmpty();
#endif
