#include "include.h"

TCHAR szSPeakName[128] = {0};
TCHAR *szBaseName = NULL;

LinkList Head = NULL;
LinkList Rear = NULL;

double G(int j)
{
	double result = 0;

	result = exp(-4*(j*j/H/H)*log(2.0));
	return result;
}

double C(int j)
{
	//Gaussian
#if 1
	double result = 0;

	for(int i = -m ; i<=m ; ++i)
	{
		result += G( j );
	}

	result = result/W;
	return result;
#endif

	//Square Wave
#if 0
	double result = 0;

	if( j>= ((1-H)/2) &&  j<= ((H-1)/2))
	{
		result = 2;
	}
	else
	{
		result = -1;
	}
	return result;
#endif
}

int SR(fileInfo *dataNew, int num)
{
	if( dataNew == NULL )
	{
		return -1;
		//需要先进行光滑处理
	}

	Head = NULL;
	//将峰链表清空

	TCHAR szBuffer[32]={0};

	szBaseName = (TCHAR*)malloc(128 * sizeof(TCHAR));
	_sntprintf_s(szBaseName, 128, 128, TEXT("Base_Straight"));
	GenerateFileName( szSPeakName );
	GenerateFileName( szBaseName );

	double y = dataNew[0].Y;

	//记录峰信息 为RecordInfo函数提供参数
	int start = 0 , end = 0;

	for(int i = m ; i<num-m ; ++i)
	{
		double temp = 0;
		for(int j = -m ; j<=m ; ++j)
		{
			temp += C(j)*(dataNew[i+j].Y-y);
		}

		temp = sqrt(abs(temp));

		if(temp > f)
		{
			_sntprintf_s(szBuffer, 32, TEXT("%.3lf  %.3lf\n"), dataNew[i].X, dataNew[i].Y);    
			MyWriteFile(1,szBuffer, szSPeakName);
			memset(szBuffer, 0, sizeof(TCHAR) * 32);

			if(end == 0)
			{
				start = i;
				++end;
			}
		}

#if 1
		//非谱峰区域直接输出，可作为直线本底谱
		else
		{
			_sntprintf_s(szBuffer, 32, TEXT("%.3lf  %.3lf\n"), dataNew[i].X, dataNew[i].Y);    
			MyWriteFile(1,szBuffer, szBaseName);
			memset(szBuffer, 0, sizeof(TCHAR) * 32);

			if(start!=0)
			{
				end = i;
				RecordInfo( Head , Rear , start , end-1 );
				end = 0;
				start = 0;
			}
		}
#endif

	}
	return 0;
}

void RecordInfo( LinkList &Head , LinkList &Rear , int start , int end )
{
	if(Head==NULL)
	{
		Head = (LinkList)malloc(sizeof(Node));

		Head->indexStart = start;
		Head->indexEnd = end;
		Head->width = end - start;
		Head->next = NULL;

		Rear = Head;
	}
	else
	{
		LinkList temp = (LinkList)malloc(sizeof(Node));
		temp->indexStart = start;
		temp->indexEnd = end;
		temp->width = end - start;
		temp->next = NULL;

		Rear->next = temp;
		Rear=temp;
	}

	return;
}