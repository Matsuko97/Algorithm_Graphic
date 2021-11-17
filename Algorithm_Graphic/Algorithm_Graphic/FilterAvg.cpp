#include "include.h"

TCHAR szAvgFilter[128] = {0};

/*
���ܣ�����ƽ��ֵ�˲����˴�Ϊ�ֶ��˲��������˲�Ч�������һ��
������
	fileInfo *data��ԭʼ���ݣ�
	int Num�����ݸ�����
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

	//֮ǰ��ʵ�־�ֵ�˲��㷨ʱ��ֱ�ӿ����˼���������������������
	//��˵�ѭ�������������ݸ���ʱ�������Զ����0
	//�����Ƕ�̬������������������ѭ���л��������Խ������º���д�����

	GenerateFileName( szAvgFilter );
	WriteFileInfo( Num - AverageNum , dataNew , szAvgFilter );

	free(dataNew);
	free(dataTemp);
	dataNew = NULL;
	dataTemp = NULL;

	return 0;
}
