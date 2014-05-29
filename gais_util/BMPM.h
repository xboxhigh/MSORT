#ifndef H_BMPM
#define H_BMPM
#define	ALPHABET_SIZE		256
#define	MAX_PATTERM_SIZE	128
#define MAX(a, b) ((a>b)?a:b)

#include <sys/types.h>

typedef struct sBMPM
{
	u_char	ucMatchPat[MAX_PATTERM_SIZE+1];
	u_short usLength;
	short	saBMBC[ALPHABET_SIZE];
	short	saBMGS[MAX_PATTERM_SIZE+1];
}SBMPM;

void BMPMPreProc(SBMPM *spBMInfo, u_char *ucPattern);
int BMPM( SBMPM *spBMInfo, u_char *y, int n);

#endif
