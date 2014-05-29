#ifndef H_PSORT
#define H_PSORT

#ifndef true
#define true	1
#endif

#ifndef false
#define false	0
#endif

#include <sys/types.h>
#include <stdio.h>

#define WBUF_SIZE		1024*1024
#define FNAME_SIZE		40
#define FNAME_PREFIX	".MsOrt."
#define SAMPLE_PNUM		12

enum MSORT_ERROR { MSORT_OK, MSORT_MALLOC, MSORT_FILE_OPEN, MSORT_FILE_READ };

int msort( const char **pszaFNames, const u_int uiFNum, char *ptrBuf, const uint uiBSize,  FILE *fpOut, pid_t iPID);

extern int msort_error_no;

#endif