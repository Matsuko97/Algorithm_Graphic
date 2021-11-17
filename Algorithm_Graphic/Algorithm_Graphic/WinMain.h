#ifndef _WINMAIN_H_
#define _WINMAIN_H_

#define bkColor		RGB(220,220,220)

typedef struct{
	double X;					
	double Y;					
}fileInfo, *FileInfo;

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK PlotWndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);
INT_PTR CALLBACK DlgProc(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam);

extern TCHAR szFileName[MAX_PATH];
extern HWND hwnd;
extern HINSTANCE hInst;

extern FileInfo fileData;
extern FileInfo fileDataSmooth;

extern int  cxChar, cyChar ;
extern int OriginalNum;
extern int SmoothNum;
extern double Factor;

#endif