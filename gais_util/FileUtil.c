#include "FileUtil.h"
#include "Log.h"
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int		g_iFileEndFlag;
char	cOpenType;


FILE *ResultOpen(const char *czFileName, const char *czMode)
{
	if( czFileName==NULL || czFileName[0]=='\0' )
	{
		cOpenType=otSTDOUT;
		return stdout;
	}else if(czFileName[0]=='|')
	{
		cOpenType=otPIPE;
		return popen(czFileName+1,czMode);
	}else
	{
		cOpenType=otFILE;
		return fopen(czFileName,czMode);
	}
}

void ResultClose(FILE *fpResult)
{
	if( cOpenType == otPIPE )
		pclose(fpResult);
	else if( cOpenType == otFILE )
		fclose(fpResult);
	else
		fclose(stdout);
}

int FileRead(uint uiRemain, uint iMaxRead, void *vpPBuffer, void *vpBuffer, int fdFile)
{
	int iCurrentRead;
	static uint x=0;
	
	if( uiRemain>0 )
		memcpy(vpBuffer,vpPBuffer,uiRemain);
	if( (iCurrentRead=read( fdFile, vpBuffer+uiRemain, iMaxRead-uiRemain ))<0)
		return iCurrentRead;
	x+=iCurrentRead/(1024*1024);
	if( x%200 <= 3 )
		LogPrintf(LOG_DEBUG,"Total Size Read: %d MB\n",x);
	if( iCurrentRead < iMaxRead-uiRemain )
		g_iFileEndFlag=1;
	else
		g_iFileEndFlag=0;
	return uiRemain+iCurrentRead;
}

int fpFileRead(uint uiRemain, uint iMaxRead, void *vpPBuffer, void *vpBuffer, FILE *fpFile)
{
	int iCurrentRead;
	
	if( uiRemain>0 )
		memcpy(vpBuffer,vpPBuffer,uiRemain);
	if( (iCurrentRead=fread(  vpBuffer+uiRemain,1, iMaxRead-uiRemain, fpFile))<0)
		return iCurrentRead;
	if( iCurrentRead < iMaxRead-uiRemain )
		g_iFileEndFlag=1;
	else
		g_iFileEndFlag=0;
	return uiRemain+iCurrentRead;
}

int TraverseFileList(const char *pszFList, int (*funcHandler)(const char *,const char *,const char *), int nLevel)
{
  char	pszFName[256];
  FILE	*fpFList;
  int	iLen;

  if( (fpFList=fopen( pszFList, "r" ))==NULL )
    return false;
  while( fgets(pszFName, 255, fpFList) )
  {
    iLen=strlen(pszFName);
    if( iLen>0 && pszFName[iLen-1] == '\n' )
      pszFName[iLen-1]='\0';
    if( iLen>1 && pszFName[iLen-2] == '\r' )
      pszFName[iLen-2]='\0';
    TraverseDir(pszFName, funcHandler, nLevel);
  }
  fclose(fpFList);
  return true;  
}

int TraverseDir(const char *cpEntryPoint, int (*funcHandler)(const char *,const char *,const char *), int nLevel)
{
	int 			iTotal,i;
    struct dirent	**dirlist;
    char			FileName[256], *pszPtr;
    struct stat		st;
    
	if( stat(cpEntryPoint, &st) == -1 )
		return 0;
	if( S_ISREG( st.st_mode ) )
	{
		pszPtr=strrchr( cpEntryPoint, '/' );
		if( pszPtr )
			pszPtr++;
		else
			pszPtr=(char *)cpEntryPoint;
		funcHandler("./",pszPtr,cpEntryPoint);
		return 1;
	}
	iTotal = scandir(cpEntryPoint, &dirlist, NULL , alphasort);
	for(i=0;i<iTotal;i++)
	{
		if( !strcmp(dirlist[i]->d_name,".") || !strcmp(dirlist[i]->d_name,"..") )
		{
			free(dirlist[i]);
			continue;
		}
		if( cpEntryPoint[strlen(cpEntryPoint)-1]=='/' )
			sprintf(FileName,"%s%s",cpEntryPoint,(char *)dirlist[i]->d_name);
		else
			sprintf(FileName,"%s/%s",cpEntryPoint,(char *)dirlist[i]->d_name);
		stat(FileName, &st);
		if( S_ISDIR( st.st_mode ) )
		{
			if( nLevel != 0 )
				TraverseDir(FileName, funcHandler, nLevel-1 );
		}
		else
		{
			if( !funcHandler( cpEntryPoint, dirlist[i]->d_name, FileName) )
			{
				free(dirlist[i]);
				break;
			}
		}
		free(dirlist[i]);
	}
	free(dirlist);
    return 1;
}

inline int IsDir( char    *FName)
{
    struct stat st;

    return ( stat( FName, &st ) == 0 && S_ISDIR( st.st_mode ) );
}

inline int IsFile( char    *FName)
{
    struct stat st;

    return ( stat( FName, &st ) == 0 && S_ISREG( st.st_mode ) );
}

inline int IsExecute( char *pszFName )
{
	struct stat st;
	uid_t	uid;
	gid_t	gid;
	
	if( stat(pszFName, &st) != 0 || S_ISREG( st.st_mode )==0 )
		return 0;
	uid=getuid();
	gid=getgid();
	if( (S_IXOTH&st.st_mode) || (uid==st.st_uid && (S_IXUSR&st.st_mode)) || (gid==st.st_gid && (S_IXGRP&st.st_mode)) )
		return 1;
	return 0;	
}

inline off_t FileSize( char *FName)
{
    struct stat st;

	if( stat( FName, &st ) <0 )
		return -1;
    return st.st_size;
}

int FileLines( char *pszFN )
{
  int fd, i, ln;
  ssize_t n;
  char	pszBuf[4096];

  ln=0;
  fd=open( pszFN, O_RDONLY);
  if( fd<0 )
    return ln;
  while( (n=read(fd, pszBuf, 4096)) >0 )
  {
    for( i=0; i<n; i++ )
    {
      if( pszBuf[i]=='\n' )
        ln++;
    }
  } 
  close( fd );
  return ln;
}
