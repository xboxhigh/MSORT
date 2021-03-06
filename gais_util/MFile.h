#ifndef H_MFILE
#define H_MFILE

#define	MFILE_BUF_SIZE	16*1024

#include <sys/types.h>
#include <stdio.h>

#ifndef true
#define true	1
#endif

#ifndef false
#define false	0
#endif


enum MFILE_ERROR {MFILE_OK, MFILE_MALLOC, MFILE_FILE_NOT_FOUND, MFILE_READ, MFILE_LSEEK, MFILE_SIZE};

typedef struct sMFILE
{
	char		**pszFNames;
	uint		uiFiles;
	uint		uiCurFile;
	int			iFP;
	char		*ptrBuf;
	uint		uiBufSize;
	uint		uiInBuf;
	uint		uiBPos;
	char		cEOF;
	off_t		*llaSize;
}MFILE;

MFILE *mfopen(const char **pszFNames, const uint uiFiles);
MFILE *mfopen_m(const char **pszFNames, const uint uiFiles, char *pszBuf, uint uiMSize);
char *im_mfgets( uint *uiLen, int *iLRCF, MFILE *mfpFP);
int mfeof( MFILE *mfpFP );
void mfclose( MFILE *mfpFP );
void mrefopen(const char **pszFNames, const uint uiFiles, MFILE *mfpFP);
void mffree( MFILE *mfpFP );
off_t mfsize( MFILE *mfpFP);
int mfseek( MFILE *mfpFP, off_t llOffset );
void mfrewind( MFILE *mfpFP );

extern int mfile_error_no;

#endif