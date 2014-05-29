#include "Error.h"
#include "Log.h"

char *ErrorMessage[]=
{
    "Lack of memory.",
    "File open error.",
    "File read error.",
    "Share Memory open attach/create error.",
    "Error while start.",
    "Can't connect to server.",
    "Directory Open/Create error.",
	"File write error."
};

void ErrorExit(int ErrNo,char *ExtraMsg)
{
	LogPrintf(LOG_ALWAYS,"%s\n",ErrorMessage[ErrNo]);
    if( ExtraMsg!=NULL )
		LogPrintf(LOG_ALWAYS,"%s\n",ExtraMsg);
	LogPrintf(LOG_ALWAYS,"Program terminated.\n");
    LogClose();
    exit(1);
}