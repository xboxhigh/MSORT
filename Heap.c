#include "Heap.h"

unsigned		int nHeapSize;
unsigned int	nHostUsed;
SPPT			**paHeapHost;

void HeapFree()
{
	if( paHeapHost )
		free( paHeapHost);
	nHeapSize=0;
	paHeapHost = NULL;
	nHostUsed=0;
}

int IsEmpty()
{
	return (nHostUsed<=0);
}

int HeapCreate(unsigned int nHSize)
{
	nHeapSize=nHSize;
	paHeapHost = (SPPT **) malloc((nHeapSize+1)*sizeof(SPPT *));
	nHostUsed=0;
	return 1;
}

int HeapPush(SPPT *pNode)
{
	if( nHostUsed >= nHeapSize )
		return 0;
    nHostUsed++;
    paHeapHost[nHostUsed]=pNode;
    UpHeap();
    return 1;
}

void UpHeap()
{
    SPPT *pPivot;
    int k;

    k=nHostUsed;
    pPivot=paHeapHost[k];
    while(k>1)
    {
    	if( strcmp( (paHeapHost[k/2])->pszLine, pPivot->pszLine) < 0 )
            break;
        paHeapHost[k]=paHeapHost[k/2];
        k=k/2;
    }
    paHeapHost[k]=pPivot;
}

SPPT *HeapPop()
{
	SPPT *pPivot;
	if( nHostUsed <= 0 )
		return NULL;
	pPivot=paHeapHost[1];
	paHeapHost[1]=paHeapHost[nHostUsed];
    nHostUsed--;
    DownHeap();
    return pPivot;
}

void DownHeap()
{
    unsigned int j,k;
	SPPT *pPivot;

    k=1;
    pPivot=paHeapHost[1];
    while(k<= (nHostUsed/2) )
    {
        j=2*k;
    	if( j<nHostUsed && strcmp( (paHeapHost[j])->pszLine, (paHeapHost[j+1])->pszLine ) > 0)
			j++;
        if( strcmp(pPivot->pszLine, (paHeapHost[j])->pszLine)<0)
            break;
        paHeapHost[k]=paHeapHost[j];
        k=j;
    }
    paHeapHost[k]=pPivot;
}
