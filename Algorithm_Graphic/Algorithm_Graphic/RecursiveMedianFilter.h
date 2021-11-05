#ifndef _RECURSIVEMEDIANFILTER_H_
#define _RECURSIVEMEDIANFILTER_H_

#define RecursiveNum  31

typedef struct platform
{
	int           ind_start;
	int           ind_end;
	int           ind_width;
	struct platform   *next;
}PLATFORM,*LINKPLATFORM;

int RecursionMiddleFilter( fileInfo *data, int Num );
void LeastSquares( fileInfo *data, int num, double *k, double *b);

extern TCHAR szFilterRecursive[128];

#endif