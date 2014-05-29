#ifndef LOG_H
#define LOG_H

#define   OVERWRITE_LOG 0
#define   APPEND_LOG    1

#define   LOG_NOT_SHOWTIME  0
#define   LOG_SHOWTIME      1

enum log_options {LOG_ALWAYS, LOG_VERBOSE, LOG_DEBUG};


/*
   Library Name: write log message
          input: fn      --> log file name
                 wMode   --> write mode, overwrite or append
                 verbose --> value = 1 , show verbose
                 debug   --> value = 1 , show debug
                 nShowTimeFlag -> value =1 , show time in logfile
         Return: 1   --> success
                 0   --> falure
    First Build: 2001.08.27
    Last Modify:
        Version: 0.01
*/
int   LogInitByFile(char *fn, int wMode, int verbose, int debug_msg, int nShowTimeFlag);

int   LogInit(char *file, int appendp, int stime, int verb, int debug);
void  LogClose(void);
void  LogFlush(void);
void  LogPrintf(enum log_options o, const char *, ...);

//extern FILE *gfpLogFile;

#endif
