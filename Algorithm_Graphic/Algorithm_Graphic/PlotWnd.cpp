#include "include.h"

PlotWnd plotWnd;

PlotWnd::PlotWnd()
{
	dataReady = false;

	function = NOT_START;

	CurrentColor = RGB(0,0,0);
	CurWidth = 1;
	PenStyle = PS_SOLID;
}

PlotWnd::~PlotWnd()
{
}

void PlotWnd::RecordLineList( LineList &LineHead , LineList &LineRear , Line &curline )
{
	Line tempLine = curline;

	if(Head==NULL)
	{
		LineHead = (LineList)malloc(sizeof(LineNode));
		LineHead->line = tempLine;
		LineHead->next = NULL;

		LineRear = LineHead;
	}
	else
	{
		LineList temp = (LineList)malloc(sizeof(LineNode));
		temp->line = tempLine;
		temp->next = NULL;

		LineRear->next = temp;
		LineRear=temp;
	}

	return;
}

void PlotWnd::DeleteAll( LineList &LineHead , LineList &LineRear )
{
	while( LineHead )
	{
		LineHead = LineHead->next ;	
	}

	LineRear = LineHead; 

	return;
}

void PlotWnd::CreatePlotWindow(HWND hwnd , RECT rectPlot)
{
	TCHAR szPlotChildClass[] = TEXT("Plot_ChildWnd");
	WNDCLASS wc;

	wc.style = CS_HREDRAW|CS_VREDRAW|CS_NOCLOSE|CS_DBLCLKS;
	wc.lpfnWndProc = PlotWndProc;                                                  
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);                              
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szPlotChildClass;

	if (RegisterClass(&wc) == 0)
	{
		//To support multi class, pass OK!
	}

	hwndPlot = CreateWindowEx(WS_EX_CLIENTEDGE,                                    
		szPlotChildClass,                                          
		NULL, 
		WS_CHILD|WS_VISIBLE|WS_BORDER,                        
		rectPlot.left,rectPlot.top,rectPlot.right-rectPlot.left,rectPlot.bottom,
		hwnd,
		(HMENU)ID_PLOTWINDOW, hInst, NULL
		);

}

LRESULT CALLBACK PlotWnd::WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)               
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	switch(message)
	{
	case WM_PAINT:
		{
			hdc = BeginPaint (hwnd, &ps) ;

			if( plotWnd.dataReady )
			{
				GetClientRect (hwnd, &rect) ;
				rect.left = rect.left + 30;
				rect.bottom = rect.bottom -  1.5 * cyChar;

				range.left = rect.left;
				range.right = rect.right;
				range.top = rect.top;
				range.bottom = rect.bottom;//画图区

				DrawRuler (hwnd, rect);

				//画水平标签
				GetClientRect (hwnd, &rect) ;
				rect.left = rect.left + 30 - cxChar * 1.5;
				rect.top = rect.bottom - cyChar;
				DrawHScaleLabel ( hwnd, rect, minX, maxX );

				//画垂直标签
				GetClientRect (hwnd, &rect) ;
				rect.right = rect.left + 25;
				rect.bottom = rect.bottom - 1 * cyChar;
				DrawVScaleLabel ( hwnd, rect, minY, maxY );

				LineList temp = (LineList)malloc(sizeof(Node));
				temp = Head;
				while(temp)
				{
					HPEN hPen , hOldPen ;
					hPen = CreatePen((temp->line).PenStyle,(temp->line).LineWidth,(temp->line).LineColor);
					hOldPen = (HPEN)SelectObject( hdc , hPen);
					if((temp->line).type ==  CURVE)
					{
						DrawPolyLine( hwnd , (temp->line).PointNum , (temp->line).data , hPen );
					}
					else if((temp->line).type == DOT)
					{
						DrawDot( hwnd , (temp->line).PointNum , (temp->line).data , hPen);
					}
					SelectObject(hdc, hOldPen);
					DeleteObject(hPen);

					temp = temp->next;
				}
				free(temp);
			}
			EndPaint (hwnd, &ps) ;
			return 0;
		}

	case WM_LBUTTONDBLCLK:
		{
			POINT pt = { 0 };
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			
			if( function == BIG )
			{
				RECT rect;
				GetClientRect(hwnd,&rect);
				int cxClient=rect.right;
				int cyClient=rect.bottom;
				HDC hdc= GetDC(hwnd);
				SetStretchBltMode(hdc,COLORONCOLOR);
				StretchBlt (hdc, 0, 0, cxClient*1.5, cyClient*1.5,
					hdc, pt.x, pt.y, cxClient, cyClient, MERGECOPY) ;
				ReleaseDC(hwnd, hdc) ;
			}

			else if( function == SMALL )
			{
				RECT rect;
				GetClientRect(hwnd,&rect);
				int cxClient=rect.right;
				int cyClient=rect.bottom;
				HDC hdcClient = GetDC(hwnd);
				HDC hdcMem = CreateCompatibleDC (hdcClient);
				HBITMAP hMem = CreateCompatibleBitmap(hdcClient, 1024, 768);
				SelectObject(hdcMem, hMem);
				BitBlt(hdcMem, 0, 0, cxClient, cyClient, hdcClient, 0, 0, SRCCOPY);
				PatBlt(hdcClient, 0, 0, 1024, 768, WHITENESS);
				StretchBlt (hdcClient, 0, 0, cxClient/1.5, cyClient/1.5,
					hdcMem, 0, 0, cxClient, cyClient, SRCCOPY);
				ReleaseDC(hwnd, hdcClient) ;
				DeleteDC (hdcMem) ;
				DeleteObject (hMem) ;
			}

			else
			{}
		}
		return 0;

	case WM_RBUTTONDOWN:
		function = NOT_START;
		return 0;

	case WM_MOUSEMOVE:
		if( function == BIG || function == SMALL )
		{
			hcursor = LoadCursor( NULL, IDC_CROSS );
			SetCursor(hcursor);
		}
		return 0;

	case IDM_BIG:
		function = BIG;
		return 0;

	case IDM_SMALL:
		function = SMALL;
		return 0;

	default:
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
