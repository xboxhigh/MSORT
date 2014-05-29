/*
    TCP/UDP Client/Server Library Header File.
    687410003 ±i½å©v @ DNA Workroom
    Version: 1.1    1998.11.20
    Version: 1.0    1996.6.30
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
                    
extern int errno;
/*
    Only root can use port < 1024, so you can define portbase
    to avoid your code use the port number less than 1024.
*/
#define portbase 0
#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif
int passivesock(const char *service, const char *transport,int qlen);
int passiveTCP(const char *service,int qlen);
int passiveUDP(const char *service);
int connectsock(const char *host,const char *service,const char *transport);
int connectUDP(const char *host,const char *service);
int connectTCP(const char *host,const char *service);
int cs_accept(int iSocket);