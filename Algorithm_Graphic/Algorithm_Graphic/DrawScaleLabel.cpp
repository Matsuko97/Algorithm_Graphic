#include "include.h"

//��X������
void DrawHScaleLabel ( HWND hwnd, RECT rect, double dStartValue, double dEndValue)
{
	static TCHAR szDecimal[] = TEXT("%d");          //������͸�ʽ
	static TCHAR szExponent[] = TEXT("%.2E");    //���ָ����ʽ
	static TCHAR szFloat[] = TEXT("%.2f") ;           //��������ʽ
	TCHAR  szBuffer[10] = { 0 } ;                            //������ʱ�ַ��������ڸ�ʽ������
	TCHAR *pFormat ;                                             //ָ��ͬ�����ݸ�ʽ
	BOOL   bIsDecimal ;                                           //�Ƿ�������������ı�־λ
	BOOL   bHAjust = FALSE ;                                //�������Ӧ�����С�Ƿ���Ҫˮƽ����
	BOOL   bVAjust = FALSE ;                                //�������Ӧ�����С�Ƿ���Ҫ��ֱ����

	int                 i, nRulerSpan, cxString, cyString, nRulerSegments, nOutputLeft ;
	HDC             hdc ;
	LOGFONT   lf ;
	RECT           rectString ;
	double          dLeftValue, dRightValue, dIntervalValue, dOutputValue ;

	hdc = GetDC (hwnd) ;
	SaveDC(hdc);

	if ( dStartValue > dEndValue )                //�����ߴ����ұߣ�Ҫ���е���
	{
		dLeftValue = dEndValue ;                  //dLeftValue���Ǵ����С����
		dRightValue = dStartValue ;               //dRightValue���Ǵ��������
	}
	else
	{
		dLeftValue = dStartValue ;                //dLeftValue���Ǵ����С����
		dRightValue = dEndValue ;               //dRightValue���Ǵ��������
	}
	dIntervalValue = (dRightValue - dLeftValue )/10 ;  //����ÿһ�εĴ�С

	SetMapMode (hdc, MM_ANISOTROPIC) ;
	SetWindowExtEx (hdc, SCALE_HEIGHT_UNIT * 20, -20 * SCALE_HEIGHT_UNIT, NULL) ;          //�߼����귶Χ

	SetViewportExtEx (hdc, GetDeviceCaps (hdc, LOGPIXELSX), GetDeviceCaps (hdc, LOGPIXELSY), NULL) ; //�豸���귶Χ
	SetViewportOrgEx (hdc, rect.left , rect.bottom , NULL) ;      //�豸����ԭ��

	DPtoLP (hdc, (PPOINT)&rect, 2);                                         //���豸����ת��Ϊ�߼�����
	//FillRect(hdc, &rect, (HBRUSH)HOLLOW_BRUSH ) ;        //�����ο�
	FillRect(hdc, &rect, (HBRUSH)WHITE_BRUSH ) ;          //�����ο�
	//FillRect(hdc, &rect, (HBRUSH)LTGRAY_BRUSH ) ;        //�����ο�

	GetObject (GetStockObject (SYSTEM_FONT), sizeof (lf), &lf) ;
	lstrcpy (lf.lfFaceName, TEXT ("Times New Roman")) ;  //ֻ��Times  New Roman������Խ��б仯
	lf.lfHeight = lf.lfHeight * 20 ;                                         //�߼�����Ĵ�СҲҪ���ű仯
	lf.lfWidth = lf.lfWidth * 20 ;                                           //����pointת��Ϊtwips,ÿ����20��twips
	lf.lfWeight         = 0 ;                                                      //Ĭ��ֵ�ǼӴ֣�����Ҫ��Ϊ���ǼӴֵ�
	SelectObject (hdc, CreateFontIndirect (&lf)) ;               //�������߼�����Ϊ��λ������

    if ( ( rect.right - rect.left) > SCALE_HEIGHT_UNIT * 100 )  // 72twips/�� * 100��tep = 7200twips,  �����趨����Сֵ��������СֵΪ׼
    {
        nRulerSegments = 10;                                  //�������Ϊ10
        nRulerSpan = ( rect.right - rect.left ) / 10 ;   //���ݶ�������ÿ�εļ����С
    }
    else
    {
        nRulerSegments = 1 + ( rect.right - rect.left ) / ( SCALE_HEIGHT_UNIT * 10 );   //���������С
        nRulerSpan = SCALE_HEIGHT_UNIT * 10 ;                                             //���������С�������
    }

    if ( (( dRightValue - dLeftValue) <= 1000000) && (( dRightValue - dLeftValue) > 10) )  //����10��С�ڵ���1���򣬲���ȡ����ʽ
    {
        pFormat = szDecimal ;                    //�����Ӧ�����͸�ʽ
        bIsDecimal = TRUE;                      //��־λ��1
    }

    if ( (( dRightValue - dLeftValue) <= 10) && (( dRightValue - dLeftValue) >= 0.01))// ���ڵ���0.01��С�ڵ���10�Ͳ��ø�����ʽ
    {
        pFormat = szFloat ;                    //�����Ӧ�ĸ����͸�ʽ
        bIsDecimal = FALSE;                //��־λ��0
    }

    if ( (( dRightValue - dLeftValue) > 1000000) || (( dRightValue - dLeftValue) < 0.01))// ����1�����С��0.01�Ͳ���ָ����ʽ
    {
        pFormat = szExponent ;                 //�����Ӧ��ָ���͸�ʽ
        bIsDecimal = FALSE;                    //��־λ��0
    }

    memset( &rectString, 0, sizeof(rectString));
	if ( bIsDecimal )            //���Ϊ���͸�ʽ
		_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR),  sizeof(szBuffer)/sizeof(TCHAR) - 1, pFormat, (int)dRightValue) ;  //�����͸�ʽ���
	else
		_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), sizeof(szBuffer)/sizeof(TCHAR) - 1, pFormat, dRightValue) ;       //�Ը����ͻ�ָ���͸�ʽ���
    DrawText ( hdc, szBuffer, -1, &rectString, DT_CALCRECT );             //�������ַ�ռ�õľ��δ�С
	cxString = rectString.right - rectString.left ;                                          //���¼�����ο��
	cyString = rectString.top - rectString.bottom ;                                      //���¼�����θ߶�

/*
//ԭ���ĳ����õ��ģ���Щbug
	dRatio = 1.0 * cxString / (nRulerSpan / 2) > 1.0 * cyString / (rect.top - rect.bottom ) ? 1.0 * nRulerSpan / 2 / cxString : 1.0 * (rect.top - rect.bottom) / cyString ;
    lf.lfWidth = -(int) (lf.lfWidth * 1.0 * dRatio );                 //�������Ƚ�������
    lf.lfHeight = (int) (lf.lfHeight * 1.0 * dRatio );                //������߶Ƚ�������
*/

//�µ��滻����
	if( cxString > nRulerSpan / 2 )
		bHAjust = TRUE ;
	if( cyString > (rect.top - rect.bottom))
		bVAjust = TRUE ;

	if (bHAjust && (!bVAjust))            //ˮƽ���򳬳�ָ���ķ���
	{
		lf.lfWidth = lf.lfWidth * ( nRulerSpan / 2) / cxString;
	}
	else if( !bHAjust && bVAjust )      //��ֱ���򳬳�ָ���ķ���
	{
		lf.lfHeight = lf.lfHeight * (rect.top - rect.bottom) / cyString;
	}
	else if( bHAjust && bVAjust )      //��ֱ���򳬳�ָ���ķ���
	{
		double  fHRatio ;
		double  fVRatio ;
		double  fRatio ;
		fHRatio = 1.0 * ( nRulerSpan / 2) / cxString ;
		fVRatio = 1.0 * (rect.top - rect.bottom) / cyString ;
		fRatio = fHRatio > fVRatio ? fVRatio : fHRatio ;
		lf.lfWidth = (LONG) (lf.lfWidth * fRatio) ;
		lf.lfHeight = (LONG) (lf.lfHeight * fRatio) ;
	}
//�µ��滻���룬end

	SelectObject ( hdc , CreateFontIndirect (&lf)) ;
	DrawText ( hdc, szBuffer, -1, &rectString, DT_CALCRECT );        //�������ַ�ռ�õľ��δ�С
	cxString = rectString.right - rectString.left ;                                      //���¼�����ο��
	cyString = rectString.top - rectString.bottom ;                                  //���¼�����ο��

	for ( i = 0; i <= nRulerSegments; i++ )
	{
		memset( szBuffer, 0, sizeof( szBuffer ));                                            //���ַ��������������
		dOutputValue = dLeftValue + i * dIntervalValue;                                      //�������ֵ��
		if( bIsDecimal)
		    _sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), sizeof(szBuffer)/sizeof(TCHAR) - 1 , pFormat, (int)dOutputValue );  //��ʽ�������ַ���
		else
			_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), sizeof(szBuffer)/sizeof(TCHAR) - 1 , pFormat, dOutputValue ); //��ʽ��С���ַ�������ɸ����ͻ�ָ����
        DrawText( hdc, szBuffer, -1, &rectString, DT_CALCRECT );
		cxString = rectString.right - rectString.left ;                //���¼�����ο��
        nOutputLeft = rect.left * (1 - i/nRulerSegments)        //��߽�ļ��㣬���i�Զ���ȡ�̣����Ϊ0��ʱ�򣬱�����߽�
			          + (i % nRulerSegments) * nRulerSpan         //ÿһ�ζԶ�������ȡ��������õ�ĳһi�ζ�Ӧ��λ��
			          - (((i % nRulerSegments) == 0)?0:1) * cxString /2       //�����Ƿ�߽������ǩ�Ƿ����ư����
			          + (rect.right - cxString) * (i/nRulerSegments) ;             //�ұ߽�ļ��㣬���i�Զ���ȡ�̣����Ϊ1��ʱ�򣬱�����߽�

		SetBkMode(hdc, TRANSPARENT);                              //���屳��Ϊ͸��ɫ
		TextOut(hdc, nOutputLeft, rect.top, szBuffer, lstrlen(szBuffer));                  //�����ǩ
	}

	RestoreDC (hdc, -1) ;
	ReleaseDC (hwnd, hdc);
	return;
}

//��Y������
void DrawVScaleLabel ( HWND hwnd, RECT rect, double dStartValue, double dEndValue)
{
	static TCHAR szDecimal[] = TEXT("%d") ;        //������͸�ʽ
	static TCHAR szExponent[] = TEXT("%.2E") ;  //���ָ����ʽ
	static TCHAR szFloat[] = TEXT("%.2f") ;          //��������ʽ
	TCHAR  szBuffer[10]={0} ;                               //������ʱ�ַ��������ڸ�ʽ������
	TCHAR  *pFormat ;                                           //ָ��ͬ�����ݸ�ʽ
	BOOL     bIsDecimal ;                                        //�Ƿ�������������ı�־λ
	BOOL     bHAjust = FALSE ;                            //�������Ӧ�����С�Ƿ���Ҫˮƽ����
	BOOL     bVAjust = FALSE ;                            //�������Ӧ�����С�Ƿ���Ҫ��ֱ����

	int                  i, nRulerSpan, cxString, cyString, nRulerSegments, nOutputTop;
	HDC              hdc;
	LOGFONT    lf ;
	RECT            rectString;
	double           dTopValue, dBottomValue, dIntervalValue, dOutputValue ;

	hdc = GetDC (hwnd) ;
	SaveDC(hdc) ;

	if ( dStartValue > dEndValue )                //�����ʼ���ڽ�����Ҫ���е���
	{                                                             //��������˱任
		dBottomValue = dEndValue ;            //���½�Ϊ(0, 0), dBottomValue���Ǵ����С����
		dTopValue = dStartValue ;                 //dTopValue���Ǵ��������
	}
	else
	{
		dBottomValue = dStartValue ;           //dBottomValue���Ǵ����С����
		dTopValue = dEndValue ;                 //dTopValue���Ǵ��������
	}
	dIntervalValue = (dTopValue - dBottomValue )/10 ;  //����ÿһ�εĴ�С

	SetMapMode (hdc, MM_ANISOTROPIC) ;
	SetWindowExtEx (hdc, SCALE_HEIGHT_UNIT * 20, -20 * SCALE_HEIGHT_UNIT, NULL) ;                   //�߼����귶Χ
	SetViewportExtEx (hdc, GetDeviceCaps (hdc, LOGPIXELSX),GetDeviceCaps (hdc, LOGPIXELSY), NULL) ; //�豸���귶Χ
	SetViewportOrgEx (hdc, rect.left , rect.bottom , NULL) ;                                      //�豸����ԭ��

	DPtoLP (hdc, (PPOINT)&rect, 2) ;                                     //���豸����ת��Ϊ�߼�����
	FillRect(hdc, &rect, (HBRUSH)WHITE_BRUSH ) ;         //�����ο�
	//FillRect(hdc, &rect, (HBRUSH)LTGRAY_BRUSH ) ;    //�����ο�
	//FillRect(hdc, &rect, (HBRUSH)NULL_BRUSH ) ;         //�����ο�
	//FillRect(hdc, &rect, (HBRUSH)HOLLOW_BRUSH ) ;        //�����ο�

	GetObject (GetStockObject (SYSTEM_FONT), sizeof (lf), &lf) ;
	lstrcpy (lf.lfFaceName, TEXT ("Times New Roman")) ;  //ֻ��Times  New Roman������Խ��б仯
	lf.lfHeight = lf.lfHeight * 20 ;                                          //�߼�����Ĵ�СҲҪ���ű仯
	lf.lfWidth = lf.lfWidth * 20 ;                                           //����pointת��Ϊtwips,ÿ����20��twips
	lf.lfWeight  = 0 ;                                                             //Ĭ��ֵ�ǼӴ֣�����Ҫ��Ϊ���ǼӴֵ�
	SelectObject (hdc, CreateFontIndirect (&lf)) ;               //�������߼�����Ϊ��λ������

	if ( ( rect.top - rect.bottom) > SCALE_HEIGHT_UNIT * 100 )  // 72twips/step * 100step = 7200twips,  �����趨����Сֵ��������СֵΪ׼
	{
		nRulerSegments = 10;                                        //�������Ϊ10
		nRulerSpan = ( rect.top - rect.bottom ) / 10 ;    //���ݶ�������ÿ�εļ����С
	}
	else
	{
		nRulerSegments = 1 + ( rect.top - rect.bottom ) / ( SCALE_HEIGHT_UNIT * 10 );   //���������С
		nRulerSpan = SCALE_HEIGHT_UNIT * 10 ;                                                            //���������С�������
	}

	if ( (( dTopValue - dBottomValue) <= 1000000) && (( dTopValue - dBottomValue) > 10) )  //����10��С�ڵ���1���򣬲���ȡ����ʽ
	{
		pFormat = szDecimal ;                    //�����Ӧ�����͸�ʽ
		bIsDecimal = TRUE;                       //��־λ��1
	}

	if ( (( dTopValue - dBottomValue) <= 10) && (( dTopValue - dBottomValue) >= 0.01))// ���ڵ���0.01��С�ڵ���10�Ͳ��ø�����ʽ
	{
		pFormat = szFloat ;                    //�����Ӧ�ĸ����͸�ʽ
		bIsDecimal = FALSE;                    //��־λ��0
	}

	if ( (( dTopValue - dBottomValue) > 1000000) || (( dTopValue - dBottomValue) < 0.01))// ����1�����С��0.01�Ͳ���ָ����ʽ
	{
		pFormat = szExponent ;                 //�����Ӧ��ָ���͸�ʽ
		bIsDecimal = FALSE;                    //��־λ��0
	}

	memset( &rectString, 0, sizeof(rectString));

	if ( bIsDecimal )  //�����͸�ʽ���
	{
		int  iTopLen ;
		int  iBottomLen ;

		memset(szBuffer, 0, sizeof(szBuffer) );
		_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), sizeof(szBuffer)/sizeof(TCHAR) - 1,  pFormat, (int)dTopValue );
		iTopLen = _tcslen(szBuffer) ;

		memset(szBuffer, 0, sizeof(szBuffer));
		_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), sizeof(szBuffer)/sizeof(TCHAR) - 1,  pFormat, (int)dBottomValue );
		iBottomLen = _tcslen(szBuffer) ;

		if (iTopLen > iBottomLen)
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), sizeof(szBuffer)/sizeof(TCHAR) - 1,  pFormat, (int)dTopValue );
		}
		else
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), sizeof(szBuffer)/sizeof(TCHAR) - 1,  pFormat, (int)dBottomValue );
		}

	}
	else  //�Ը����ͻ�ָ���͸�ʽ���
	{
		int  iTopLen ;
		int  iBottomLen ;

		memset(szBuffer, 0, sizeof(szBuffer));
		_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), sizeof(szBuffer)/sizeof(TCHAR) - 1,  pFormat, dTopValue );
		iTopLen = _tcslen(szBuffer) ;

		memset(szBuffer, 0, sizeof(szBuffer));
		_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), sizeof(szBuffer)/sizeof(TCHAR) - 1,  pFormat, dBottomValue );
		iBottomLen = _tcslen(szBuffer) ;

		if (iTopLen > iBottomLen)
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), sizeof(szBuffer)/sizeof(TCHAR) - 1,  pFormat, dTopValue );
		}
		else
		{
			memset(szBuffer, 0, sizeof(szBuffer));
			_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR), sizeof(szBuffer)/sizeof(TCHAR) - 1,  pFormat, dBottomValue );
		}
	}

	DrawText ( hdc, szBuffer, -1, &rectString, DT_CALCRECT );            //�������ַ�ռ�õľ��δ�С
	cxString = rectString.right - rectString.left ;                                          //���¼�����ο��
	cyString = rectString.top - rectString.bottom ;

	while (cxString > (rect.right - rect.left) )
	{
		lf.lfWidth = (int) (lf.lfWidth * 1.0 * (rect.right - rect.left) / cxString );              //�������Ƚ�������
 		lf.lfHeight = (int) (lf.lfHeight * 1.0 * (rect.right - rect.left) / cxString );            //������߶Ƚ�������
		SelectObject ( hdc , CreateFontIndirect (&lf)) ;
		DrawText ( hdc, szBuffer, -1, &rectString, DT_CALCRECT );  
		cxString = rectString.right - rectString.left ;                                                     //���¼�����ο��
	}

	for ( i = 0; i <= nRulerSegments; i++ )
	{
		memset( szBuffer, 0, sizeof( szBuffer ));                                           //���ַ��������������
		dOutputValue = dBottomValue + i * dIntervalValue;                                     //�������ֵ

		if( bIsDecimal)
		    _sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR),  sizeof(szBuffer)/sizeof(TCHAR) - 1, pFormat, (int)dOutputValue );  //��ʽ�������ַ���
		else
			_sntprintf_s(szBuffer, sizeof(szBuffer)/sizeof(TCHAR),  sizeof(szBuffer)/sizeof(TCHAR) - 1, pFormat, dOutputValue ); //��ʽ��С���ַ�������ɸ����ͻ�ָ����

		DrawText( hdc, szBuffer, -1, &rectString, DT_CALCRECT );
		cxString = rectString.right - rectString.left ;                                     //���¼�����ο��
		cyString = rectString.top - rectString.bottom ;
		nOutputTop = (rect.bottom + cyString) * (1 - i/nRulerSegments)                                    //��߽�ļ��㣬���i�Զ���ȡ�̣����Ϊ0��ʱ�򣬱�����߽�
			          + (i % nRulerSegments) * nRulerSpan                                   //ÿһ�ζԶ�������ȡ��������õ�ĳһi�ζ�Ӧ��λ��
			          - (((i % nRulerSegments) == 0)?0:1) * cyString /2                     //�����Ƿ�߽������ǩ�Ƿ����ư����
			          + (rect.top ) * (i/nRulerSegments) ;                     //�ұ߽�ļ��㣬���i�Զ���ȡ�̣����Ϊ1��ʱ�򣬱�����߽�
		SetBkMode(hdc, TRANSPARENT);                              //���屳��Ϊ͸��ɫ
		TextOut(hdc, rect.right - rectString.right + rectString.left, nOutputTop, szBuffer, lstrlen(szBuffer));                  //�����ǩ
	}

	RestoreDC (hdc, -1) ;
	ReleaseDC (hwnd, hdc);
	return;

}