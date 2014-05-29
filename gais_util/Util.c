#include "Util.h"
#include "Log.h"
#include <ctype.h>

int		g_iStrrstrBigLen;

inline char *strrstr(const char *big, const char *little)
{
	int		len_little,len_big;
	
	for( len_little=0; ;len_little++ )
	{
		if( little[len_little]=='\0' )
			break;
		if( big[len_little]=='\0' )
		{
			g_iStrrstrBigLen=len_little;
			return NULL;
		}
	}
	for( len_big=len_little;;len_big++ )
		if( big[len_big]=='\0' )
			break;
	g_iStrrstrBigLen=len_big;
	for( len_big-=len_little ; len_big >= 0; len_big -- )
	{
		if( !strncmp( big+len_big, little, len_little) )
			return (char *)(big+len_big);
	}
	return NULL;
}

inline char *strrstr2(const char *big, const char *little,int len_little)
{
	int		len_big;
	
	for( len_big=0;;len_big++ )
		if( big[len_big]=='\0' )
			break;
	g_iStrrstrBigLen=len_big;
	if( len_big < len_little )
		return NULL;
	for( len_big-=len_little ; len_big >= 0; len_big -- )
	{
		if( !strncmp( big+len_big, little, len_little) )
			return (char *)(big+len_big);
	}
	return NULL;
}


inline char *strrstr3(const char *big,int len_big,const char *little,int len_little)
{
	for( len_big-=len_little ; len_big >= 0; len_big -- )
	{
		if( !strncmp( big+len_big, little, len_little) )
			return (char *)(big+len_big);
	}
	return NULL;
}


int isPrime (long n)
{
    long    upperBound, i;

    upperBound = (long)pow(n, 0.5);

    if ((n % 2) == 0) {return false;}

    for (i = 3; i < upperBound; i += 2) {
        if ((n % i) == 0) {return false;}
    }

    return true;
}

long CalculatePrime (long lTableSize)
{
    while (isPrime(lTableSize) == false) 
    {
        lTableSize += 1;
    }
    return lTableSize;
}

inline void ToLower(char *Str)
{
	int i;
	
    for( i=0;Str[i]!='\0';i++ )
		Str[i]=tolower(Str[i]);
}

inline int CheckURLSite(char *cpURL)
{
	int  i;

   /* 先找 : 位置 */
	for (i = 0; cpURL[i]!='\0'; i++)
		if (cpURL[i] == ':')
			break;
	if( cpURL[i]=='\0' )
		return false;
   /* skip '/' 位置 */
	for (i = i+1 ; cpURL[i]!='\0'; i++)
		if (cpURL[i] != '/')
			break;
	if( cpURL[i]=='\0' )
		return false;
   /* 找下一個 '/' 位置 */
	for (i = i + 1; cpURL[i]!='\0'&& cpURL[i] != '/'; i++){};
	if( cpURL[i]=='\0' || (cpURL[i] == '/'&&cpURL[i+1]=='\0') || ( cpURL[i] == '/'&&(!strncasecmp( &(cpURL[i+1]),"index.",6)||!strncasecmp( &(cpURL[i+1]),"main.",5)) ) )
		return true;
	else
		return false;
}
