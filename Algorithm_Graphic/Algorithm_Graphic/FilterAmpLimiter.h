#ifndef _FILTERAMPLIMITER_H_
#define _FILTERAMPLIMITER_H_

//���ܣ��޷��˲��ļ���ʵ��

#define VARIATION_RANGE 0.5

void AmplitudeLimiterFilter( fileInfo *data ,int Num );

extern TCHAR szAmpFilter[128];

#endif