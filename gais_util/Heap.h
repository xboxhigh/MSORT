#ifndef H_HEAP
#define H_HEAP

#include <stdlib.h>

void			DownHeap();
void			UpHeap();
int 			HeapCreate(unsigned int nHSize,unsigned int nNSize ,int (*cFunc)(void *x,void *y));
int 			HeapPush(void *pNode);
int				HeapPop(void *pNode);
int				IsEmpty();
#endif
