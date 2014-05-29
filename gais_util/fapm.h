#ifndef H_FAPM
#define H_FAPM

#include	<sys/types.h>
#include	"amember.h"
#define		MAX_MATCH_NUM	4096

typedef struct sFAPM
{
	u_char 		*pszPats;
	u_char		**pszaPArray;
	uint		uiPNum;
	uint		uiPat;
	uint		uiErrors;
	AMemberM	sAPM;
	int			iaMid[MAX_MATCH_NUM];
	u_char		**pszaMPat[MAX_MATCH_NUM];
}SFAPM;

SFAPM	*InitFAPM( const char *pszFName, uint uiErrors);
int ExecuteFAPM( SFAPM *spFAPM, u_char *pszPat);
int FAPM(SFAPM *spFAPM, int iError, u_char *pszPat);
#endif