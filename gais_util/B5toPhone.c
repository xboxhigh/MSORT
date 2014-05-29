#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
//#include <md5.h>
#include <string.h>
#include "B5toPhone.h"
#include "phonetic.h"
B5_hash_t HashTable[HASH_TABLE_SIZE];

/*=================================================================================================
  Purpose: 利用 MD5 計算一個中文字的 Hash Value
       In: term -- 一個中文字
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
  Purpose: 中文字轉 Phonetic 的初始化，主要在建立中文字與發音轉換的 HashTable，
       In: HashTable -- HashTable的位址，用途為中文字在編碼表中的位置
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
  Purpose: 中文字轉 Phonetic 
       In: data -- 所要轉碼中文字
       	   HashTable -- 建立好的 HashTable
      Out: 此中文字發音的編碼
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
  Purpose: 中文字轉 Phonetic 的結束動作，主要在 Free 所建立的 HashTable
       In: HashTable -- 建立好的 HashTable
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
  Purpose: 將中文字串轉成發音的編碼
       In: B5text -- 要轉碼的中文字串
       	   Phonetic -- 轉碼後的結果
      Out: 轉碼後的字串長度
=================================================================================================*/
int _B5toPhonetic(u_char *B5text, u_char *Phonetic) {
	int j;

	u_char word[3];		// 儲存一個中文字
	unsigned char *phonetic;
	
	word[2] = '\0';
	strcpy(Phonetic, "");
	for (j = 0; j < strlen(B5text); ) {
		if ( (B5text[j] & 0x80) == 0x80 ) {	// 判斷是否為中文字
			memcpy(word, B5text+j, 2);
			phonetic = B5toPhone(word);	// 將中文字轉換成發音的編碼
			sprintf (Phonetic, "%s%s", Phonetic, phonetic);
			j = j + 2;
		}
		else {		// 不是中文字，直接 Output
			sprintf (Phonetic, "%s%c", Phonetic, B5text[j]);
			j++;
		}
	}	
	return strlen(Phonetic);
}

int B5toPhonetic(u_char *B5text, u_char *Phonetic) 
{
	int	len;
	
	B5toPhonetic_Init();	// 根據發音編碼表，建立中文字與發音的轉換 (Hash Table)
	len=_B5toPhonetic( B5text, Phonetic );
	B5toPhonetic_Final();
	return len;
}
