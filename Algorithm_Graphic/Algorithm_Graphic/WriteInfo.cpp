#include"include.h"

void MyWriteFile( int lineNum , TCHAR *dataNew ,const TCHAR* path )
{
	FILE *fp=NULL;
	
	if (_tfopen_s( &fp, path, TEXT("a")))
		return ;

	fseek( fp, 0L, SEEK_END );
	fputws( dataNew, fp);

	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}
	return;
}

void WriteFileInfo( int num , FileInfo dataNew ,const TCHAR* path )
{
	FILE *fp=NULL;
	
	if (_tfopen_s( &fp, path, TEXT("a")))
		return ;

	fseek( fp, 0L, SEEK_END );
	for(int i=0; i<num; i++)
	{
		fprintf_s( fp, "%.3lf  %.3lf\n", dataNew[i].X, dataNew[i].Y);
	}
	
	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}
	return;
}

void GenerateFileName( TCHAR* str )
{
	SYSTEMTIME time;
	TCHAR tmp[128]={0};

	GetLocalTime(&time);
	_sntprintf_s(tmp,128,TEXT("_%4d-%02d-%02d_%02dµã%02d·Ö%02dÃë.txt"),time.wYear,time.wMonth,
		time.wDay,time.wHour,time.wMinute,time.wSecond);
	
	_tcscat_s(str , 128 , tmp);
	return ;
}