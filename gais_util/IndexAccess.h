#ifndef H_INDEXACCESS
#define H_INDEXACCESS

#define MAX_INDEX_DIR	10

#ifndef ERROR
	#define	ERROR	-1
#endif
#ifndef	true
	#define	true	1
#endif
#ifndef	false
	#define	false	0
#endif
#define	MAX_FILE_NAME	80
#define INDEX_PREFIX	".GaisIndex"
#define	MAX_BUFFER_SIZE	4096

#include <sys/types.h>
#include "Codec.h"

int	indexAccessInit(u_char *IdxHome);
int	readindex(int fN, double offset, u_char *Ibuf, int MaxLen);
int	getTermInfo(u_char *term, int *Df, int *bCnt, int *fn, int *offset, int *begPID,int MinPid, int MaxPid);

#endif