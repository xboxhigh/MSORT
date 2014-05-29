#ifndef H_URLUTIL
#define H_URLUTIL

#define MAX_HOST_LEN	512

char *GetHostFromUrl(const char *url);
int CheckURLSite(char *cpURL);
int ChkUrlIsCGI(char *cpUrl);
int NormalizeURL(char *url, int *urllen);
int GetUrlLevel(const char *cpUrl);
int DecodeURL(char *czURL);

#endif