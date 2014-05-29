#ifndef H_PARSERECORD
#define H_PARSERECORD

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>				// For Zip
#include "FileUtil.h"

#define MAX_RECORD_SIZE			2*1024*1024
#define MIN_MEM_SIZE			10*1024*1024
#define REC_DELIMITER			"@\n@U:http://"
#define LEN_REC_DELIMITER		12

typedef struct sRawRec
{
	char	*cpRecStart;
	int		iRecSize;
}SRawRecord;

typedef struct sRec
{
	char	*czURL;
	char	*czTitle;
	char	*czBody;
	char	*czAliasName;
	char	*czHost;
	char	*czLanguage;
	int		iRecSize;
	int		iBodySize;
	int		iSize;
	int		iRCount;
	int		iSiteRCount;
	int		iPageScore;
	int		iPID;
}SWebRecord;


int InitParseRecord(int iSize);
int	EndParseRecord();
int SetRecordFile(const char *czFileName);
void CloseRecordFile();
int GetRecord(SWebRecord *spRecord, int iUnZipped);
int ParseField(SRawRecord *spRawRec, SWebRecord *spRecord, int iUnZipped);
int ParseRawRecord(SRawRecord *spRecord);
#endif