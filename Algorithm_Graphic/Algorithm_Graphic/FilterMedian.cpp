#include "include.h"

TCHAR szMedianFilter[128] = {TEXT("MedianFilter")};

void MedianFilter( fileInfo *data, int Num )

{
	double result = 0;
	double change = 0;
	FileInfo dataNew = (fileInfo*)calloc( Num , sizeof(fileInfo) );

	double *dataTemp;
	dataTemp = (double*)malloc(sizeof(double) * number); 

	int i,j;
	int nCount = MedianNum/2 ;
	
	for(int a = 0 ; a < (Num - nCount); a++ )
	{
		for(i = a ; i < (MedianNum + a); i++)
			dataTemp[i-a] = (data[i].Y);

		for( i = 1; i < MedianNum; i++ )
			for( j = 0; j < MedianNum - i; j++ )
			{
				if(dataTemp[j] > dataTemp[j+1])
				{
					change = dataTemp[j];
					dataTemp[j] = dataTemp[j+1];
					dataTemp[j+1] = change;
				}
			}
	for(int count = 1 ; count < MedianNum - 1; count++)             
        result += dataTemp[count]; 
		result = result/(MedianNum - 2);

		dataNew[a].X = data[a + nCount].X ;
		dataNew[a].Y = result;
		memset( dataTemp, 0, sizeof(double) * number);
	}

	GenerateFileName( szMedianFilter );
	WriteFileInfo( Num - MedianNum, dataNew , szMedianFilter );

	free(dataNew);
	free(dataTemp);

	return ;
}
