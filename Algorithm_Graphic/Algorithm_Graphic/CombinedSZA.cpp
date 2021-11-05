#include "include.h"

double CombinedThreshold = 0.0;
double CombinedDeviation = 0.3;
double y = 0.0;

TCHAR szCombinedPeakName[128] = {0};

void CombinedSR(fileInfo *dataNew, int num, int &i)
{
	TCHAR szBuffer[32]={0};

	int start = 0 , end = 0;

	for( ; i < num-m ; ++i)
	{
		double temp = 0;
		for(int j = -m ; j <= m ; ++j)
		{
			temp += C(j)*(dataNew[i+j].Y - y);
		}

		temp = sqrt(abs(temp));

		if(temp > f)
		{
			_sntprintf_s(szBuffer, 32, TEXT("%.3lf  %.3lf\n"), dataNew[i].X, dataNew[i].Y);    
			MyWriteFile(1,szBuffer, szCombinedPeakName);
			memset(szBuffer, 0, sizeof(TCHAR) * 32);

			if(end == 0)
			{
				start = i;
				++end;
			}

		}
		else
		{
			if(start != 0)
			{
				end = i;
				RecordInfo( Head , Rear , start , end-1 );
				end = 0;
				start = 0;
				CombinedThreshold = dataNew[i].Y;
			}

			break;
		}
	}
	//threshold = dataNew[i].Y;
	return;
}

int CombinedAmplitude(fileInfo *dataNew, int num)
{
	if( dataNew == NULL )
	{
		return -1;
		//需要先进行光滑处理
	}

	GenerateFileName( szCombinedPeakName );

//限幅
	TCHAR szBuffer[32]={0};
	int start = 0 , end = 0;
//

	int i = 0;
	CombinedThreshold = dataNew[0].Y;

	for(; i<num-1 ; ++i)
	{
		double temp = 0.0;
		temp = dataNew[i+1].Y - CombinedThreshold ;
		if(abs(temp) > CombinedDeviation)
		{
			CombinedSR(dataNew , num , i );
		}
		else
		{
			y = dataNew[i].Y;
		}
	}

	return 0;
}