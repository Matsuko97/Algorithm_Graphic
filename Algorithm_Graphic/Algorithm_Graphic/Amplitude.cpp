#include "include.h"

double threshold = 0.0;
double deviation = 0.5;
TCHAR szAmpName[128] = {0};

int Amplitude(fileInfo *dataNew, int num)
{
	if( dataNew == NULL )
	{
		return -1;
		//需要先进行光滑处理
	}

	Head = NULL;
	//将峰链表清空

	GenerateFileName( szAmpName );

	//限幅
	TCHAR szBuffer[32]={0};
	int start = 0 , end = 0;
	//

	int i = 0;
	threshold = dataNew[0].Y;

	for(; i<num-1 ; ++i)
	{
		double temp = 0.0;
		temp = dataNew[i+1].Y - threshold ;
		if(abs(temp) > deviation)
		{
			//限幅
			_sntprintf_s(szBuffer, 32, TEXT("%.3lf  %.3lf\n"), dataNew[i].X, dataNew[i].Y);    
			MyWriteFile(1,szBuffer, szAmpName);
			memset(szBuffer, 0, sizeof(TCHAR) * 32);
			if(end == 0)
			{
				start = i;
				++end;
			}
			else
			{}//
		}
		//限幅
		else
		{
			if(start!=0)
			{
				end = i;
				RecordInfo( Head , Rear , start , end-1 );
				end = 0;
				start = 0;
				//threshold = dataNew[i].Y;
				//动态调整阈值 threshold 时，依据绝对值来判定峰，导致threshold上下的deviation
				//都判定为峰，在曲线上就是峰结束后有一段下降的趋势，也判定为峰
			}
			else
			{}
		}
	}

	return 0;
}