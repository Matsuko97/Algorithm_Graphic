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
//文件输入输出
#include "GetInfo.h"
#include "WriteInfo.h"
//绘制
#include "DrawRuler.h"
#include "DrawScaleLabel.h"
#include "DrawPlot.h"
//寻峰算法
#include "Smooth.h"
#include "SymmetricZeroArea.h"
#include "Amplitude.h"
//滤波算法
#include "FilterAmpLimiter.h"
#include "FilterAvg.h"
#include "FilterDeBnc.h"
#include "FilterMedian.h"