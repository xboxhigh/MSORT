#include	<stdio.h>
#define	uchar	unsigned char
#define patbufsize      (1<<16)
#define MaxPatSize      256

#define Multi           3       // number of operations allowed in augment()


typedef struct  {
        int     *HashTab;
        int     nHashTab;
        int     *Pid;
        uchar   **DataArray;
	uchar	*patpool;
        int	npat;
	uchar	**origpats;
	int	norigpat;
} AMemberM;



