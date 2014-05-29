#ifndef H_TIMEUTIL
#define H_TIMEUTIL
#include <stdio.h>

void TU_SetOutput( FILE *fp );
void TU_Init( int iNum );
int TU_Clear( int iNum );
int TU_ClearAll();
int TU_Set( int iNum );
int TU_Interval( char *pszString, int iNum, int iSet );
int TU_Add( char *pszString, int iNum, int Num2, int iSet );
int TU_Output( char *pszString, int iNum );
#endif