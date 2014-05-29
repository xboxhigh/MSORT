#include "TimeUtil.h"
#include <sys/time.h>
#include <stdlib.h>
 #include <string.h>

int		GTU_iTimes;
struct  timeval *GTU_spTValue;
FILE	*GTU_fpOut;


void TU_SetOutput( FILE *fp )
{
	GTU_fpOut=fp;
}

void TU_Init( int iNum )
{
	GTU_iTimes=iNum;
	if( GTU_spTValue )
		free( GTU_spTValue );
	GTU_spTValue=malloc( sizeof(struct  timeval) * iNum);
	bzero( GTU_spTValue, sizeof(struct  timeval) * iNum );
	GTU_fpOut=stdout;
}

int TU_Clear( int iNum )
{
	if( iNum < 0 || iNum >= GTU_iTimes || GTU_iTimes==0 )
		return 0;
	(GTU_spTValue[ iNum ]).tv_sec=0;
	(GTU_spTValue[ iNum ]).tv_usec=0;
	return 1;
}

int TU_ClearAll()
{
	int i;
	
	for( i=0; i<GTU_iTimes; i++ )
		TU_Clear( i );
	return 1;
}

int TU_Set( int iNum )
{
	if( iNum < 0 || iNum >= GTU_iTimes || GTU_iTimes==0 )
		return 0;
	gettimeofday( &(GTU_spTValue[ iNum ]), NULL );
	return 1;
}


int TU_Interval( char *pszString, int iNum, int iSet )
{
	struct  timeval ltv;
	long sec, usec;

	if( iNum < 0 || iNum >= GTU_iTimes || GTU_iTimes==0 )
		return 0;
	gettimeofday( &ltv, NULL );
	usec=ltv.tv_usec-(GTU_spTValue[ iNum ]).tv_usec;
	sec=ltv.tv_sec-(GTU_spTValue[ iNum ]).tv_sec;
	if( usec < 0 )
	{
		sec--;
		usec+=1000000;
	}
	fprintf( GTU_fpOut, "%s: %ld.%6.6ld seconds\n", pszString, sec, usec);
	if( iSet == 1 )
	{
		(GTU_spTValue[ iNum ]).tv_sec=ltv.tv_sec;
		(GTU_spTValue[ iNum ]).tv_usec=ltv.tv_usec;
	}
	return 1;
}

int TU_Add( char *pszString, int iNum, int iNum2, int iSet )
{
	struct  timeval ltv;
	long sec, usec;

	if( iNum < 0 || iNum >= GTU_iTimes || GTU_iTimes==0 )
		return 0;
	if( iNum2 < 0 || iNum2 >= GTU_iTimes )
		return 0;
	gettimeofday( &ltv, NULL );
	usec=ltv.tv_usec-(GTU_spTValue[ iNum ]).tv_usec;
	sec=ltv.tv_sec-(GTU_spTValue[ iNum ]).tv_sec;
	if( usec < 0 )
	{
		sec--;
		usec+=1000000;
	}
	if( pszString )
		fprintf( GTU_fpOut, "%s: %ld.%6.6ld seconds\n", pszString, sec, usec);
	if( iSet == 1 )
	{
		(GTU_spTValue[ iNum ]).tv_sec=ltv.tv_sec;
		(GTU_spTValue[ iNum ]).tv_usec=ltv.tv_usec;
	}
	(GTU_spTValue[ iNum2 ]).tv_sec+=sec;
	(GTU_spTValue[ iNum2 ]).tv_usec=usec;
	if( (GTU_spTValue[ iNum2 ]).tv_usec >= 1000000 )
	{
		(GTU_spTValue[ iNum2 ]).tv_sec+=1;
		(GTU_spTValue[ iNum2 ]).tv_usec-=1000000;
	}
	return 1;
}

int TU_Output( char *pszString, int iNum )
{
	if( iNum < 0 || iNum >= GTU_iTimes || GTU_iTimes==0 )
		return 0;
	fprintf( GTU_fpOut, "%s: %ld.%6.6ld seconds\n", pszString, (GTU_spTValue[ iNum ]).tv_sec, (GTU_spTValue[ iNum ]).tv_usec);
	return 1;
}