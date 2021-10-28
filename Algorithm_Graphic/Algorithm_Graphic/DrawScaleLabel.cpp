#include "include.h"

void DrawHScaleLabel ( HWND hwnd, RECT * rect, double dStartValue, double dEndValue)
{
    static TCHAR szDecimal[] = TEXT("%d");        //输出整型格式
    static TCHAR szExponent[] = TEXT("%.2E");     //输出指数格式
    static TCHAR szFloat[] = TEXT("%.2f");        //输出浮点格式
    TCHAR  szBuffer[10]={0};                      //定义临时字符串，用于格式化数字
    TCHAR *pFormat;                               //指向不同的数据格式
    bool   bIsDecimal;                            //是否用于整型输出的标志位

    int        i, nRulerSpan, cxString, cyString, nRulerSegments, nOutputLeft;
    HDC        hdc;
    LOGFONT    lf ;
    RECT rectString;
    double dLeftValue, dRightValue, dIntervalValue, dOutputValue, dRatio;
    
    if ( dStartValue > dEndValue )                //如果左边大于右边，要进行调换
    {
        dLeftValue = dEndValue ;                  //dLeftValue总是存放最小的数
        dRightValue = dStartValue ;               //dRightValue总是存放最大的数
    }
    else
    {
        dLeftValue = dStartValue ;                //dLeftValue总是存放最小的数
        dRightValue = dEndValue ;                 //dRightValue总是存放最大的数
    }
    dIntervalValue = (dRightValue - dLeftValue )/10 ;  //计算每一段的大小

    hdc = GetDC (hwnd) ;
    SaveDC(hdc);

    SetMapMode (hdc, MM_ANISOTROPIC) ;
    SetWindowExtEx (hdc, SCALE_HEIGHT_UNIT * 20, -20 * SCALE_HEIGHT_UNIT, NULL) ;                   //逻辑坐标范围
    SetViewportExtEx (hdc, GetDeviceCaps (hdc, LOGPIXELSX),GetDeviceCaps (hdc, LOGPIXELSY), NULL) ; //设备坐标范围
    SetViewportOrgEx (hdc, rect->left , rect->bottom , NULL) ;                                      //设备坐标原点

    DPtoLP (hdc, (PPOINT)rect, 2);                       //将设备坐标转化为逻辑坐标
    FillRect(hdc, rect, (HBRUSH)WHITE_BRUSH ) ;         //填充矩形框

    GetObject (GetStockObject (SYSTEM_FONT), sizeof (lf), &lf) ;
    lstrcpy (lf.lfFaceName, TEXT ("Times New Roman")) ;  //只有Times  New Roman字体可以进行变化
    lf.lfHeight = lf.lfHeight * 20 ;                     //逻辑字体的大小也要跟着变化
    lf.lfWidth = lf.lfWidth * 20 ;                       //将点point转化为twips,每个点20个twip
    lf.lfWeight         = 0 ;                            //默认值是加粗，现在要改为不是加粗的
    SelectObject (hdc, CreateFontIndirect (&lf)) ;       //创建以逻辑坐标为单位的字体

    if ( ( rect->right - rect->left) > SCALE_HEIGHT_UNIT * 100 )  // 72twips/step * 100step = 7200twips,  超过设定的最小值，则以最小值为准
    {
        nRulerSegments = 10;                             //段数最大为10
        nRulerSpan = ( rect->right - rect->left ) / 10 ; //根据段数计算每段的间隔大小
    }
    else
    {
        nRulerSegments = 1 + ( rect->right - rect->left ) / ( SCALE_HEIGHT_UNIT * 10 );   //计算区间大小
        nRulerSpan = SCALE_HEIGHT_UNIT * 10 ;                                             //根据区间大小计算段数
    }

    if ( (( dRightValue - dLeftValue) <= 1000000) && (( dRightValue - dLeftValue) > 10) )  //大于10而小于等于1百万，采用取整形式
    {
        pFormat = szDecimal ;                    //赋予对应的整型格式
        bIsDecimal = TRUE;                       //标志位置1
    }

    if ( (( dRightValue - dLeftValue) <= 10) && (( dRightValue - dLeftValue) >= 0.01))// 大于等于0.01且小于等于10就采用浮点形式
    {
        pFormat = szFloat ;                    //赋予对应的浮点型格式
        bIsDecimal = FALSE;                    //标志位置0
    }

    if ( (( dRightValue - dLeftValue) > 1000000) || (( dRightValue - dLeftValue) < 0.01))// 大于1百万或小于0.01就采用指数形式
    {
        pFormat = szExponent ;                 //赋予对应的指数型格式
        bIsDecimal = FALSE;                    //标志位置0
    }

    memset( &rectString, 0, sizeof(rectString));
	if ( bIsDecimal )
		_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), pFormat, (int)dRightValue) ;  //以整型格式输出
	else
		_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), pFormat, dRightValue) ;       //以浮点型或指数型格式输出
    DrawText ( hdc, szBuffer, -1, &rectString, DT_CALCRECT );                                //获得输出字符占用的矩形大小
    cxString = rectString.right - rectString.left ;                                          //重新计算矩形宽度
    cyString = rectString.top - rectString.bottom ;

	dRatio = 1.0 * cxString / (nRulerSpan / 2) > 1.0 * cyString / (rect->top - rect->bottom ) ? 1.0 * nRulerSpan / 2 / cxString : 1.0 * (rect->top - rect->bottom) / cyString ;
    lf.lfWidth = -(int) (lf.lfWidth * 1.0 * dRatio );                 //对字体宽度进行缩放
    lf.lfHeight = (int) (lf.lfHeight * 1.0 * dRatio );                //对字体高度进行缩放
    SelectObject ( hdc , CreateFontIndirect (&lf)) ;
	DrawText ( hdc, szBuffer, -1, &rectString, DT_CALCRECT );                            //获得输出字符占用的矩形大小
	cxString = rectString.right - rectString.left ;                                      //重新计算矩形宽度
	cyString = rectString.top - rectString.bottom ;                                      //重新计算矩形宽度

    for ( i = 0; i <= nRulerSegments; i++ )
    {
        memset( szBuffer, 0, sizeof( szBuffer ));                                            //对字符串进行清零操作
        dOutputValue = dLeftValue + i * dIntervalValue;                                      //计算输出值，
		if( bIsDecimal)
		    _sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), pFormat, (int)dOutputValue );  //格式化整型字符串
		else
			_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), pFormat, dOutputValue ); //格式化小数字符串，变成浮点型或指数型
        DrawText( hdc, szBuffer, -1, &rectString, DT_CALCRECT );
		cxString = rectString.right - rectString.left ;                                     //重新计算矩形宽度
        nOutputLeft = rect->left * (1 - i/nRulerSegments)                                   //左边界的计算，如果i对段数取商，结果为0的时候，保留左边界
			          + (i % nRulerSegments) * nRulerSpan                                   //每一段对段数进行取余操作，得到某一i段对应的位置
			          - (((i % nRulerSegments) == 0)?0:1) * cxString /2                     //根据是否边界决定标签是否左移半个身长
			          + (rect->right - cxString) * (i/nRulerSegments) ;                     //右边界的计算，如果i对段数取商，结果为1的时候，保留左边界
        TextOut(hdc, nOutputLeft, rect->top, szBuffer, lstrlen(szBuffer));                  //输出标签
    }
}

void DrawVScaleLabel ( HWND hwnd, RECT * rect, double dStartValue, double dEndValue)
{
    static TCHAR szDecimal[] = TEXT("%d");        //输出整型格式
    static TCHAR szExponent[] = TEXT("%.2E");     //输出指数格式
    static TCHAR szFloat[] = TEXT("%.2f");        //输出浮点格式
    TCHAR  szBuffer[10]={0};                      //定义临时字符串，用于格式化数字
    TCHAR *pFormat;                               //指向不同的数据格式
    bool   bIsDecimal;                            //是否用于整型输出的标志位

    int        i, nRulerSpan, cxString, cyString, nRulerSegments, nOutputTop;
    HDC        hdc;
    LOGFONT    lf ;
    RECT rectString;
    double dTopValue, dBottomValue, dIntervalValue, dOutputValue;
    
    if ( dStartValue > dEndValue )                //如果左边大于右边，要进行调换
    {
        dBottomValue = dEndValue ;                //dBottomValue总是存放最小的数
        dTopValue = dStartValue ;                 //dTopValue总是存放最大的数
    }
    else
    {
        dBottomValue = dStartValue ;              //dBottomValue总是存放最小的数
        dTopValue = dEndValue ;                   //dTopValue总是存放最大的数
    }
    dIntervalValue = (dTopValue - dBottomValue )/10 ;  //计算每一段的大小

    hdc = GetDC (hwnd) ;
    SaveDC(hdc);

    SetMapMode (hdc, MM_ANISOTROPIC) ;
    SetWindowExtEx (hdc, SCALE_HEIGHT_UNIT * 20, -20 * SCALE_HEIGHT_UNIT, NULL) ;                   //逻辑坐标范围
    SetViewportExtEx (hdc, GetDeviceCaps (hdc, LOGPIXELSX),GetDeviceCaps (hdc, LOGPIXELSY), NULL) ; //设备坐标范围
    SetViewportOrgEx (hdc, rect->left , rect->bottom , NULL) ;                                      //设备坐标原点

    DPtoLP (hdc, (PPOINT)rect, 2);                       //将设备坐标转化为逻辑坐标
    FillRect(hdc, rect, (HBRUSH)WHITE_BRUSH ) ;         //填充矩形框

    GetObject (GetStockObject (SYSTEM_FONT), sizeof (lf), &lf) ;
    lstrcpy (lf.lfFaceName, TEXT ("Times New Roman")) ;  //只有Times  New Roman字体可以进行变化
    lf.lfHeight = lf.lfHeight * 20 ;                     //逻辑字体的大小也要跟着变化
    lf.lfWidth = lf.lfWidth * 20 ;                       //将点point转化为twips,每个点20个twip
    lf.lfWeight         = 0 ;                            //默认值是加粗，现在要改为不是加粗的
    SelectObject (hdc, CreateFontIndirect (&lf)) ;       //创建以逻辑坐标为单位的字体

    if ( ( rect->top - rect->bottom) > SCALE_HEIGHT_UNIT * 100 )  // 72twips/step * 100step = 7200twips,  超过设定的最小值，则以最小值为准
    {
        nRulerSegments = 10;                             //段数最大为10
        nRulerSpan = ( rect->top - rect->bottom ) / 10 ; //根据段数计算每段的间隔大小
    }
    else
    {
        nRulerSegments = 1 + ( rect->top - rect->bottom ) / ( SCALE_HEIGHT_UNIT * 10 );   //计算区间大小
        nRulerSpan = SCALE_HEIGHT_UNIT * 10 ;                                             //根据区间大小计算段数
    }

    if ( (( dTopValue - dBottomValue) <= 1000000) && (( dTopValue - dBottomValue) > 10) )  //大于10而小于等于1百万，采用取整形式
    {
        pFormat = szDecimal ;                    //赋予对应的整型格式
        bIsDecimal = TRUE;                       //标志位置1
    }

    if ( (( dTopValue - dBottomValue) <= 10) && (( dTopValue - dBottomValue) >= 0.01))// 大于等于0.01且小于等于10就采用浮点形式
    {
        pFormat = szFloat ;                    //赋予对应的浮点型格式
        bIsDecimal = FALSE;                    //标志位置0
    }

    if ( (( dTopValue - dBottomValue) > 1000000) || (( dTopValue - dBottomValue) < 0.01))// 大于1百万或小于0.01就采用指数形式
    {
        pFormat = szExponent ;                 //赋予对应的指数型格式
        bIsDecimal = FALSE;                    //标志位置0
    }

    memset( &rectString, 0, sizeof(rectString));

	if ( bIsDecimal )
	{
		_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), pFormat, wsprintf(szBuffer, pFormat, (int)dTopValue) >  wsprintf(szBuffer, pFormat, (int)dBottomValue) ? (int)dTopValue : (int)dBottomValue) ;  //以整型格式输出
	}
	else
	{
		_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), pFormat, wsprintf(szBuffer, pFormat, dTopValue) >  wsprintf(szBuffer, pFormat, dBottomValue) ? (int)dTopValue : (int)dBottomValue) ;  //以整型格式输出

	}

    DrawText ( hdc, szBuffer, -1, &rectString, DT_CALCRECT );                                //获得输出字符占用的矩形大小
    cxString = rectString.right - rectString.left +100 ;                                          //重新计算矩形宽度
	//cxString = rectString.right - rectString.left;//输出两位小数时，输不全，+100后可以完全显示

    if ( cxString > (rect->right - rect->left) )
    {
        lf.lfWidth = -(int) (lf.lfWidth * 1.0 * (rect->right - rect->left) / cxString );                //对字体宽度进行缩放
        lf.lfHeight = (int) (lf.lfHeight * 1.0 * (rect->right - rect->left) / cxString );               //对字体高度进行缩放
        SelectObject ( hdc , CreateFontIndirect (&lf)) ;
		cxString = rectString.right - rectString.left ;                                     //重新计算矩形宽度
    }

    for ( i = 0; i <= nRulerSegments; i++ )
    {
        memset( szBuffer, 0, sizeof( szBuffer ));                                           //对字符串进行清零操作
        dOutputValue = dBottomValue + i * dIntervalValue;                                     //计算输出值

		if( bIsDecimal)
		    _sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), pFormat, (int)dOutputValue );  //格式化整型字符串
		else
			_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), pFormat, dOutputValue ); //格式化小数字符串，变成浮点型或指数型

        DrawText( hdc, szBuffer, -1, &rectString, DT_CALCRECT );
		cxString = rectString.right - rectString.left ;                                     //重新计算矩形宽度
		cyString = rectString.top - rectString.bottom ;
		nOutputTop = (rect->bottom + cyString) * (1 - i/nRulerSegments)                                    //左边界的计算，如果i对段数取商，结果为0的时候，保留左边界
			          + (i % nRulerSegments) * nRulerSpan                                   //每一段对段数进行取余操作，得到某一i段对应的位置
			          - (((i % nRulerSegments) == 0)?0:1) * cyString /2                     //根据是否边界决定标签是否左移半个身长
			          + (rect->top ) * (i/nRulerSegments) ;                     //右边界的计算，如果i对段数取商，结果为1的时候，保留左边界
        TextOut(hdc, rect->right - rectString.right + rectString.left, nOutputTop, szBuffer, lstrlen(szBuffer));                  //输出标签
    }
}