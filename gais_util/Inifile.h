#ifndef INIFILE
#define INIFILE

#include <stdio.h>
#include <fcntl.h>

#define MAX_INI_FIELD		256

int ReadIniFile(const char *czFileName);
char *GetIniValue(const char *czKey);
typedef struct sIniNide
{
	char	*cpKey;
	char	*cpValue;
} SIniNode;
#endif