#include "include.h"

double minX = 0.0;
double maxX = 0.0;
double minY = 0.0;
double maxY = 0.0;

double kx,ky;//比例系数
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
	hdc = GetDC (hwnd) ;               //获得窗口句柄
	SaveDC(hdc);                       //保存原来的设备句柄
	
	HPEN hOldPen;
	hOldPen = (HPEN)SelectObject(hdc , hPen);//选择画笔
	
	SetMapMode (hdc, MM_ANISOTROPIC) ;
	SetWindowExtEx (hdc, SCALE_HEIGHT_UNIT * 20, SCALE_HEIGHT_UNIT * -20, NULL) ;   //逻辑坐标间距
	SetViewportExtEx (hdc, GetDeviceCaps (hdc, LOGPIXELSX),GetDeviceCaps (hdc, LOGPIXELSY), NULL) ;  //设备坐标间距
	SetViewportOrgEx (hdc, temprange.left , temprange.bottom , NULL) ;   //设备坐标原点在左下角
	DPtoLP (hdc, (PPOINT)&temprange, 2);         //将设备坐标转化为逻辑坐标
	
	kx = (temprange.right - temprange.left) /(maxX - minX);
	ky = (temprange.top - temprange.bottom) /(maxY - minY);

	DrawMoveTo(hdc,data[0].X,data[0].Y);          //移动到起始点，然后将起始点转化为windows坐标
	for (i=0; i<lineNum; ++i)
	{
		nX = LogicX2WindowX(data[i].X);           //将待画数组中的数据转化为windows坐标
		nY = LogicY2WindowY(data[i].Y);
		DrawLine(hdc, nX, nY);                         
	}
	
	SelectObject(hdc , hOldPen);
	RestoreDC (hdc, -1) ;           //恢复原来的设备句柄
	ReleaseDC (hwnd, hdc);          //释放窗口句柄

}

void DrawDot(HWND hwnd ,int lineNum, FileInfo data, HPEN hPen  )
{
	int i = 0;
	double nX = 0.0 ,nY = 0.0;
	RECT rectTemp;
	RECT temprange = range;
	HDC hdc;
	hdc = GetDC (hwnd) ;               //获得窗口句柄
	SaveDC(hdc);                            //保存原来的设备句柄

	HPEN hOldPen;
	hOldPen = (HPEN)SelectObject(hdc , hPen);//选择画笔
	
	SetMapMode (hdc, MM_ANISOTROPIC) ;
	SetWindowExtEx (hdc, SCALE_HEIGHT_UNIT * 20, SCALE_HEIGHT_UNIT * -20, NULL) ;   //逻辑坐标间距
	SetViewportExtEx (hdc, GetDeviceCaps (hdc, LOGPIXELSX),GetDeviceCaps (hdc, LOGPIXELSY), NULL) ;  //设备坐标间距
	SetViewportOrgEx (hdc, temprange.left , temprange.bottom , NULL) ;   //设备坐标原点在左下角
	DPtoLP (hdc, (PPOINT)&temprange, 2);         //将设备坐标转化为逻辑坐标
	
	kx = (temprange.right - temprange.left) /(maxX - minX);
	ky = (temprange.top - temprange.bottom) /(maxY - minY);

	DrawMoveTo(hdc,data[0].X,data[0].Y);          //移动到起始点，然后将起始点转化为windows坐标
	for (i=0; i<lineNum; ++i)
	{
		nX = LogicX2WindowX(data[i].X);            //将待画数组中的数据转化为windows坐标
		nY = LogicY2WindowY(data[i].Y);

		rectTemp.left  = nX - 50 ;
		rectTemp.right =  nX + 50 ;
		rectTemp.top  = nY + 50 ;
		rectTemp.bottom  =  nY - 50 ;

		Ellipse(hdc, rectTemp.left, rectTemp.top, rectTemp.right, rectTemp.bottom);
	}
	
	SelectObject(hdc , hOldPen);
	RestoreDC (hdc, -1) ;               //恢复原来的设备句柄
	ReleaseDC (hwnd, hdc);          //释放窗口句柄

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

	int n = 0;
	n = CalcExponent( maxX - minX ) - 1;
	maxX = maxX + pow(10.0, n);
	minX = minX - pow(10.0, n);

	n = CalcExponent( maxY - minY ) - 1;
	maxY = maxY + pow(10.0, n);
	minY = minY - pow(10.0, n);
	//根据 max 和 min 的差值，适当扩大坐标轴范围

	return;
}

int CalcExponent(double num)
{
	int n = 0;
	if(num < 1)
	{
		while (num < 1)
		{
			num *= 10;
			n--;
		}
	}
	else
	{
		while (num > 10)
		{
			num /= 10;
			n++;
		}
	}
	return n;
}