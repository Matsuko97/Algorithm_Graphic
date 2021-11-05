#include "include.h"

TCHAR szSNIPBase[128]={0};

/*
	功能：BasicSNIP算法

	参数：
		*dataNew ：输入原始数据；
		num ：输入数据总个数；
		m：输入迭代次数，m一般取半峰宽；
		i：当前数据的道址；
		type：峰的类型，true = 波峰，false = 波谷；
*/
void BasicSNIP( fileInfo *dataNew, int num , int m , int i , bool type)
{
	TCHAR szBuffer[32]={0};

	double y = 0;
	double yPlusm = 0;
	double yMinusm = 0;

	//对数据进行LLS变换
	y = log(log(sqrt(abs(dataNew[i].Y) + 1) + 1) + 1);

	double yPlusOriginal = 0;
	double yMinusOriginal = 0;

	for(int j = 1 ; j <= m ; ++j)
	{
		//防止峰出现得靠前，使得没有数据用于计算
		if(i-j<0)
		{
			yMinusOriginal = dataNew[0].Y;
		}
		else
		{
			yMinusOriginal = dataNew[i-j].Y;
		}

		//防止峰出现得靠后，没有数据用于计算
		if(i+j>=num)
		{
			yPlusOriginal = dataNew[num-1].Y;
		}
		else
		{
			yPlusOriginal = dataNew[i+j].Y;
		}

		//对数据进行LLS变换
		yPlusm = log(log(sqrt(abs(yPlusOriginal) + 1) + 1) + 1);
		yMinusm = log(log(sqrt(abs(yMinusOriginal) + 1) + 1) + 1);

		//SNIP算法逻辑
		if(type)//出现波峰，按照SNIP算法判断计算
		{
			y = y<(( yPlusm + yMinusm )/2) ? y : ( yPlusm + yMinusm )/2;
		}
		else//出现波谷，需要按照相反逻辑判断
		{
			y = y>(( yPlusm + yMinusm )/2) ? y : ( yPlusm + yMinusm )/2;
		}
	}

	//对处理后的本底谱数据进行反LLS变换
	y = pow((exp(exp( y ) - 1) - 1),2) - 1 ;

	///////
	_sntprintf_s(szBuffer, 32, TEXT("%.3lf  %.3lf\n"), dataNew[i].X, y);    
	MyWriteFile(1,szBuffer, szSNIPBase);
	memset(szBuffer, 0, sizeof(TCHAR) * 32);
	///////以上语句在改进的SNIP算法中用于输出数据信息

	return;
}

void ImprovedSNIP(fileInfo *dataNew, int num , LinkList Head)
{
	TCHAR szBuffer[32]={0};
	LinkList p = Head;
	int start = 0;
	int end = 0;
	int width = 0;
	bool flag = true;//判断当前是否需要获取峰的信息

	for( int i = 0 ; i<num ;++i)
	{
		if(flag)
		{
			ObtainInfo( p , start , end , width , num );
			flag = false;
		}
		else
		{
		}

		if( i>= start && i<end )
		{
			BasicSNIP(dataNew,num,width,i,false);
		}
		else if( i == end )
		{
			flag = true;
			//需要重新获取下一个峰的信息，对SNIP算法窗口宽度进行调整

			_sntprintf_s(szBuffer, 32, TEXT("%.3lf  %.3lf\n"), dataNew[i].X, dataNew[i].Y);    
			MyWriteFile(1,szBuffer, szSNIPBase);
			memset(szBuffer, 0, sizeof(TCHAR) * 32);
		}
		else
		{
			BasicSNIP( dataNew, num, FixedWidth, i, false);
			//_sntprintf_s(szBuffer, 32, TEXT("%.3lf  %.3lf\n"), dataNew->Time[i], dataNew->Pressure[i]);    
			//MyWriteFile(1,szBuffer, szSNIPBase);
			//memset(szBuffer, 0, sizeof(TCHAR) * 32);
		}
	}

	//----------注释掉上面的所有语句，只写下面的for循环，代表以固定宽度处理所有数据---------//
	//for( int i = 0 ; i<num ;++i )
	//{
	//	BasicSNIP( dataNew, num, /*width*/ 31, i, false);
	//}//固定宽度处理所有数据

	return;
}

void ObtainInfo(LinkList &p , int &start , int &end , int &width , int num)
{
	if(p)
	{
		start = p->indexStart;
		end = p->indexEnd;
		width = p->width;
		p = p->next;
	}
	else
	{
		start = 0;
		end = num ;
		width = 0 ;
	}
	return;
}