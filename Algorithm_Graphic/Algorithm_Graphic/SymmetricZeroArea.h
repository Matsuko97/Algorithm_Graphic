#ifndef _SYMMETRICZEROAREA_H_
#define _SYMMETRICZEROAREA_H_

//Gaussian
static int m = 5;//2m+1=W
static int W = 11;
static int H = 4;
static double f = 2.0;

//Square
//static int m = 31;//2m+1=W
//static int W = 63;
//static int H = 21;
//static double f = 2.0;

typedef struct NODE
{
	int indexStart;
	int indexEnd;
	int width;
	struct NODE *next;
}*LinkList,Node;

extern LinkList Head;
extern LinkList Rear;

extern TCHAR szSPeakName[128];
extern TCHAR szBaseName[128];

double G(int j);
double C(int j);
void SR(fileInfo *dataNew, int num);

void RecordInfo( LinkList &Head , LinkList &Rear , int start , int end );

#endif