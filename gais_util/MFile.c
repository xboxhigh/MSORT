#include "MFile.h"
#include "FileUtil.h"
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>

int		mfile_error_no;

void mfrewind( MFILE *mfpFP )
{
	mfile_error_no=MFILE_OK;
	if( mfpFP->iFP != 0 )
	{
		close( mfpFP->iFP );
		mfpFP->iFP=0;
	}
	mfpFP->cEOF=true;
	mfpFP->uiInBuf=0;
	mfpFP->uiBPos=0;
	mfpFP->uiCurFile=0;
}

int mfseek( MFILE *mfpFP, off_t llOffset )
{
	off_t	llRemain;
	int		i;
	
	if( mfpFP->llaSize == NULL )
		mfsize( mfpFP );
	llRemain=llOffset;
	for( i=0; i<mfpFP->uiFiles ;i++ )
	{
		if( llRemain > mfpFP->llaSize[i] )
			llRemain-=mfpFP->llaSize[i];
		else
			break;
	}
	if( i >= mfpFP->uiFiles )
	{
		mfile_error_no=MFILE_SIZE;
		return false;
	}
	if( mfpFP->uiCurFile != i || mfpFP->iFP == 0 )
	{
		if( mfpFP->iFP != 0 )
			close( mfpFP->iFP );
		if( (mfpFP->iFP = open(mfpFP->pszFNames[ i ], O_RDONLY)) == -1 )
		{
			mfile_error_no=MFILE_FILE_NOT_FOUND;
			return false;
		}
	}
	mfpFP->uiCurFile=i;
	if( lseek( mfpFP->iFP, llRemain, SEEK_SET ) == -1)
	{
		mfile_error_no=MFILE_LSEEK;
		return false;
	}
	mfpFP->uiInBuf=0;
	mfpFP->uiBPos=0;
	mfpFP->cEOF=false;
	return true;
}

MFILE *mfopen(const char **pszFNames, const uint uiFiles)
{
	MFILE *mfpFP;
	
	mfile_error_no=MFILE_OK;
	mfpFP=(MFILE *)malloc( sizeof(MFILE) );
	if( mfpFP == NULL )
	{
		mfile_error_no=MFILE_MALLOC;
		return NULL;
	}
	bzero( mfpFP, sizeof( MFILE ) );
	mfpFP->uiBufSize=MFILE_BUF_SIZE;
	mfpFP->ptrBuf=(char *)malloc(sizeof(u_char) * (mfpFP->uiBufSize + 1) );
	if( mfpFP->ptrBuf == NULL )
	{
		mfile_error_no=MFILE_MALLOC;
		free( mfpFP );
		return NULL;
	}
	mfpFP->pszFNames=(char **)pszFNames;
	mfpFP->uiFiles=(uint)uiFiles;
	mfpFP->cEOF=true;
	return mfpFP;
}

MFILE *mfopen_m(const char **pszFNames, const uint uiFiles, char *pszBuf, uint uiMSize)
{
	MFILE *mfpFP;
	
	mfile_error_no=MFILE_OK;
	mfpFP=(MFILE *)malloc( sizeof(MFILE) );
	if( mfpFP == NULL )
	{
		mfile_error_no=MFILE_MALLOC;
		return NULL;
	}
	bzero( mfpFP, sizeof( MFILE ) );
	mfpFP->uiBufSize=uiMSize-1;
	mfpFP->ptrBuf=(char *)pszBuf;
	mfpFP->pszFNames=(char **)pszFNames;
	mfpFP->uiFiles=(uint)uiFiles;
	mfpFP->cEOF=true;
	return mfpFP;
}


off_t mfsize( MFILE *mfpFP)
{
	int		i=0;
	off_t	llTSize;
	
	if( mfpFP->llaSize == NULL )
	{
		mfpFP->llaSize=(off_t *)malloc( sizeof(off_t)*mfpFP->uiFiles );
		for( i=0; i<mfpFP->uiFiles; i++ )
			mfpFP->llaSize[i]=FileSize( mfpFP->pszFNames[i] );
	}
	
	llTSize=0;
	for( i=0; i<mfpFP->uiFiles; i++ )
		llTSize+=(mfpFP->llaSize[i]);
	return llTSize;
}

void mrefopen(const char **pszFNames, const uint uiFiles, MFILE *mfpFP)
{
	mfile_error_no=MFILE_OK;
	if( mfpFP->iFP != 0 )
	{
		close( mfpFP->iFP );
		mfpFP->iFP=0;
	}
	if( mfpFP->llaSize != NULL )
	{
		free(mfpFP->llaSize);
		mfpFP->llaSize=NULL;
	}
	mfpFP->pszFNames=(char **)pszFNames;
	mfpFP->uiFiles=(uint)uiFiles;
	mfpFP->cEOF=true;
	mfpFP->uiInBuf=0;
	mfpFP->uiBPos=0;
	mfpFP->uiCurFile=0;
}

void mfclose( MFILE *mfpFP )
{
	mfile_error_no=MFILE_OK;
	if( mfpFP->iFP != 0 )
	{
		close( mfpFP->iFP );
		mfpFP->iFP=0;
	}
	mfpFP->cEOF=true;
	mfpFP->uiInBuf=0;
	mfpFP->uiBPos=0;
	mfpFP->uiCurFile=0;
	mfpFP->uiFiles=0;
}

void mffree( MFILE *mfpFP )
{
	mfile_error_no=MFILE_OK;
	
	if( mfpFP == NULL )
		return;
	if( mfpFP->ptrBuf != NULL )
		free(mfpFP->ptrBuf);
	if( mfpFP->iFP != 0 )
		close( mfpFP->iFP );
	if( mfpFP->llaSize != NULL )
		free(mfpFP->llaSize);
	free( mfpFP );
	return;
}

int mfread( MFILE *mfpFP )
{
	int 	iCRead;
	uint	uiRemain, uiNeed;

	uiRemain=mfpFP->uiInBuf - mfpFP->uiBPos;
	//printf("uiRemain:%d\n", uiRemain);
	if( uiRemain>0 )
		memcpy( mfpFP->ptrBuf, mfpFP->ptrBuf+mfpFP->uiBPos, uiRemain);
	uiNeed=mfpFP->uiBufSize-uiRemain;
	if( (iCRead=read( mfpFP->iFP, mfpFP->ptrBuf+uiRemain, uiNeed ))<0)
		return iCRead;
	if( iCRead < uiNeed )
		mfpFP->cEOF=true;
	else
		mfpFP->cEOF=false;
	mfpFP->uiInBuf=iCRead+uiRemain;
	mfpFP->uiBPos=uiRemain;
	return iCRead;
}

char *im_mfgets( uint *uiLen, int *iLRCF, MFILE *mfpFP)
{
	uint	uiBakBPos, uiLStart;
	
	mfile_error_no=MFILE_OK;
	uiLStart=mfpFP->uiBPos;
	(*iLRCF)=false;
	while( true )
	{
		while( mfpFP->uiBPos < mfpFP->uiInBuf )
		{
			if( mfpFP->ptrBuf[ mfpFP->uiBPos ] == '\n' )
			{
				(*iLRCF)=true;
				mfpFP->uiBPos++;
				break;
			}
			mfpFP->uiBPos++;
		}
		(*uiLen)= mfpFP->uiBPos - uiLStart;
		if( (*iLRCF) )
			return &(mfpFP->ptrBuf[ uiLStart ]);
		else if( mfpFP->cEOF == false )
		{
			if( uiLStart == 0 && mfpFP->uiInBuf > 0)
				return &(mfpFP->ptrBuf[ uiLStart ]); //Line size > Memory
			uiBakBPos=uiLStart-mfpFP->uiBPos;
			mfpFP->uiBPos=uiLStart;
			if( mfread(mfpFP) < 0 )
			{
				mfile_error_no=MFILE_READ;
				return NULL;
			}
			uiLStart=0;
			mfpFP->uiBPos+=uiBakBPos;
		}else if( (*uiLen) > 0 )
		{
			return &(mfpFP->ptrBuf[ uiLStart ]);
		}
		else if( mfpFP->uiCurFile >= mfpFP->uiFiles )
			return NULL;
		else
		{
			if( mfpFP->iFP != 0 )
				close( mfpFP->iFP );
			if( (mfpFP->iFP = open(mfpFP->pszFNames[ mfpFP->uiCurFile ], O_RDONLY)) == -1 )
			{
				mfile_error_no=MFILE_FILE_NOT_FOUND;
				return NULL;
			}
			mfpFP->uiCurFile++;
			mfpFP->uiInBuf=0;
			mfpFP->uiBPos=0;
			mfpFP->cEOF=false;
			uiLStart=0;
		}
	}
}

int mfeof( MFILE *mfpFP )
{
	if( mfpFP->uiCurFile >= mfpFP->uiFiles && mfpFP->cEOF && mfpFP->uiBPos >= mfpFP->uiInBuf )
		return true;
	else
		return false;
		
}