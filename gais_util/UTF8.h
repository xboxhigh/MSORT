#ifndef H_UTF8_CONVER
#define H_UTF8_CONVER

#define UTF8_UN         0
#define UTF8_B5         1
#define UTF8_GBK        2
#define UTF8_BOTH       3
#define UTF8_JP         4
#define UTF8_KR         5


int UTF82B5( unsigned char *pszStr, int iSize );
int UTF82GBK( unsigned char *pszStr, int iSize );
int UTF82JOHAB( unsigned char *pszStr, int iSize );
int UTF82SJIS( unsigned char *pszStr, int iSize );

int isUTF8(unsigned char *text, int fsize);
int UTF8Type( unsigned char *pszStr, int iSize);
void SetForceType(int iType);

#endif