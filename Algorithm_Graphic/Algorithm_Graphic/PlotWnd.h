#ifndef _PLOTWND_H_
#define _PLOTWND_H_

class PlotWnd
{
public:
	PlotWnd();
	~PlotWnd();

//��ͼ�������
public:
	HWND hwndPlot;
	HCURSOR hcursor;

public:
	void CreatePlotWindow(HWND hwnd , RECT rectPlot);
	LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam);              

//�������
public:
	BOOL dataReady;
	LineList Head;
	LineList Rear;

	void RecordLineList( LineList &LineHead , LineList &LineRear , Line &curline );
	void DeleteAll( LineList &LineHead , LineList &LineRear );

public:
	enum { NOT_START, BIG, SMALL, SELECT } function;

//��ͼ���
private:
	COLORREF CurrentColor;
	int CurWidth;
	int PenStyle;

public:
	void SetCurrentColor(COLORREF newColor)			//�޸ĵ�ǰ������ɫ
	{
		CurrentColor = newColor;
	}

	void SetCurWidth(int newWidth)					//�޸ĵ�ǰ���ʿ��
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