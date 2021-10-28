#include "include.h"

void DrawRuler (HWND hwnd, RECT * rect)
{
    HDC    hdc;
    static int iRuleSize [10] = { 2.5 * SCALE_HEIGHT_UNIT, SCALE_HEIGHT_UNIT, SCALE_HEIGHT_UNIT, SCALE_HEIGHT_UNIT, SCALE_HEIGHT_UNIT,
                                    2 * SCALE_HEIGHT_UNIT, SCALE_HEIGHT_UNIT, SCALE_HEIGHT_UNIT, SCALE_HEIGHT_UNIT, SCALE_HEIGHT_UNIT } ;
    int i, j, nXStep, nYStep;

    hdc = GetDC (hwnd) ;
    SaveDC(hdc);

    SetMapMode (hdc, MM_ANISOTROPIC) ;
    SetWindowExtEx (hdc, SCALE_HEIGHT_UNIT * 20, SCALE_HEIGHT_UNIT * -20, NULL) ;//�߼�����
    SetViewportExtEx (hdc, GetDeviceCaps (hdc, LOGPIXELSX),GetDeviceCaps (hdc, LOGPIXELSY), NULL) ;//�豸����
    SetViewportOrgEx (hdc, rect->left , rect->bottom , NULL) ;//�豸����

    DPtoLP (hdc, (PPOINT)rect, 2);
    Rectangle (hdc, rect->left, rect->top, rect->right, rect->bottom - TWIP_PER_POINT );  //�����ο�

    if ((rect->right - rect->left ) < 100 * SCALE_HEIGHT_UNIT)
        nXStep = SCALE_HEIGHT_UNIT;
    else
        nXStep = (rect->right - rect->left) / 100;

    for (i = rect->left, j = 0 ; i <= rect->right ; i += nXStep, j++)         //��ˮƽ��Ŀ̶���
    {
         MoveToEx (hdc, i, rect->bottom , NULL) ;
         LineTo   (hdc, i, iRuleSize [j % 10]) ;
    }

    if ((rect->top - rect->bottom ) < 100 * SCALE_HEIGHT_UNIT)
        nYStep = SCALE_HEIGHT_UNIT;
    else
        nYStep = (rect->top - rect->bottom ) / 100;

    for (i = rect->bottom , j = 0 ; i <= rect->top ; i += nYStep, j++)         //����ֱ��Ŀ̶���
    {
         MoveToEx (hdc, rect->left, i, NULL) ;
         LineTo   (hdc, iRuleSize [j % 10], i) ;
    }

    RestoreDC (hdc, -1) ;
    ReleaseDC (hwnd, hdc);

    return;
}