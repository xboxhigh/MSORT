#include "MSort.h"
#include "Heap.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>


int		msort_error_no;
char	*g_ms_wBuf;

int ms_fillMem( char *ptrBuf, const uint uiBSize, MFILE *mfpFP )
{
	int		iLRCF, iRemain;
	uint	uiLen;
	char	*pszLine, *ptrEnd;
	char	**ptraLines;
	
	ptraLines=(char **)ptrBuf;
	ptrEnd=ptrBuf+uiBSize;
	while(  (iRemain=(ptrEnd - (char *)ptraLines)) >= sizeof( char *) )
	{
		if( (pszLine=im_mfgets( &uiLen, &iLRCF, mfpFP )) == NULL )
		{
			if( mfile_error_no == MFILE_OK )
				break;
			else
				return -mfile_error_no;
		}
		if( iLRCF )
			pszLine[ uiLen - 1] ='\0';
		else
		{
			pszLine[ uiLen] ='\0';
			uiLen++;
		}
		if( iRemain >= uiLen + sizeof( char *) )
		{
			ptrEnd-=uiLen;
			*ptraLines=ptrEnd;
			memcpy( ptrEnd, pszLine, uiLen );
			ptraLines++;
		}else
		{
			*ptraLines=pszLine;
			ptraLines++;
		}
	}
	return (ptraLines - (char **)ptrBuf );
}

int ms_strcmp( const void *a, const void *b)
{
	char *x, *y;
	
	x= *((char **)a);
	y= *((char **)b);
	return strcmp( x, y);
	
}

int ms_fbwrite( char **pszaLines, int iLines, FILE *fpOut )
{
	int			i, iLen;
	char		*ptrWBuf, *ptrEnd;
	
	if( g_ms_wBuf == NULL )
		g_ms_wBuf=(char *)malloc(WBUF_SIZE);

	if( g_ms_wBuf == NULL )
		return false;
	ptrWBuf=g_ms_wBuf;
	ptrEnd=ptrWBuf+WBUF_SIZE;
	for( i=0; i< iLines; i++ )
	{
		iLen=strlen( pszaLines[i] )+1;
		if( ptrWBuf + iLen > ptrEnd )
		{
			fwrite( g_ms_wBuf, 1, ptrWBuf - g_ms_wBuf, fpOut );
//			printf("w %d ms_fbwrite BW\n", ptrWBuf - g_ms_wBuf);
			ptrWBuf=g_ms_wBuf;
			if( iLen >= WBUF_SIZE )
			{
				pszaLines[i][iLen-1]='\n';
				fwrite( pszaLines[i], 1, iLen, fpOut );
//				printf("w %d ms_fbwrite LW\n", iLen);
				pszaLines[i][iLen-1]='\0';
				continue;
			}
		}
		pszaLines[i][iLen-1]='\n';
		memcpy( ptrWBuf, pszaLines[i], iLen );
		pszaLines[i][iLen-1]='\0';
		ptrWBuf+=iLen;
	}
	if( ptrWBuf - g_ms_wBuf > 0 )
	{
		fwrite( g_ms_wBuf, 1, ptrWBuf - g_ms_wBuf, fpOut );
//		printf("w %d ms_fbwrite LAST\n", ptrWBuf - g_ms_wBuf);
	}
	return true;
}

int ms_bwrite( char **pszaLines, int iLines, int iFD )
{
	int			i, iLen, iTWrite;
	char		*ptrWBuf, *ptrEnd;
	
	if( g_ms_wBuf == NULL )
		g_ms_wBuf=(char *)malloc(WBUF_SIZE);

	if( g_ms_wBuf == NULL )
		return false;
	iTWrite=0;
	ptrWBuf=g_ms_wBuf;
	ptrEnd=ptrWBuf+WBUF_SIZE;
	for( i=0; i< iLines; i++ )
	{
		iLen=strlen( pszaLines[i] )+1;
		if( ptrWBuf + iLen > ptrEnd )
		{
			iTWrite+=ptrWBuf - g_ms_wBuf;
			write( iFD, g_ms_wBuf, ptrWBuf - g_ms_wBuf);
//			printf("w %d ms_bwrite BW\n", ptrWBuf - g_ms_wBuf);
			ptrWBuf=g_ms_wBuf;
			if( iLen >= WBUF_SIZE )
			{
				pszaLines[i][iLen-1]='\n';
				iTWrite+=iLen;
				write( iFD, pszaLines[i], iLen );
//				printf("w %d ms_bwrite LW\n", iLen);
				pszaLines[i][iLen-1]='\0';
				continue;
			}
		}
		pszaLines[i][iLen-1]='\n';
		memcpy( ptrWBuf, pszaLines[i], iLen );
		pszaLines[i][iLen-1]='\0';
		ptrWBuf+=iLen;
	}
	if( ptrWBuf - g_ms_wBuf > 0 )
	{
		iTWrite+=ptrWBuf - g_ms_wBuf;
		write( iFD, g_ms_wBuf, ptrWBuf - g_ms_wBuf);
//		printf("w %d ms_bwrite LAST\n", ptrWBuf - g_ms_wBuf);
	}
	return iTWrite;
}

void ms_file_name( char *pszFName, pid_t iPID, int iNum , int iPart)
{
	sprintf( pszFName, "%s%5.5d.%4.4d.%4.4d", FNAME_PREFIX, iPID, iNum, iPart);
}

int ms_create_file( pid_t iPID, int iNum )
{
	char	pszFName[FNAME_SIZE];
	
	ms_file_name( pszFName, iPID, iNum, 0 );
	return open( pszFName, O_CREAT|O_WRONLY, 0644);
}

int ms_init_merge( SPPT *spPTT, int iPNum, char *ptrBuf, uint uiBSize,  pid_t iPID)
{
	char	*ptrFBuf, **pszaFN, *ptrPBuf;
	uint	uiBPSize;
	int		i;
	
	uiBPSize=uiBSize/iPNum;
	if( (ptrFBuf=(char *)malloc( iPNum * (sizeof(char *)+strlen(FNAME_PREFIX)+16) )) ==NULL )
		return false;
	ptrPBuf=ptrBuf;
	for( i=0; i< iPNum; i++ )
	{
		pszaFN=(char **)ptrFBuf;  ptrFBuf+=sizeof(char **);
		pszaFN[0]=ptrFBuf;
		ms_file_name( ptrFBuf, iPID, i, 0 );
		ptrFBuf+=strlen(ptrFBuf)+1;
		(spPTT[i]).mfp=mfopen_m((const char **)pszaFN, 1, ptrPBuf, uiBPSize);
		ptrPBuf+=uiBPSize;
		(spPTT[i]).pszLine=NULL;
	}
	return true;
}

int mfp_gets(SPPT *spPPT)
{
	int		iLRCF;
	uint	uiLen;

	if( (spPPT->pszLine=im_mfgets( &uiLen, &iLRCF, spPPT->mfp )) == NULL )
		return false;
	if( iLRCF )
		spPPT->pszLine[ uiLen - 1] ='\0';
	else
	{
		spPPT->pszLine[ uiLen] ='\0';
		uiLen++;
	}
	//spPPT->uiLen=uiLen;
	return true;
}

int pptcmp( void *a, void *b)
{
	SPPT *x, *y;
	
	x=(SPPT *)a;
	y=(SPPT *)b;
	
	return strcmp(x->pszLine, y->pszLine);
	
}

int msort( const char **pszaFNames, const u_int uiFNum, char *ptrBuf, const uint uiBSize,  FILE *fpOut, pid_t iPID)
{
	MFILE	*mfpFP;
	int		iLines, i;
	int		iPNum, iFD;
	long long llTFSize;
	SPPT	*spPTT, *spHNode;
	char	*ptrWBuf, *ptrEnd;
	uint	uiLen;
	
	msort_error_no=MSORT_OK;

	iPNum=iLines=0;
	if( (mfpFP = mfopen( pszaFNames, uiFNum )) == NULL )
		return MSORT_FILE_OPEN;

	llTFSize=mfsize( mfpFP );
	fprintf( stderr, "%d Files, Size: %lld\n", uiFNum, llTFSize);
	TU_Set(1);TU_Set(2);TU_Set(3);
	if( (iLines=ms_fillMem( ptrBuf, uiBSize, mfpFP )) < 0 )
	{
		msort_error_no=(-iLines);
		return MSORT_FILE_READ;
	}
	TU_Interval( " - Load Partition Time", 1, 1);
	qsort( ptrBuf, iLines, sizeof( char *), ms_strcmp );
	//radixsort( ptrBuf, iLines, NULL, 0 );
	TU_Interval( " - Sort Partition Time", 1, 1);
	if( mfeof( mfpFP ) ) // Small files, Finished in first sort.
	{
		mffree( mfpFP );
		if( ms_fbwrite( (char **)ptrBuf, iLines, fpOut ) == false )
			return MSORT_MALLOC;
		else
			return MSORT_OK;
	}else
	{
		iFD=ms_create_file(iPID, iPNum);
		if( ms_bwrite( (char **)ptrBuf, iLines, iFD ) == false )
			return MSORT_MALLOC;
		close( iFD );
		fprintf(stdout, "Partition %4.4d write out.\n", iPNum);
		iPNum++;
	}
	TU_Interval( " - Write Partition Time", 1, 1);
	TU_Interval( " - Total Partition Time", 2, 1);
	while( mfeof( mfpFP )==false )
	{
		if( (iLines=ms_fillMem( ptrBuf, uiBSize, mfpFP )) < 0 )
		{
			msort_error_no=(-iLines);
			return MSORT_FILE_READ;
		}
		TU_Interval( " - Load Partition Time", 1, 1);
		qsort( ptrBuf, iLines, sizeof( char *), ms_strcmp );
		TU_Interval( " - Sort Partition Time", 1, 1);
		//radixsort( ptrBuf, iLines, NULL, 0);
		iFD=ms_create_file(iPID, iPNum);
		if( ms_bwrite( (char **)ptrBuf, iLines, iFD ) == false )
			return MSORT_MALLOC;
		close( iFD );
		TU_Interval( " - Write Partition Time", 1, 1);
		TU_Interval( " - Total Partition Time", 2, 1);		
		fprintf(stdout, "Partition %4.4d write out.\n", iPNum);
		iPNum++;
	}
	mfclose( mfpFP );
	mffree( mfpFP );
	TU_Interval( " - First Phase Time", 3, 1);
	spPTT=(SPPT *)malloc( sizeof( SPPT ) * iPNum );
	TU_Set(1);
	ms_init_merge( spPTT, iPNum, ptrBuf, uiBSize, iPID);
	
	fprintf(stderr, "Merging %d partitions.\n", iPNum);
	HeapCreate(iPNum);
	for( i=0; i<iPNum; i++ )
	{
		mfp_gets(&(spPTT[i]));
		HeapPush( &(spPTT[i]) );
	}
	ptrWBuf=g_ms_wBuf;
	ptrEnd=ptrWBuf+WBUF_SIZE;
	while( !IsEmpty() )
	{
		if( (spHNode=HeapPop())!=NULL )
		{
			uiLen=strlen(spHNode->pszLine)+1;
			if( ptrWBuf + uiLen > ptrEnd )
			{
				fwrite( g_ms_wBuf, 1, ptrWBuf - g_ms_wBuf, fpOut );
//				printf("w %d msort BW\n", ptrWBuf - g_ms_wBuf);
				ptrWBuf=g_ms_wBuf;
				if( uiLen >= WBUF_SIZE )
				{
					spHNode->pszLine[uiLen-1]='\n';
					fwrite( spHNode->pszLine, 1, uiLen, fpOut );
//					printf("w %d msort LW\n", uiLen);
					uiLen=0;
				}
			}
			if( uiLen != 0 )
			{
				spHNode->pszLine[uiLen-1]='\n';
				memcpy( ptrWBuf, spHNode->pszLine, uiLen );
				ptrWBuf+=uiLen;
			}
			if( mfp_gets(spHNode) )
				HeapPush( spHNode );
			else
			{
				mfclose( spHNode->mfp );
				unlink( (spHNode->mfp)->pszFNames[0]);
			}
		}
	}
	if( ptrWBuf - g_ms_wBuf > 0 )
	{
		fwrite( g_ms_wBuf, 1, ptrWBuf - g_ms_wBuf, fpOut );
//		printf("w %d msort LAST\n", ptrWBuf - g_ms_wBuf);
	}
	TU_Interval( " - Merge Time", 1, 1);
	return MSORT_OK;	
}
