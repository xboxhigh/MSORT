#ifndef H_ERROR
#define H_ERROR
#include <string.h>

enum { errLackofMem,errFileOpenError,errFileReadError,errOpenShareMemory,errStart ,
               errConnect, errDir, errFileWriteError};
void ErrorExit(int ErrNo,char *ExtraMsg);

#endif
