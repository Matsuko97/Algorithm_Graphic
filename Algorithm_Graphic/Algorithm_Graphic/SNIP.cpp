#include "include.h"

TCHAR szSNIPBase[128]={0};

/*
	���ܣ�BasicSNIP�㷨

	������
		*dataNew ������ԭʼ���ݣ�
		num �����������ܸ�����
		m���������������mһ��ȡ����
		i����ǰ���ݵĵ�ַ��
		type��������ͣ�true = ���壬false = ���ȣ�
*/
void BasicSNIP( fileInfo *dataNew, int num , int m , int i , bool type)
{
	TCHAR szBuffer[32]={0};

	double y = 0;
	double yPlusm = 0;
	double yMinusm = 0;

	//�����ݽ���LLS�任
	y = log(log(sqrt(abs(dataNew[i].Y) + 1) + 1) + 1);

	double yPlusOriginal = 0;
	double yMinusOriginal = 0;

	for(int j = 1 ; j <= m ; ++j)
	{
		//��ֹ����ֵÿ�ǰ��ʹ��û���������ڼ���
		if(i-j<0)
		{
			yMinusOriginal = dataNew[0].Y;
		}
		else
		{
			yMinusOriginal = dataNew[i-j].Y;
		}

		//��ֹ����ֵÿ���û���������ڼ���
		if(i+j>=num)
		{
			yPlusOriginal = dataNew[num-1].Y;
		}
		else
		{
			yPlusOriginal = dataNew[i+j].Y;
		}

		//�����ݽ���LLS�任
		yPlusm = log(log(sqrt(abs(yPlusOriginal) + 1) + 1) + 1);
		yMinusm = log(log(sqrt(abs(yMinusOriginal) + 1) + 1) + 1);

		//SNIP�㷨�߼�
		if(type)//���ֲ��壬����SNIP�㷨�жϼ���
		{
			y = y<(( yPlusm + yMinusm )/2) ? y : ( yPlusm + yMinusm )/2;
		}
		else//���ֲ��ȣ���Ҫ�����෴�߼��ж�
		{
			y = y>(( yPlusm + yMinusm )/2) ? y : ( yPlusm + yMinusm )/2;
		}
	}

	//�Դ����ı��������ݽ��з�LLS�任
	y = pow((exp(exp( y ) - 1) - 1),2) - 1 ;

	///////
	_sntprintf_s(szBuffer, 32, TEXT("%.3lf  %.3lf\n"), dataNew[i].X, y);    
	MyWriteFile(1,szBuffer, szSNIPBase);
	memset(szBuffer, 0, sizeof(TCHAR) * 32);
	///////��������ڸĽ���SNIP�㷨���������������Ϣ

	return;
}

void ImprovedSNIP(fileInfo *dataNew, int num , LinkList Head)
{
	TCHAR szBuffer[32]={0};
	LinkList p = Head;
	int start = 0;
	int end = 0;
	int width = 0;
	bool flag = true;//�жϵ�ǰ�Ƿ���Ҫ��ȡ�����Ϣ

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
			//��Ҫ���»�ȡ��һ�������Ϣ����SNIP�㷨���ڿ�Ƚ��е���

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

	//----------ע�͵������������䣬ֻд�����forѭ���������Թ̶���ȴ�����������---------//
	//for( int i = 0 ; i<num ;++i )
	//{
	//	BasicSNIP( dataNew, num, /*width*/ 31, i, false);
	//}//�̶���ȴ�����������

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