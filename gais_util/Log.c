#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <errno.h>
#include "Log.h"

#ifndef errno
extern int errno;
#endif


FILE *gfpLogFile;
static int  gLog_ShowTime = 0;
static int  gLog_Verbose = 0;
static int  gLog_Debug = 0;


#define CHECK_VERBOSE(x)                        \
switch (x)                                      \
{                                               \
   case LOG_ALWAYS:                             \
      break;                                    \
   case LOG_VERBOSE:                            \
      if (!gLog_Verbose)                        \
      return;                                   \
      break;                                    \
   case LOG_DEBUG:                              \
      if (!gLog_Debug)                          \
        return;                                 \
      break;                                    \
}




int  LogInit(char *file, int appendp, int stime, int verbose, int debug)
{

  if(stime)
     gLog_ShowTime = 1;
  if(verbose)
     gLog_Verbose = 1;
  if(debug)
     gLog_Debug = 1;


  if(file)
  {
     gfpLogFile = fopen(file, appendp ? "a" : "w");
     if (!gfpLogFile)
     {
        perror(file);
        return -1;
     }
  }
  else
     gfpLogFile = stdout;
     
  return 1;
}


void  LogClose(void)
{
   fclose(gfpLogFile);

}

void LogFlush(void)
{
   fflush(gfpLogFile);
}


void  LogPrintf(enum log_options o, const char *fmt, ...)
{
   char buf[1024];
   va_list ap;

   CHECK_VERBOSE (o);
   if (gLog_ShowTime)
   {
      time_t now;
      struct tm *ctm;
      
      time(&now);
      ctm=localtime(&now);      
      sprintf(buf, "[%4.4d/%2.2d/%2.2d %2.2d:%2.2d:%2.2d] ",
                  ctm->tm_year+1900, ctm->tm_mon+1, ctm->tm_mday, ctm->tm_hour,
                  ctm->tm_min,ctm->tm_sec);
      fwrite(buf, strlen(buf), 1, gfpLogFile);
   }
   
   va_start(ap, fmt);
   (void)vfprintf(gfpLogFile, fmt, ap);
   LogFlush();
}


int   LogInitByFile(char *fn, int wMode, int verbose, int debug_msg, int nShowTimeFlag)
{
   if ((verbose == 1) && (debug_msg == 1))
      return LogInit(fn, wMode, nShowTimeFlag, 1, 1);
   else if ((verbose == 1) && (debug_msg == 0))
      return LogInit(fn, wMode, nShowTimeFlag, 1, 0);
   else if ((verbose == 0) && (debug_msg == 1))
      return LogInit(fn, wMode, nShowTimeFlag, 0, 1);
   else if ((verbose == 0) && (debug_msg == 0))
      return LogInit(fn, wMode, nShowTimeFlag, 0, 0);   

   return 0;   
}
   