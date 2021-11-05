#include "include.h"

TCHAR szFilterRecursive[128] = {0};
LINKPLATFORM         HeadNode;

int RecursionMiddleFilter( fileInfo *data ,int Num )
{
	if( data == NULL )
	{
		return -1;
	}

	double result = 0;
	double change ;
	double k,b;
	double *dataTemp;
	dataTemp = (double*)malloc(sizeof(double) * RecursiveNum); 
	fileInfo *dataExtension;
	dataExtension = (fileInfo*)malloc(sizeof(fileInfo) * RecursiveNum);
	fileInfo *dataNew;
	dataNew = (fileInfo*)malloc(sizeof(fileInfo) * Num);

	LINKPLATFORM   p,q;
	BOOL        Flag = TRUE;
	int i,j;
	int nCount = RecursiveNum/2 ;

	int n = 0;
	double temp = 0;

	for(int a=0 ; a< (Num - 2* nCount);a++ )
	{
		for(i = a ; i < (RecursiveNum + a); i++)
			dataTemp[i-a] = ( data[i].Y );

		for( i=1; i < RecursiveNum; i++ )
			for( j=0; j < RecursiveNum - i; j++ )
			{
				if(dataTemp[j] > dataTemp[j+1])
				{
					change = dataTemp[j];
					dataTemp[j] = dataTemp[j+1];
					dataTemp[j+1] = change;
				}
			}

			result = dataTemp[nCount];

			if( 0 == a )
			{
				for(i=0 ; i < RecursiveNum; i++)
				{
					dataExtension[i].X = data[i].X;
					dataExtension[i].Y = data[i].Y;
				}
				LeastSquares( dataExtension , RecursiveNum , &k , &b );
				for(i=0 ; i < nCount; i++)
				{
					dataNew[i].X = data[i].X;
					dataNew[i].Y = k * dataNew[i].X  +b;
				}
				memset(dataExtension, 0, sizeof(fileInfo) * RecursiveNum);
			} 

			dataNew[ a + nCount ].X = data[ a + nCount ].X;
			dataNew[ a + nCount ].Y = result;
			memset( dataTemp, 0, sizeof(double) * RecursiveNum);
	}

	free( dataExtension );

	dataExtension = (fileInfo*)malloc(sizeof(fileInfo) * nCount);

	for( i = 0; i < nCount; i++ )
	{
		dataExtension[i].X = dataNew[ Num - RecursiveNum + i].X;
		dataExtension[i].Y = dataNew[ Num - RecursiveNum + i].Y;
	}
	LeastSquares( dataExtension , nCount , &k , &b );
	for( i = Num - nCount ; i < Num; i++)
	{
		dataNew[i].X = data[i].X;
		dataNew[i].Y = k * dataNew[i].X + b;
	}
	memset(dataExtension, 0, sizeof(fileInfo) * nCount);

	int width=0;
	BOOL flag_rise = FALSE;
	BOOL flag_decline = FALSE;

	for(int n=1; n<Num; n++)
	{
		if(	dataNew[n].Y - dataNew[n-1].Y > 5)
		{
			if((width!=0)&&flag_decline && width < RecursiveNum/2)
			{
				if (Flag)              //判断头节点是否已经建立
				{
					HeadNode = (LINKPLATFORM) malloc(sizeof(PLATFORM));
					HeadNode->next = NULL;
					p = HeadNode;
					Flag = FALSE;
				}
				else
				{
					q = (LINKPLATFORM) malloc(sizeof(PLATFORM));
					q->next = NULL;
					p->next = q;
					p = q;
				}
				p->ind_end = n-1;       
				p->ind_start = p->ind_end-width;    
				p->ind_width = width;
			}
			width=0;
			flag_rise=TRUE;
		}

		else if(dataNew[n].Y - dataNew[n-1].Y< -5)
		{
			if((width!=0) && flag_rise && width < RecursiveNum/2)
			{
				if (Flag)              //判断头节点是否已经建立
				{
					HeadNode = (LINKPLATFORM) malloc(sizeof(PLATFORM));
					HeadNode->next = NULL;
					p = HeadNode;
					Flag = FALSE;
				}
				else
				{
					q = (LINKPLATFORM) malloc(sizeof(PLATFORM));
					q->next = NULL;
					p->next = q;
					p = q;
				}
				p->ind_end = n-1;       
				p->ind_start = p->ind_end-width;   
				p->ind_width = width;
			}
			width=0;
			flag_decline=TRUE;
		}

		else
		{
			width++;
		}
	}
	p = HeadNode;
	while(p != NULL)
	{
		int n = p->ind_start;
		int a = 0;
		int listlength = p->ind_width;
		for(  ; listlength >=0 ;listlength = listlength-2,n++)
		{
			dataTemp = (double*)malloc(sizeof(double) * listlength); 
			for(i=n-listlength/2 ; i<=n+listlength/2; a++,i++)
				dataTemp[a] = data[i].Y;
			for( i=1; i < listlength; i++ )
				for( j=0; j < listlength - i; j++ )
				{
					if(dataTemp[j] > dataTemp[j+1])
					{
						change = dataTemp[j];
						dataTemp[j] = dataTemp[j+1];
						dataTemp[j+1] = change;
					}
				}
				result = dataTemp[nCount];
				memset( dataTemp, 0, sizeof(double) * listlength);
				dataNew[n].Y = result;			
		}
		n = p->ind_end;
		for(; listlength > 1; listlength = listlength-2, n--)
		{
			dataTemp = (double*)malloc(sizeof(double) * listlength); 
			for(i=n-listlength/2 ; i<=n+listlength/2;a++, i++)
				dataTemp[a] = data[i].Y;
			for( i=1; i < listlength; i++ )
				for( j=0; j < listlength - i; j++ )
				{
					if(dataTemp[j] > dataTemp[j+1])
					{
						change = dataTemp[j];
						dataTemp[j] = dataTemp[j+1];
						dataTemp[j+1] = change;
					}
				}

				result = dataTemp[nCount];
				memset(dataTemp,0,sizeof(double) * listlength);
				dataNew[n].Y = result;			
		}
		dataNew[n-1].Y = data[n-1].Y ;
		p = p->next;
	}
	p=NULL;
	free(dataTemp);
	free(dataExtension);

	GenerateFileName( szFilterRecursive );
	WriteFileInfo( Num , dataNew , szFilterRecursive );

	free(dataNew);
	dataTemp = NULL;
	dataExtension = NULL;
	dataNew = NULL;

	return 0;
}


void LeastSquares( fileInfo *data, int num, double *k, double *b )
{
	double xSquareSum = 0.0;
	double ySum = 0.0;
	double xSum = 0.0;
	double x_multiply_y = 0.0;
	double xSumAverage = 0.0;
	double ySumAverage = 0.0;

	for (int i = 0; i < num; ++i) 
	{
		xSquareSum += data[i].X * data[i].X;
		ySum += data[i].Y;
		xSum += data[i].X;
		x_multiply_y += data[i].X * data[i].Y;
	}

	xSumAverage = xSum / num;
	ySumAverage = ySum / num;

	*k = (x_multiply_y / num - xSumAverage * ySumAverage)/(xSquareSum / num - xSumAverage * xSumAverage);
	*b = (xSquareSum * ySum - xSum * x_multiply_y )/( num * xSquareSum - xSum * xSum );

	return ;
}