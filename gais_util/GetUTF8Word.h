#ifndef H_GETUTF8WORD
#define H_GETUTF8WORD

#define UTF8OTHER	1
#define UTF8ONE		0
#define UTF8TWO		2
#define UTF8THREE	3
#define UTF8FOUR	4

#ifndef ALPHABET_SIZE
#define ALPHABET_SIZE	256
#endif

void initGetUTF8Word();
int GetUTF8Word(char *pszTarget, const char *pszSource);
int GetUTF8WordLen(const char *pszSource);

#endif