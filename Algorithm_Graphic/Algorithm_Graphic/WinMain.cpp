#include "include.h"

TCHAR szFileName[100]={0};
static OPENFILENAME ofn = { 0 };                //打开文件所需结构体
static TCHAR szTitleName[MAX_PATH];
static TCHAR szFilter[] = TEXT("文本文件 (*.txt)\0*.txt\0所有文件 (*.*)\0*.*\0\0");

HWND hwnd;
HWND hwndPlot;
HWND hwndFunction;
HWND hwndLog;
HWND hSetDlg;

HINSTANCE hInst;
int  cxChar, cyChar ;

FileInfo fileData;
FileInfo fileDataSmooth;

BOOL beFaund = false;

int OriginalNum = 0;
int SmoothNum = 0;

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
	wndclass.hbrBackground = CreateSolidBrush(bkColor);/*(HBRUSH) GetStockObject (LTGRAY_BRUSH)*/
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
		//rect.left = rect.right/4;
		//plotWnd.CreatePlotWindow(hwnd , rect);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("原始数据"),
			WS_CHILD|WS_VISIBLE,10,10,rect.right/4 - 20,30,
			hwnd, (HMENU)IDB_GETINFO, NULL,NULL);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("光滑处理"),
			WS_CHILD|WS_VISIBLE,40,rect.bottom/16+30,200,30,
			hwnd, (HMENU)IDB_SMOOTH, NULL,NULL);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("对称零面积：高斯"),
			WS_CHILD|WS_VISIBLE,40,rect.bottom/16+70,200,30,
			hwnd, (HMENU)IDB_GAUSSIAN, NULL,NULL);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("对称零面积：方波"),
			WS_CHILD|WS_VISIBLE,40,rect.bottom/16+110,200,30,
			hwnd, (HMENU)IDB_SQUARE_WAVE, NULL,NULL);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("幅值寻峰"),
			WS_CHILD|WS_VISIBLE,40,rect.bottom/16+150,200,30,
			hwnd, (HMENU)IDB_AMPLITUDE, NULL,NULL);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("CombinedSZA"),
			WS_CHILD|WS_VISIBLE,40,rect.bottom/16+190,200,30,
			hwnd, (HMENU)IDB_COMBINEDSZA, NULL,NULL);

		CreateWindow(TEXT("button"), TEXT("Peak Searching"), WS_CHILD|WS_VISIBLE|BS_GROUPBOX, 
			10, rect.bottom/16+10, rect.right/4 - 20, 3 * rect.bottom/8 + 15 , hwnd, (HMENU)IDC_FRAME1, NULL, NULL); 

		CreateWindow(TEXT("button"), TEXT("Filtering Algorithm"), WS_CHILD|WS_VISIBLE|BS_GROUPBOX, 
			10, rect.bottom/2 , rect.right/4 - 20, 3 * rect.bottom/8 + 15, hwnd, (HMENU)IDC_FRAME1, NULL, NULL); 

		hwndFunction = CreateWindow(TEXT("button"),TEXT("AmplitudeLimiter"),
			WS_CHILD|WS_VISIBLE,40,rect.bottom/2+20,200,30,
			hwnd, (HMENU)IDB_FILTERAMP, NULL,NULL);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("Average Filter"),
			WS_CHILD|WS_VISIBLE,40,rect.bottom/2+60,200,30,
			hwnd, (HMENU)IDB_FILTERAVG, NULL,NULL);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("Median Filter"),
			WS_CHILD|WS_VISIBLE,40,rect.bottom/2+100,200,30,
			hwnd, (HMENU)IDB_FILTERMID, NULL,NULL);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("Debounce Filter"),
			WS_CHILD|WS_VISIBLE,40,rect.bottom/2+140,200,30,
			hwnd, (HMENU)IDB_FILTERDEBNC, NULL,NULL);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("RecursiveMedian Filter"),
			WS_CHILD|WS_VISIBLE,40,rect.bottom/2+180,200,30,
			hwnd, (HMENU)IDB_FILTERRECURSIVE, NULL,NULL);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("直线本底"),
			WS_CHILD|WS_VISIBLE,10,rect.bottom-40,rect.right/8-15,30,
			hwnd, (HMENU)IDB_STRAIGHTLINE, NULL,NULL);

		hwndFunction = CreateWindow(TEXT("button"),TEXT("SNIP拟合本底"),
			WS_CHILD|WS_VISIBLE,rect.right/8+5,rect.bottom-40,rect.right/8-15,30,
			hwnd, (HMENU)IDB_SNIP, NULL,NULL);

		rect.left = rect.right/4;
		plotWnd.CreatePlotWindow(hwnd , rect);

		ReleaseDC(hwnd,hdc);

		return 0;

	case WM_CTLCOLORSTATIC: 
		{ 
			HDC hdc = (HDC)wParam; 
			//SetBkMode(hdc, TRANSPARENT);
			//将背景模式设置成透明时，会看到组合框框架的框线；

			SetTextColor( hdc, RGB(0,0,0) );  //白色
			SetBkColor( hdc, bkColor );

		} 
		return (BOOL)((HBRUSH)GetStockObject(NULL_BRUSH));
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
				ofn.lpstrFilter = szFilter;             //过滤器，显示什么格式的文件
				ofn.nFilterIndex = 2;
				ofn.lpstrFile = szFileName;      //文件名+完整的路径
				ofn.nMaxFile = MAX_PATH;           //ofn.lpstrFile字符串最多有多长
				ofn.lpstrFileTitle = szTitleName;     //只有文件名
				ofn.nMaxFileTitle = MAX_PATH;
				ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
				ofn.lpstrDefExt = TEXT("txt");

				if (GetOpenFileName(&ofn)) 
				{
					fileData = GetData( szFileName );
					if(fileData)
					{
						MessageBox( NULL, TEXT("数据获取成功"), TEXT("数据处理"), MB_OK );
						OriginalNum = GetLineNum( szFileName );
						plotWnd.dataReady = true;
						GetDataRange(minX , maxX, minY , maxY ,fileData);

						SendMessage( plotWnd.hwndPlot , WM_PAINT , wParam , lParam );
					}
					else
					{
						MessageBox( NULL, TEXT("数据获取失败"), TEXT("数据处理"), MB_OK );
					}
				}

				break;

			case IDB_GETINFO:
				{
					p = plotWnd.Head;
					while(p) {
						if((p->line).LineName == ORIGIN) {
							beFaund = true;
							p = p->next;
						}
						else {
							p = p->next;
						}
					}

					if(beFaund){
						beFaund = false;
					}
					else{
						if( fileData == NULL )
						{
							//break;
							//先获取数据
						}
						else{
							Line origin;
							origin.data = fileData;
							origin.type = CURVE;
							origin.LineColor = BLACK;
							origin.LineName = ORIGIN;
							origin.PointNum = OriginalNum;
							_sntprintf_s(origin.szName, 128, TEXT("OriginalData"));
							temp = origin;
							plotWnd.RecordLineList( plotWnd.Head , plotWnd.Rear , temp );
						}
					}
					SendMessage( plotWnd.hwndPlot , WM_PAINT , wParam , lParam );
				}
				break;

			case IDB_SMOOTH:
				{
					p = plotWnd.Head;
					while(p) {
						if((p->line).LineName == SMOOTH) {
							beFaund = true;
							p = p->next;
						}
						else {
							p = p->next;
						}
					}

					if(beFaund){
						beFaund = false;
					}
					else{
						if( fileData == NULL )
						{
							//break;
							//先获取数据
						}
						else{
							lineNum = OriginalNum;
							Smooth(fileData , lineNum ,fileDataSmooth);
							SmoothNum = lineNum;

							Line smooth;
							smooth.data = fileDataSmooth;
							smooth.PointNum = lineNum;
							smooth.LineColor = RED;
							smooth.PenStyle = PS_DASH;
							smooth.type = CURVE;
							smooth.LineName = SMOOTH;
							_sntprintf_s(smooth.szName, 128, TEXT("Smooth"));
							temp = smooth;
							plotWnd.RecordLineList( plotWnd.Head , plotWnd.Rear , temp);

							_sntprintf_s(szFileNameSmooth, 128, smooth.szName);
							GenerateFileName( szFileNameSmooth );
							WriteFileInfo( lineNum , fileDataSmooth , szFileNameSmooth );
						}
					}
					SendMessage(plotWnd.hwndPlot , WM_PAINT , wParam ,lParam);
				}
				break;

			case IDB_GAUSSIAN:
				{
					p = plotWnd.Head;
					while(p) {
						if((p->line).LineName == SZA_G) {
							beFaund = true;
							p = p->next;
						}
						else {
							p = p->next;
						}
					}

					if(beFaund){
						beFaund = false;
					}
					else{
						Line gaussian;
						_sntprintf_s(gaussian.szName, 128, TEXT("SZA_Gaussian"));
						_sntprintf_s(szSPeakName, 128, gaussian.szName);

						if( SR( fileDataSmooth , SmoothNum ) )
						{
							//break;
							//先进行光滑处理
						}
						else{
							gaussian.data = GetData(szSPeakName);
							gaussian.PointNum = lineNum;
							gaussian.LineColor = ORANGE;
							gaussian.type = DOT;
							gaussian.LineName = SZA_G;
							temp = gaussian;
							plotWnd.RecordLineList( plotWnd.Head , plotWnd.Rear , temp);
						}
					}
					SendMessage(plotWnd.hwndPlot , WM_PAINT , wParam ,lParam);
				}
				break;

			case IDB_AMPLITUDE:
				{
					p = plotWnd.Head;
					while(p) {
						if((p->line).LineName == AMPLITUDE) {
							beFaund = true;
							p = p->next;
						}
						else {
							p = p->next;
						}
					}

					if(beFaund){
						beFaund = false;
					}
					else{
						Line amplitude;
						_sntprintf_s(amplitude.szName, 128, TEXT("FdPk_Amplitude"));
						_sntprintf_s(szAmpName, 128, amplitude.szName);

						if( Amplitude( fileDataSmooth , SmoothNum ) )
						{
							//break;
							//先进行光滑处理
						}
						else{
							amplitude.data = GetData(szAmpName);
							amplitude.PointNum = lineNum;
							amplitude.LineColor = PURPLE;
							amplitude.type = DOT;
							amplitude.LineName = AMPLITUDE;
							temp = amplitude;
							plotWnd.RecordLineList( plotWnd.Head , plotWnd.Rear , temp);
						}
					}
					SendMessage(plotWnd.hwndPlot , WM_PAINT , wParam ,lParam);
				}
				break;

			case IDB_COMBINEDSZA:
				{	p = plotWnd.Head;
				while(p) {
					if((p->line).LineName == COMBINEDSZA) {
						beFaund = true;
						p = p->next;
					}
					else {
						p = p->next;
					}
				}

				if(beFaund){
					beFaund = false;
				}
				else{
					Line CombinedSZA;
					_sntprintf_s(CombinedSZA.szName, 128, TEXT("FdPk_CombinedSZA"));
					_sntprintf_s(szCombinedPeakName, 128, CombinedSZA.szName);

					if( CombinedAmplitude( fileData, SmoothNum ) )
					{
						//break;
						//先进行光滑处理
					}
					else{

						CombinedSZA.data = GetData(szCombinedPeakName);
						CombinedSZA.PointNum = lineNum;
						CombinedSZA.LineColor = LTPINK;
						CombinedSZA.PenStyle = PS_SOLID;
						CombinedSZA.type = DOT;
						CombinedSZA.LineName = COMBINEDSZA;
						temp = CombinedSZA;
						plotWnd.RecordLineList( plotWnd.Head , plotWnd.Rear , temp);
					}
				}
				SendMessage(plotWnd.hwndPlot , WM_PAINT , wParam ,lParam);
				}
				break;

			case IDB_FILTERAMP:
				{
					p = plotWnd.Head;
					while(p) {
						if((p->line).LineName == FILTERAMP) {
							beFaund = true;
							p = p->next;
						}
						else {
							p = p->next;
						}
					}

					if(beFaund){
						beFaund = false;
					}
					else{
						Line filterAmp;
						_sntprintf_s(filterAmp.szName, 128, TEXT("Filter_AmplitudeLimiter"));
						_sntprintf_s(szAmpFilter, 128, filterAmp.szName);

						if( AmplitudeLimiterFilter( fileData, OriginalNum ) )
						{
							//break;
							//先打开文件获取数据
						}
						else{
							filterAmp.data = GetData(szAmpFilter);
							filterAmp.PointNum = lineNum;
							filterAmp.LineColor = GREEN;
							filterAmp.PenStyle = PS_DASHDOT;
							filterAmp.type = CURVE;
							filterAmp.LineName = FILTERAMP;
							temp = filterAmp;
							plotWnd.RecordLineList( plotWnd.Head , plotWnd.Rear , temp);
						}
					}
					SendMessage(plotWnd.hwndPlot , WM_PAINT , wParam ,lParam);
				}
				break;

			case IDB_FILTERAVG:
				{
					p = plotWnd.Head;
					while(p) {
						if((p->line).LineName == FILTERAVG) {
							beFaund = true;
							p = p->next;
						}
						else {
							p = p->next;
						}
					}

					if(beFaund){
						beFaund = false;
					}
					else{
						Line filterAvg;
						_sntprintf_s(filterAvg.szName, 128, TEXT("Filter_Average"));
						_sntprintf_s(szAvgFilter, 128, filterAvg.szName);

						if( AverageFilter( fileData, OriginalNum ) )
						{
							//break;
							//先打开文件获取数据
						}
						else{
							filterAvg.data = GetData(szAvgFilter);
							filterAvg.PointNum = lineNum;
							filterAvg.LineColor = BLUE;
							filterAvg.PenStyle = PS_DASH;
							filterAvg.type = CURVE;
							filterAvg.LineName = FILTERAVG;
							temp = filterAvg;
							plotWnd.RecordLineList( plotWnd.Head , plotWnd.Rear , temp);
						}
					}
					SendMessage(plotWnd.hwndPlot , WM_PAINT , wParam ,lParam);
				}
				break;

			case IDB_FILTERMID:
				{	p = plotWnd.Head;
				while(p) {
					if((p->line).LineName == FILTERMEDIAN) {
						beFaund = true;
						p = p->next;
					}
					else {
						p = p->next;
					}
				}

				if(beFaund){
					beFaund = false;
				}
				else{
					Line filterMedian;
					_sntprintf_s(filterMedian.szName, 128, TEXT("Filter_Median"));
					_sntprintf_s(szMedianFilter, 128, filterMedian.szName);

					if( MedianFilter( fileData, OriginalNum ) )
					{
						//break;
						//先打开文件获取数据
					}
					else{

						filterMedian.data = GetData(szMedianFilter);
						filterMedian.PointNum = lineNum;
						filterMedian.LineColor = PINK;
						filterMedian.PenStyle = PS_SOLID;
						filterMedian.type = CURVE;
						filterMedian.LineName = FILTERMEDIAN;
						temp = filterMedian;
						plotWnd.RecordLineList( plotWnd.Head , plotWnd.Rear , temp);
					}
				}
				SendMessage(plotWnd.hwndPlot , WM_PAINT , wParam ,lParam);
				}
				break;

			case IDB_FILTERDEBNC:
				{	p = plotWnd.Head;
				while(p) {
					if((p->line).LineName == FILTERDEBNC) {
						beFaund = true;
						p = p->next;
					}
					else {
						p = p->next;
					}
				}

				if(beFaund){
					beFaund = false;
				}
				else{
					Line filterDeBnc;
					_sntprintf_s(filterDeBnc.szName, 128, TEXT("Filter_Debounce"));
					_sntprintf_s(szFilterDeBnc, 128, filterDeBnc.szName);

					if( DebounceFilter( fileData, OriginalNum ) )
					{
						//break;
						//先打开文件获取数据
					}
					else{

						filterDeBnc.data = GetData(szFilterDeBnc);
						filterDeBnc.PointNum = lineNum;
						filterDeBnc.LineColor = LTBLUE;
						filterDeBnc.PenStyle = PS_DASHDOT;
						filterDeBnc.type = CURVE;
						filterDeBnc.LineName = FILTERDEBNC;
						temp = filterDeBnc;
						plotWnd.RecordLineList( plotWnd.Head , plotWnd.Rear , temp);
					}
				}
				SendMessage(plotWnd.hwndPlot , WM_PAINT , wParam ,lParam);
				}
				break;

			case IDB_FILTERRECURSIVE:
				{	p = plotWnd.Head;
				while(p) {
					if((p->line).LineName == FILTERRECURSIVE) {
						beFaund = true;
						p = p->next;
					}
					else {
						p = p->next;
					}
				}

				if(beFaund){
					beFaund = false;
				}
				else{
					Line filterRecursive;
					_sntprintf_s(filterRecursive.szName, 128, TEXT("Filter_RecursiveMedian"));
					_sntprintf_s(szFilterRecursive, 128, filterRecursive.szName);

					if( RecursionMiddleFilter( fileData, OriginalNum ) )
					{
						//break;
						//先打开文件获取数据
					}
					else{

						filterRecursive.data = GetData(szFilterRecursive);
						filterRecursive.PointNum = lineNum;
						filterRecursive.LineColor = YELLOW;
						filterRecursive.PenStyle = PS_SOLID;
						filterRecursive.type = CURVE;
						filterRecursive.LineName = FILTERRECURSIVE;
						temp = filterRecursive;
						plotWnd.RecordLineList( plotWnd.Head , plotWnd.Rear , temp);
					}
				}
				SendMessage(plotWnd.hwndPlot , WM_PAINT , wParam ,lParam);
				}
				break;

			case IDB_STRAIGHTLINE:
				{	p = plotWnd.Head;
				while(p) {
					if((p->line).LineName == BASESTRAIGHT) {
						beFaund = true;
						p = p->next;
					}
					else {
						p = p->next;
					}
				}

				if(beFaund){
					beFaund = false;
				}
				else{
					if(szBaseName){
						Line BaseStraight;
						_sntprintf_s(BaseStraight.szName, 128, TEXT("Base_StraightLine"));
						BaseStraight.data = GetData(szBaseName);
						BaseStraight.PointNum = lineNum;
						BaseStraight.LineColor = LTPURPLE;
						BaseStraight.PenStyle = PS_SOLID;
						BaseStraight.type = CURVE;
						BaseStraight.LineName = BASESTRAIGHT;
						temp = BaseStraight;
						plotWnd.RecordLineList( plotWnd.Head , plotWnd.Rear , temp);
					}
					else
					{
					//先进行对称零面积处理
					}
				}
				SendMessage(plotWnd.hwndPlot , WM_PAINT , wParam ,lParam);
				}
				break;

			case IDB_SNIP:

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