#include <windows.h>
#include<stdio.h>
#include<tchar.h>
#include<math.h>
#include<strsafe.h>
#include<WinUser.h>
#include<stdlib.h>

#include "resource.h"
#include "WinMain.h"
#include "Line.h"
#include "PlotWnd.h"
//�ļ��������
#include "GetInfo.h"
#include "WriteInfo.h"
//����
#include "DrawRuler.h"
#include "DrawScaleLabel.h"
#include "DrawPlot.h"
//Ѱ���㷨
#include "Smooth.h"
#include "SymmetricZeroArea.h"
#include "Amplitude.h"
//�˲��㷨
#include "FilterAmpLimiter.h"
#include "FilterAvg.h"
#include "FilterDeBnc.h"
#include "FilterMedian.h"