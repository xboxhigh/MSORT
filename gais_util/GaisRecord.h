#ifndef H_GAISRECORD
#define H_GAISRECORD

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>

#define GR_FALSE			0
#define GR_OK				1
#define GR_ERROR			-1
#define GR_TRUE				1
#define FIELD_NUMBER		76
#define FIELD_DELIMITER		'@'
#define FIELD_END			':'
#define GREC_DELIMITER		"@GaiSRecorD\n"
#define	MAX_GREC_SIZE		2560000
#define GREC_FILE_BUF		MAX_GREC_SIZE*2
#define MAX_FTEXT_SIZE		1050000 //1049654 MAX (Define bigger for save
#define MAX_FTEXT			1024*1024
						//	"/0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz"
#define DEF_FIELD_TYPE		"UUUUUUUUUUUUUUUUUUTTCCCCCCCCCCCCCCCCCCCCCCCCUUUUUUCCCCCCCCCCCCCCCCCCCCCCCCCC"
#define DEF_FIELD_ORDER		"UHPLTtskdCVEBmuchoNjJbvzOIrfFQKZXqSeGgainxwpDYyWMlRA"

#define GRecFBN(x) (( x>'z' )?0:( (x<'0')?0:(x-'/')))

enum GR_ErrorCode { GR_ER_OK, GR_ER_Open, GR_ER_Malloc, GR_ER_Read, GR_ER_UNZIP, GR_ER_ZIP, GR_ER_SIZE, GR_ER_DATA };

typedef struct sGRec
{
	char		*cpRecord;// Record Pointer
	int			iRecSize; // Record Size
	long long	oOffset;  // The offset of this record in the file
	char		*cpaFields[FIELD_NUMBER]; // Fileds
} SGRec;

extern	long long	g_GR_TtlSize; // Total Size read after init.
extern 	long long	g_GR_FSize; // Total Size read of this file.
extern	int			g_GR_ErrorCode; // Error code.

int GRecInit();
int GRecSetFile(const char *pszFileName);
void GRecCloseFile();
int GRecGet(SGRec *spGRec);
int GRecParse(SGRec *spGRec);
void GRecOutput(SGRec *spGRec, FILE *FP);
char *GRecUnzipText( char *cpText );
char *GRecZipText( char *cpText );
int GRecSetOrd(const char *pszOrdString);
int GRecSetType(const char *pszTypeString);
int GRecSetTextSize( const int iSize);
#endif