#include "include.h"

TCHAR szFileName[100]={0};
static OPENFILENAME ofn = { 0 };                //���ļ�����ṹ��
static TCHAR szTitleName[MAX_PATH];
static TCHAR szFilter[] = TEXT("�ı��ļ� (*.txt)\0*.txt\0�����ļ� (*.*)\0*.*\0\0");

HWND hwnd;
HWND hwndPlot;
HWND hwndFunction;
HWND hSetDlg;

HINSTANCE hInst;
int  cxChar, cyChar ;

FileInfo fileData;
FileInfo fileDataSmooth;

BOOL beFaund = false;

int OriginalNum = 0;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	static TCHAR szAppName[]=TEXT("TEST");
	MSG msg;
	WNDCLASS wndclass;
	static int cxChar,cyChar;

	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (LTGRAY_BRUSH) ;
	wndclass.lpszMenuName  = MAKEINTRESOURCE(IDR_TESTMENU) ;
	wndclass.lpszClassName = szAppName ;

	if ( !RegisterClass(&wndclass))
	{
		MessageBox(NULL,TEXT ("This program requires Windows NT!"), 
			szAppName, MB_ICONERROR) ;
		return 0 ;
	}

	hInst = hInstance;
	hwnd = CreateWindow (
		szAppName,                 
		TEXT ("TEST"),		  
		WS_OVERLAPPEDWINDOW,//  100 ,100 ,1000,600,      
		CW_USEDEFAULT,              
		CW_USEDEFAULT,              
		CW_USEDEFAULT,             
		CW_USEDEFAULT,              
		NULL,                      
		NULL,                       
		hInstance,                  
		NULL) ;

	ShowWindow(hwnd,nShowCmd);
	UpdateWindow(hwnd);

	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK PlotWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return plotWnd.WndProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	Line temp;
	LineList p;

	switch( message )
	{
	case WM_CREATE:
		hdc=GetDC(hwnd);

		cxChar = LOWORD (GetDialogBaseUnits ()) ;
		cyChar = HIWORD (GetDialogBaseUnits ()) ;

		GetClientRect(hwnd,&rect);
		rect.left = rect.right/4;

		plotWnd.CreatePlotWindow(hwnd , rect);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("ԭʼ����"),
			WS_CHILD|WS_VISIBLE,10,100,200,30,
			hwnd, (HMENU)IDB_GETINFO, NULL,NULL);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("�⻬����"),
			WS_CHILD|WS_VISIBLE,10,150,200,30,
			hwnd, (HMENU)IDB_SMOOTH, NULL,NULL);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("�Գ����������˹"),
			WS_CHILD|WS_VISIBLE,10,200,200,30,
			hwnd, (HMENU)IDB_GAUSSIAN, NULL,NULL);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("�Գ������������"),
			WS_CHILD|WS_VISIBLE,10,250,200,30,
			hwnd, (HMENU)IDB_SQUARE_WAVE, NULL,NULL);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("��ֵѰ��"),
			WS_CHILD|WS_VISIBLE,10,300,200,30,
			hwnd, (HMENU)IDB_AMPLITUDE, NULL,NULL);

		ReleaseDC(hwnd,hdc);

		return 0;

	case WM_PAINT:
		hdc=BeginPaint(hwnd,&ps);

		EndPaint(hwnd,&ps);
		return 0;

	case WM_COMMAND:
		{
			int wmID = LOWORD(wParam);
			switch (wmID)
			{
			case IDM_FILE_NEW:
				break;

			case IDM_FILE_OPEN:
				plotWnd.dataReady = false;
				memset( szFileName , 0 , sizeof(TCHAR) * 100 );
				memset( szTitleName , 0 , sizeof(TCHAR) * MAX_PATH);
				memset( &ofn , 0 , sizeof(OPENFILENAME));

				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = hwnd;
				ofn.lpstrFilter = szFilter;             //����������ʾʲô��ʽ���ļ�
				ofn.nFilterIndex = 2;
				ofn.lpstrFile = szFileName;      //�ļ���+������·��
				ofn.nMaxFile = MAX_PATH;           //ofn.lpstrFile�ַ�������ж೤
				ofn.lpstrFileTitle = szTitleName;     //ֻ���ļ���
				ofn.nMaxFileTitle = MAX_PATH;
				ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
				ofn.lpstrDefExt = TEXT("txt");

				if (GetOpenFileName(&ofn)) 
				{
					fileData = GetData( szFileName );
					if(fileData)
					{
						MessageBox( NULL, TEXT("���ݻ�ȡ�ɹ�"), TEXT("���ݴ���"), MB_OK );
						OriginalNum = GetLineNum( szFileName );
						plotWnd.dataReady = true;
						GetDataRange(minX , maxX, minY , maxY ,fileData);

						SendMessage( plotWnd.hwndPlot , WM_PAINT , wParam , lParam );
					}
					else
					{
						MessageBox( NULL, TEXT("���ݻ�ȡʧ��"), TEXT("���ݴ���"), MB_OK );
					}
				}

				break;

			case IDB_GETINFO:
				{
					p = plotWnd.Head;
					while(p) {
						if((p->line).LineName == (p->line).ORIGIN) {
							beFaund = true;
						}
						else {
							p = p->next;
						}
					}

					if(beFaund){
						beFaund = false;
					}
					else{
						Line origin;
						origin.data = fileData;
						origin.type = CURVE;
						origin.LineName = origin.ORIGIN;
						origin.PointNum = OriginalNum;
						temp = origin;
						plotWnd.RecordLineList( plotWnd.Head , plotWnd.Rear , temp );
					}
					SendMessage( plotWnd.hwndPlot , WM_PAINT , wParam , lParam );
				}
				break;

			case IDB_SMOOTH:
				{
					p = plotWnd.Head;
					while(p) {
						if((p->line).LineName == (p->line).SMOOTH) {
							beFaund = true;
						}
						else {
							p = p->next;
						}
					}

					if(beFaund){
						beFaund = false;
					}
					else{
						lineNum = OriginalNum;
						Smooth(fileData , lineNum ,fileDataSmooth);
						GenerateFileName( szFileNameSmooth );
						WriteFileInfo( lineNum , fileDataSmooth , szFileNameSmooth );

						Line smooth;
						smooth.data = fileDataSmooth;
						smooth.PointNum = lineNum;
						smooth.LineColor = RGB(255,0,0);
						smooth.PenStyle = PS_DASH;
						smooth.type = CURVE;
						smooth.LineName = smooth.SMOOTH;
						temp = smooth;
						plotWnd.RecordLineList( plotWnd.Head , plotWnd.Rear , temp);
					}
					SendMessage(plotWnd.hwndPlot , WM_PAINT , wParam ,lParam);
				}
				break;

			case IDB_GAUSSIAN:
				{
					p = plotWnd.Head;
					while(p) {
						if((p->line).LineName == (p->line).SZA_G) {
							beFaund = true;
						}
						else {
							p = p->next;
						}
					}

					if(beFaund){
						beFaund = false;
					}
					else{
						SR( fileDataSmooth , lineNum );

						Line gaussian;
						gaussian.data = GetData(szSPeakName);
						gaussian.PointNum = lineNum;
						gaussian.LineColor = RGB(0 , 200, 200);
						gaussian.type = DOT;
						gaussian.LineName = gaussian.SZA_G;
						temp = gaussian;
						plotWnd.RecordLineList( plotWnd.Head , plotWnd.Rear , temp);
					}
					SendMessage(plotWnd.hwndPlot , WM_PAINT , wParam ,lParam);
				}
				break;

			case IDB_AMPLITUDE:
				{
					p = plotWnd.Head;
					while(p) {
						if((p->line).LineName == (p->line).AMPLITUDE) {
							beFaund = true;
						}
						else {
							p = p->next;
						}
					}

					if(beFaund){
						beFaund = false;
					}
					else{
						Amplitude( fileDataSmooth , lineNum );

						Line amplitude;
						amplitude.data = GetData(szAmpName);
						amplitude.PointNum = lineNum;
						amplitude.LineColor = RGB(0,150,0);
						amplitude.type = DOT;
						amplitude.LineName = amplitude.AMPLITUDE;
						temp = amplitude;
						plotWnd.RecordLineList( plotWnd.Head , plotWnd.Rear , temp);
					}
					SendMessage(plotWnd.hwndPlot , WM_PAINT , wParam ,lParam);
				}
				break;

			case IDM_SETTING:
				hSetDlg = CreateDialog( hInst, MAKEINTRESOURCE(IDD_SETDLG), hwnd, (DLGPROC)DlgProc);
				ShowWindow( hSetDlg, SW_NORMAL );

				break;

			case IDM_BIG:
				SendMessage( plotWnd.hwndPlot, IDM_BIG, wParam, lParam);
				break;

			case IDM_SMALL:
				SendMessage( plotWnd.hwndPlot, IDM_SMALL, wParam, lParam);
				break;

			case IDM_ORIGIN:
				InvalidateRect(plotWnd.hwndPlot,NULL,TRUE);

				break;

			case IDM_DELETE:
				plotWnd.DeleteAll( plotWnd.Head , plotWnd.Rear );
				SendMessage(plotWnd.hwndPlot , WM_PAINT , wParam ,lParam);

				break;
			}
		}
		return 0;

	case WM_RBUTTONUP:
		{
			HMENU hPopup = CreatePopupMenu();
			AppendMenu(hPopup, MF_STRING, IDM_SETTING, TEXT("Setting"));
			AppendMenu(hPopup, MF_STRING, IDM_BIG, TEXT("Big"));
			AppendMenu(hPopup, MF_STRING, IDM_ORIGIN, TEXT("Original"));
			AppendMenu(hPopup, MF_STRING, IDM_SMALL, TEXT("Small"));
			AppendMenu(hPopup, MF_STRING, IDM_DELETE, TEXT("Delete"));
			RECT rect;
			GetWindowRect(hwnd,&rect);
			POINT pt = { 0 };
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);
			ClientToScreen(hwnd, &pt);
			TrackPopupMenu(hPopup, TPM_LEFTALIGN|TPM_TOPALIGN,pt.x,pt.y, 0, hwnd, NULL);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}

INT_PTR CALLBACK DlgProc(HWND hdlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:

		return 0;

	case WM_SYSCOMMAND:
		if(wParam == SC_CLOSE)
		{
			DestroyWindow(hdlg);
		}
		return 0;

	case WM_CLOSE:
		DestroyWindow(hdlg);
		return 0;
	}
	return (INT_PTR)FALSE;
}