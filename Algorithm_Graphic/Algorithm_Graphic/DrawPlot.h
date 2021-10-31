#ifndef _DRAWPLOT_H_
#define _DRAWPLOT_H_

extern double minX ;
extern double maxX ;
extern double minY ;
extern double maxY ;

extern RECT range;
extern double LEFT_EDGE ;
extern double BOTTOM_EDGE ;

void GetDataRange(double &minX , double &maxX , double &minY , double &maxY ,FileInfo fileData);

BOOL DrawMoveTo( HDC hdc,double x, double y);
void DrawPolyLine(HWND hwnd ,int lineNum, FileInfo data, HPEN hPen );
BOOL DrawLine(HDC hdc, int nX, int nY);
void DrawDot(HWND hwnd ,int lineNum, FileInfo data, HPEN hPen  );

int CalcExponent(double num);
int LogicX2WindowX(double x);
int LogicY2WindowY(double y);

#endif