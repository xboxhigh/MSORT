#ifndef H_NODEGEN
#define H_NODEGEN
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NODE_INITIAL		1
#define	NODE_NONINITIAL		0
#define NO_MAX_NODE_LIMIATION	0

typedef struct sHookNode
{
	struct sHookNode 	*spNext;
	void				*czContent;
}SHookNode;

typedef struct sNodeGen
{
        unsigned int    uiFreeNode;
        void   			*vpFreeList;
        unsigned int    uiNodeSize;
        unsigned int    uiNodePerAllocate;
        unsigned int    uiMaxNodes,uiAllocated;
        short			sInit;
        SHookNode		*spHook;
}SNodeGen;

SNodeGen	*CreateNodeGen(unsigned int uiNSize,short sInitial,unsigned int uiNPAllocate,unsigned int uiMNode);
int 		Allocate(SNodeGen *spNodeGen);
void		*GetNode(SNodeGen *spNodeGen);
void		FreeNode(void *vpNode, SNodeGen *spNodeGen);
void		FreeNodeGen(SNodeGen *spNodeGen);
void		ResetNodeGen(SNodeGen *spNodeGen);
int			NodeGenUsed(SNodeGen *spNodeGen);

#endif
