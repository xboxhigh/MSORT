#ifndef H_RECORD_SW
#define H_RECORD_SW

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include "BMPM.h"

#define GR_FALSE			0
#define GR_OK				1
#define GR_ERROR			-1
#define GR_TRUE				1
#define	MAX_FNAME_SIZE		2
#define MAX_FIELDS			101
#define MAX_FIELD_LEN		20
#define FIELD_DELIMITER		'@'
#define FIELD_END			':'
#define GREC_DELIMITER		"@\n@U:http://"
#define GREC_P_DELIMITER	"@\n"
#define	MAX_GREC_SIZE		4560000
#define GREC_FILE_BUF		MAX_GREC_SIZE*2
#define MAX_FTEXT_SIZE		2050000 //1049654 MAX (Define bigger for save
#define MAX_FTEXT			1024*1024
#define DEF_FIELDS_NO		33
#define DEF_FIELDS			{"U", "P", "H", "Y", "F", "y", "l", "L", "s", "t", "T", "k", "d", "B", "cs", "ps", "et", "tt", "lt", "as", "il", "dl", "sl", "xl", "p", "g", "YT", "YD", "Yk", "Yt", "YL", "R", "A"}

enum GR_ErrorCode { GR_ER_OK, GR_ER_Open, GR_ER_Malloc, GR_ER_Read, GR_ER_UNZIP, GR_ER_ZIP, GR_ER_SIZE, GR_ER_DATA };

typedef struct sGRec
{
	char		*cpRecord;// Record Pointer
	int			iRecSize; // Record Size
	long long	oOffset;  // The offset of this record in the file
	char		*cpaFields[MAX_FIELDS]; // Fileds
} SGRec;

typedef struct sMRProfile
{
	SBMPM		*g_GR_BMInfo;
	int			g_GR_FieldNo;
	char		g_GR_PDelimiter[1024];
	int			g_GR_FieldOrd[MAX_FIELDS];
	char		*g_GR_Fields[MAX_FIELDS];
	char		*g_GR_GRecBuf;
	char		*g_GR_GTextBuf, *g_GR_GTextBuf2;
	int			g_GR_FD;
	int			g_GR_BufIn;
	int			g_GR_BufPos;
	int			g_GR_FileEnd;
	int			g_GR_MaxTexSize;
	int			g_GR_FBufSize;
	int			g_GR_MaxRecSize;
	long long	g_GR_TtlSize;
	long long	g_GR_FSize;
	char		g_GR_pszFieldName[MAX_FIELDS*(MAX_FIELD_LEN+1)];
} SMRProfile;

extern	long long	g_GR_TtlSize; // Total Size read after init.
extern 	long long	g_GR_FSize; // Total Size read of this file.
extern	int			g_GR_ErrorCode; // Error code.

int GRecFBN(char *pszFName);
int GRecInit(char *pszDelimiter);
int GRecSetFile(const char *pszFileName);
int GRecSetFP(FILE *fp);
void GRecCloseFile();
int GRecGet(SGRec *spGRec);
int GRecTDGet(SGRec *spGRec);
int GRecParse(SGRec *spGRec);
void GRecOutput(SGRec *spGRec, FILE *FP);
char *GRecUnzipText( char *cpText );
char *GRecZipText( char *cpText );
char *GRecZT( char *cpText, int *iLen);
int GRecSetOrd(const char *pszOrdString);
int GRecSetType(const char *pszTypeString);
int GRecSetTextSize( const int iSize);
int StrCmp(const void *x, const void *y);
int GRecSetFields(const char **pszaFields, int iFields);
void GRecSetPDelimiter(char *pszPDelimiter);
void StrTrans(char *pszStr);
void GRecFree();


// Multi Record Profile Functions
int mGRecFBN(char *pszFName, SMRProfile *spMRP);
SMRProfile *mGRecInit(char *pszDelimiter);
void mGRecSetPDelimiter(char *pszPDelimiter, SMRProfile *spMRP);
int mGRecSetFields(const char **pszaFields, int iFields, SMRProfile *spMRP);
int mGRecSetFile(const char *pszFileName, SMRProfile *spMRP);
int mGRecSetFP(FILE *fp, SMRProfile *spMRP);
void mGRecCloseFile(SMRProfile *spMRP);
int mGRecGet(SGRec *spGRec, SMRProfile *spMRP);
int mGRecTDGet(SGRec *spGRec, SMRProfile *spMRP);
int mGRecParse(SGRec *spGRec, SMRProfile *spMRP);
void mGRecOutput(SGRec *spGRec, FILE *FP, SMRProfile *spMRP);
char *mGRecUnzipText( char *cpText, SMRProfile *spMRP);
char *mGRecZipText( char *cpText, SMRProfile *spMRP );
int mGRecSetTextSize( const int iSize, SMRProfile *spMRP);
void mGRecFree(SMRProfile *spMRP);


#endif