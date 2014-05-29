#ifndef H_HASH
#define H_HASH

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <md4.h>


typedef struct sHash
{
	void	**vpHashTable;
	int		iHashTableSize;
}SHash;

typedef struct sHasNode
{
	char			*czKey;
	struct sHasNode	*spNext;
} SHashNode;

SHash *CreateHash(int iHTSize);
int HashQuery( uint	uiHashValue, char *czCKey, SHash *spHash);
SHashNode* HashQuery2( uint	uiHashValue, char *czCKey, SHash *spHash);
void HashInsert( uint uiHashValue, SHashNode *spHNode, SHash *spHash);
void FreeHash(SHash *spHash);
void ResetHash(SHash *spHash);
uint StandardHashValue(void *vpKey);
uint StandardHashValue2(void *vpKey);
uint StandardHashValue3(unsigned char *cpKey);
uint StandardHashValue4(void *vpKey);
void asc2bin(char* masterkey, int size, char* digest);
#endif