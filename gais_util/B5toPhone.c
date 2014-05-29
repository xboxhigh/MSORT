#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
//#include <md5.h>
#include <string.h>
#include "B5toPhone.h"
#include "phonetic.h"
B5_hash_t HashTable[HASH_TABLE_SIZE];

/*=================================================================================================
  Purpose: �Q�� MD5 �p��@�Ӥ���r�� Hash Value
       In: term -- �@�Ӥ���r
      Out: Hash Value
=================================================================================================*/
int hash(u_char *term)
{
	MD5_CTX context;
	u_char digest[16];
	u_int len;
	u_int hash_value;
	int i;
	
	len = strlen(term);
	
	MD5Init(&context);
	MD5Update(&context, term, len);
	MD5Final(digest, &context);
	
	hash_value = 0;
	for (i = 0; i < 4; i ++) {
	        hash_value <<= 8;
	        hash_value += digest[i];
	}
	hash_value = hash_value % HASH_TABLE_SIZE;
	
	return (int)hash_value;
}


/*=================================================================================================
  Purpose: ����r�� Phonetic ����l�ơA�D�n�b�إߤ���r�P�o���ഫ�� HashTable�A
       In: HashTable -- HashTable����}�A�γ~������r�b�s�X������m
      Out: 
=================================================================================================*/
void B5toPhonetic_Init() {
	int i, j;
	unsigned char *data, word[3];
	int HashValue;
	B5_hash_t *newnode;
	
	for ( i=0; i<HASH_TABLE_SIZE; i++ ) {
		HashTable[i].idx = -1 ;
		HashTable[i].next = NULL ;
	}	
	
	word[2] = '\0';
	for ( i = 0; PhoneticTable[i][0] != 0; i++){	// i is index 
		if ( PhoneticTable[i][0] != (u_char *)"" ) {
			data = PhoneticTable[i][0];
			for (j=0; data[j] != '\0'; j=j+2) {	// Index all word with same phonetic 
				memcpy(word, data+j, 2);
				HashValue =  hash(word);
				
				if ( HashTable[HashValue].idx == -1 )
					HashTable[HashValue].idx = i;
				else {
					newnode = (struct B5_hash *)malloc( sizeof(struct B5_hash) );
					newnode->idx = i;
					newnode->next = HashTable[HashValue].next;
					HashTable[HashValue].next = newnode;
				}	
			}
		}
		
	}
}

/*=================================================================================================
  Purpose: ����r�� Phonetic 
       In: data -- �ҭn��X����r
       	   HashTable -- �إߦn�� HashTable
      Out: ������r�o�����s�X
=================================================================================================*/
u_char *B5toPhone(u_char *data) {
	int HashValue;
	int index;
	B5_hash_t *tmpnode;
	
	HashValue =  hash(data);
	
	if ( HashTable[HashValue].idx == -1 )
		return data;
	else {
		tmpnode = &HashTable[HashValue];
		index = tmpnode->idx;
		while ( index != -1 ) {
			if ( strstr(PhoneticTable[index][0], data) != NULL )
				return PhoneticTable[index][1];
			else {
				if (tmpnode->next != NULL)
					tmpnode = tmpnode->next;
				else
					return data;
			}
			index = tmpnode->idx;
		}
		return NULL;
	}
}


/*=================================================================================================
  Purpose: ����r�� Phonetic �������ʧ@�A�D�n�b Free �ҫإߪ� HashTable
       In: HashTable -- �إߦn�� HashTable
      Out: 
=================================================================================================*/
void B5toPhonetic_Final() {
	int i;
	B5_hash_t *tmpnode, *current;
	
	for ( i=0; i<HASH_TABLE_SIZE; i++ ) {
		if (HashTable[i].next != NULL) {
			current = HashTable[i].next;
			while ( current->next != NULL ) {
				tmpnode = current->next;
				free(current);
				current = tmpnode;	
			}
		}
	}	
	
}


/*=================================================================================================
  Purpose: �N����r���ন�o�����s�X
       In: B5text -- �n��X������r��
       	   Phonetic -- ��X�᪺���G
      Out: ��X�᪺�r�����
=================================================================================================*/
int _B5toPhonetic(u_char *B5text, u_char *Phonetic) {
	int j;

	u_char word[3];		// �x�s�@�Ӥ���r
	unsigned char *phonetic;
	
	word[2] = '\0';
	strcpy(Phonetic, "");
	for (j = 0; j < strlen(B5text); ) {
		if ( (B5text[j] & 0x80) == 0x80 ) {	// �P�_�O�_������r
			memcpy(word, B5text+j, 2);
			phonetic = B5toPhone(word);	// �N����r�ഫ���o�����s�X
			sprintf (Phonetic, "%s%s", Phonetic, phonetic);
			j = j + 2;
		}
		else {		// ���O����r�A���� Output
			sprintf (Phonetic, "%s%c", Phonetic, B5text[j]);
			j++;
		}
	}	
	return strlen(Phonetic);
}

int B5toPhonetic(u_char *B5text, u_char *Phonetic) 
{
	int	len;
	
	B5toPhonetic_Init();	// �ھڵo���s�X��A�إߤ���r�P�o�����ഫ (Hash Table)
	len=_B5toPhonetic( B5text, Phonetic );
	B5toPhonetic_Final();
	return len;
}
