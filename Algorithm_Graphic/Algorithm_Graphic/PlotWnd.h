#ifndef _PLOTWND_H_
#define _PLOTWND_H_

class PlotWnd
{
public:
	PlotWnd();
	~PlotWnd();

//绘图窗口相关
public:
	HWND hwndPlot;

public:
	void CreatePlotWindow(HWND hwnd , RECT rectPlot);
	LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);              

public:
	BOOL dataReady;
	LineList Head;
	LineList Rear;

	void RecordLineList( LineList &LineHead , LineList &LineRear , Line &curline );
	void DeleteAll( LineList &LineHead , LineList &LineRear );

//画图相关
private:
	COLORREF CurrentColor;
	int CurWidth;
	int PenStyle;

public:
	void SetCurrentColor(COLORREF newColor)					//修改当前画笔颜色
	{
		CurrentColor = newColor;
	}

	void SetCurWidth(int newWidth)					//修改当前画笔宽度
	{
		CurWidth = newWidth;
	}

	void SetPenStyle(int newStyle)
	{
		PenStyle = newStyle;
	}

};

extern PlotWnd plotWnd;

#endif