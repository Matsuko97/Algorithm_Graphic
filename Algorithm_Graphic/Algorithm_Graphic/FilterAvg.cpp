#include "include.h"

TCHAR szAvgFilter[128] = {0};

/*
功能：进行平均值滤波，此处为分段滤波，递推滤波效果会更好一点
参数：
	fileInfo *data：原始数据；
	int Num：数据个数；
*/

int AverageFilter( fileInfo *data, int Num )
{
	if( data == NULL )
	{
		return -1;
	}

	double result = 0;
	double *dataTemp;
	dataTemp = (double*)calloc( AverageNum , sizeof(double) );
	FileInfo dataNew = (fileInfo*)calloc( Num , sizeof(fileInfo) );
	int i;	

	for( int a = 0; a < Num - AverageNum ; a = a + AverageNum )
	{
		for( i = a; i < (AverageNum + a); i++)
			dataTemp[i-a] = (data[i].Y);

		result = 0;
		for(int count = 0; count < AverageNum; count++)
			result += dataTemp[count];

		result = result/AverageNum ;

		for(i = a; i < (AverageNum + a); i++)
		{
			dataNew[i].X = data[i].X;
			dataNew[i].Y = result;
		}
			memset(dataTemp, 0, sizeof(double) * AverageNum);
	}

	//之前在实现均值滤波算法时，直接开辟了极大个数的数组来存放数据
	//因此当循环次数超出数据个数时，数据自动填充0
	//现在是动态开辟数组个数，因此在循环中会出现数组越界而导致后续写入出错

	GenerateFileName( szAvgFilter );
	WriteFileInfo( Num - AverageNum , dataNew , szAvgFilter );

	free(dataNew);
	free(dataTemp);
	dataNew = NULL;
	dataTemp = NULL;

	return 0;
}
