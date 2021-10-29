#include "include.h"

TCHAR szAvgFilter[128] = {TEXT("AverageFilter")};

/*
���ܣ�����ƽ��ֵ�˲����˴�Ϊ�ֶ��˲��������˲�Ч�������һ��
������
	fileInfo *data��ԭʼ���ݣ�
	int Num�����ݸ�����
*/

void AverageFilter( fileInfo *data, int Num )

{
	double result = 0;
	double *dataTemp;
	dataTemp = (double*)calloc( number , sizeof(double) );
	FileInfo dataNew = (fileInfo*)calloc( Num , sizeof(fileInfo) );
	int i;	

	for( int a = 0; a < Num - number/2 ; a = a + number )
	{
		for( i = a ; i < (number + a); i++)
			dataTemp[i-a] = (data[i].Y);

		for(int count = 0; count < number; count++)             
		  result += dataTemp[count];

		result = result/number;

		for(i=a ; i<(number+a) ; i++)
		{
			dataNew[i].X = data[i].X;
			dataNew[i].Y = result;
		}
			memset(dataTemp,0,sizeof(double) * number);
	}

	//֮ǰ��ʵ�־�ֵ�˲��㷨ʱ��ֱ�ӿ����˼���������������������
	//��˵�ѭ�������������ݸ���ʱ�������Զ����0
	//�����Ƕ�̬������������������ѭ���л��������Խ������º���д�����

	GenerateFileName( szAvgFilter );
	WriteFileInfo( Num - number/2 , dataNew , szAvgFilter );

	free(dataNew);
	free(dataTemp);

	return ;
}
