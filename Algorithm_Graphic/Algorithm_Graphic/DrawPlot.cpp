#include "include.h"

double minX = 0.0;
double maxX = 0.0;
double minY = 0.0;
double maxY = 0.0;

double kx,ky;//����ϵ��
RECT range;

BOOL DrawMoveTo( HDC hdc,double x, double y)
{
	return MoveToEx(hdc, LogicX2WindowX(x), LogicY2WindowY(y), NULL);
}

BOOL DrawLine(HDC hdc, int nX, int nY)
{
	return LineTo(hdc,nX,nY);
}

void DrawPolyLine(HWND hwnd ,int lineNum, FileInfo data, HPEN hPen  )
{
	int i = 0;
	double nX = 0.0 ,nY = 0.0;
	RECT temprange = range;
	HDC hdc;
	hdc = GetDC (hwnd) ;               //��ô��ھ��
	SaveDC(hdc);                       //����ԭ�����豸���

	SelectObject(hdc , hPen);//ѡ�񻭱�
	
	SetMapMode (hdc, MM_ANISOTROPIC) ;
	SetWindowExtEx (hdc, SCALE_HEIGHT_UNIT * 20, SCALE_HEIGHT_UNIT * -20, NULL) ;   //�߼�������
	SetViewportExtEx (hdc, GetDeviceCaps (hdc, LOGPIXELSX),GetDeviceCaps (hdc, LOGPIXELSY), NULL) ;  //�豸������
	SetViewportOrgEx (hdc, temprange.left , temprange.bottom , NULL) ;   //�豸����ԭ�������½�
	DPtoLP (hdc, (PPOINT)&temprange, 2);         //���豸����ת��Ϊ�߼�����
	
	kx = (temprange.right - temprange.left) /(maxX - minX);
	ky = (temprange.top - temprange.bottom) /(maxY - minY);

	DrawMoveTo(hdc,data[0].X,data[0].Y);          //�ƶ�����ʼ�㣬Ȼ����ʼ��ת��Ϊwindows����
	for (i=0; i<lineNum; ++i)
	{
		nX = LogicX2WindowX(data[i].X);           //�����������е�����ת��Ϊwindows����
		nY = LogicY2WindowY(data[i].Y);
		DrawLine(hdc,nX,nY);                         
	}
	
	RestoreDC (hdc, -1) ;           //�ָ�ԭ�����豸���
	ReleaseDC (hwnd, hdc);          //�ͷŴ��ھ��

}

void DrawDot(HWND hwnd ,int lineNum, FileInfo data, HPEN hPen  )
{
	int i = 0;
	double nX = 0.0 ,nY = 0.0;
	RECT rectTemp;
	RECT temprange = range;
	HDC hdc;
	hdc = GetDC (hwnd) ;               //��ô��ھ��
	SaveDC(hdc);                            //����ԭ�����豸���

	SelectObject(hdc , hPen);//ѡ�񻭱�
	
	SetMapMode (hdc, MM_ANISOTROPIC) ;
	SetWindowExtEx (hdc, SCALE_HEIGHT_UNIT * 20, SCALE_HEIGHT_UNIT * -20, NULL) ;   //�߼�������
	SetViewportExtEx (hdc, GetDeviceCaps (hdc, LOGPIXELSX),GetDeviceCaps (hdc, LOGPIXELSY), NULL) ;  //�豸������
	SetViewportOrgEx (hdc, temprange.left , temprange.bottom , NULL) ;   //�豸����ԭ�������½�
	DPtoLP (hdc, (PPOINT)&temprange, 2);         //���豸����ת��Ϊ�߼�����
	
	kx = (temprange.right - temprange.left) /(maxX - minX);
	ky = (temprange.top - temprange.bottom) /(maxY - minY);

	DrawMoveTo(hdc,data[0].X,data[0].Y);          //�ƶ�����ʼ�㣬Ȼ����ʼ��ת��Ϊwindows����
	for (i=0; i<lineNum; ++i)
	{
		nX = LogicX2WindowX(data[i].X);            //�����������е�����ת��Ϊwindows����
		nY = LogicY2WindowY(data[i].Y);

		rectTemp.left  = nX - 50 ;
		rectTemp.right =  nX + 50 ;
		rectTemp.top  = nY + 50 ;
		rectTemp.bottom  =  nY - 50 ;

		Ellipse(hdc, rectTemp.left, rectTemp.top, rectTemp.right, rectTemp.bottom);
	}
	
	RestoreDC (hdc, -1) ;               //�ָ�ԭ�����豸���
	ReleaseDC (hwnd, hdc);          //�ͷŴ��ھ��

}

int LogicX2WindowX(double x)
{
	return (int)( (x - minX)*kx)  ;
}

int LogicY2WindowY(double y)
{
	return (int)( (y - minY)*ky) ;
}

void GetDataRange(double &minX , double &maxX , double &minY , double &maxY ,FileInfo fileData)
{
	minX = fileData[0].X;
	maxX = fileData[0].X;
	minY = fileData[0].Y;
	maxY = fileData[0].Y;

	for(int i = 0 ; i<lineNum ; ++i)
	{
		if(fileData[i].X<minX)
			minX = fileData[i].X;

		if(fileData[i].X>maxX)
			maxX = fileData[i].X;

		if(fileData[i].Y<minY)
			minY = fileData[i].Y;

		if(fileData[i].Y>maxY)
			maxY = fileData[i].Y;

	}
}