#ifndef _DRAWSCALELABEL_H_
#define _DRAWSCALELABEL_H_

void  DrawLabelHor(HWND hwnd, RECT *rect, int nXLabelNum, double dXStartValue, double dXEndValue);
void  DrawLabelVer(HWND hwnd, RECT *rect, int nYLabelNum, double dYStartValue, double dYEndValue);
void DrawHScaleLabel ( HWND hwnd, RECT * rect, double dStartValue, double dEndValue);
void DrawVScaleLabel ( HWND hwnd, RECT * rect, double dStartValue, double dEndValue);

typedef struct{
    TCHAR  str[20];
}MYCHAR;

#endif