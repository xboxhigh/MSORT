#include "Inifile.h"
#include "FileUtil.h"
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>               
               
SIniNode	sNode[MAX_INI_FIELD];
char		*cpIniContent;
int			iIniNum;

int CmpIniNode(const void *x, const void *y)
{
	SIniNode *a,*b;
	
	a=(SIniNode *)x;
	b=(SIniNode *)y;
	
	return strcasecmp(a->cpKey, b->cpKey);
}

inline int BiSearch(const char *IniName)
{
    int hi,low,mid,cmp;

    if(iIniNum<=0)
        return -1;
    hi=iIniNum-1;
    low=0;
    while(low<=hi)
    {
        mid=(low+hi)/2;
        cmp=strcasecmp(sNode[mid].cpKey,IniName);
        if(cmp==0)
            return mid;
        if(cmp>0)
            hi=mid-1;
        else
            low=mid+1;
    }
    return -1;
}

char *GetIniValue(const char *czKey)
{
	int n;
	
	n=BiSearch(czKey);
	if( n==-1 )
		return NULL;
	else
		return sNode[n].cpValue;
}

int ReadIniFile(const char *czFileName)
{
	int		iFileSize;
	int		fdIni,i;
	int		iFilePos=0;
	char	*cpIniStart;
	
	iIniNum=0;
	iFileSize=FileSize((char *)czFileName);
	if( iFileSize<=0 )
		return 0;
	if( (fdIni=open(czFileName,O_RDONLY))==-1 )
		return 0;
	cpIniContent=(char *)malloc(iFileSize+1);
	read(fdIni, cpIniContent, iFileSize);
	cpIniContent[iFileSize]='\0';
	cpIniStart=cpIniContent;
	while( cpIniContent[iFilePos]!='\0')
	{
		iFilePos++;
		if( cpIniContent[iFilePos-1]!='\n' )
			continue;
		cpIniContent[iFilePos-1]='\0';
		if( cpIniStart[0] != '#' )
		{
			sNode[iIniNum].cpKey=(char *)strtok(cpIniStart, "\r\n \t=:");
			if( sNode[iIniNum].cpKey != NULL )
			{
				for( i=strlen(sNode[iIniNum].cpKey)+1; cpIniStart[i]!='\n' && cpIniStart[i]!='\0'; i++ )
				{
					if(cpIniStart[i]=='\t' || cpIniStart[i]==' ' || cpIniStart[i]=='=' || cpIniStart[i]==':')
						continue;
					else
						break;
				}
				if( cpIniStart[i]=='\n' || cpIniStart[i]=='\0')
					sNode[iIniNum].cpValue=NULL;
				else
				{
					if( cpIniStart[i]=='\"' )
					{
						char *ptr;
					
						ptr=&(cpIniStart[i+1]);
						for(i=i+1;cpIniStart[i]!='\n' && cpIniStart[i]!='\0';i++)
						{
							if( cpIniStart[i]=='\"' )
								cpIniStart[i]='\0';
							if( cpIniStart[i]=='\\' )
							{
								if( cpIniStart[i+1]!='\n' )
									i++;
							}
						}
						if( ptr==&(cpIniStart[i]) )
							sNode[iIniNum].cpValue=NULL;
						else
							sNode[iIniNum].cpValue=ptr;
					}else
						sNode[iIniNum].cpValue=&(cpIniStart[i]);
				}
				iIniNum++;
			}
			if( iIniNum >= MAX_INI_FIELD )
			{
				printf("More than MAX ini field found, truncated.\n");
				break;
			}
		}
		cpIniStart=&(cpIniContent[iFilePos]);
	}
	qsort( sNode, iIniNum ,sizeof(SIniNode), CmpIniNode);
	close(fdIni);	
	return 1;
}

