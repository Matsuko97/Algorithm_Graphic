#include "include.h"

TCHAR szFileNameSmooth[128]={0};

void Smooth(FileInfo dataNew, int &lineNum, FileInfo &dataNewSmooth)
{
	dataNewSmooth = (fileInfo*)calloc( lineNum , sizeof(fileInfo) );

	for(int i=2;i<(lineNum-2);i++)
	{
		dataNewSmooth[i-2].X = dataNew[i].X;
		dataNewSmooth[i-2].Y = ( -3*dataNew[i-2].Y + 12* dataNew[i-1].Y + 17 * dataNew[i].Y + 
			12 * dataNew[i+1].Y - 3 * dataNew[i+2].Y)/35;
	}

	lineNum = lineNum - 4;
	
	//GenerateFileName( szFileNameSmooth );
	//WriteFileInfo( lineNum , dataNewSmooth , szFileNameSmooth );

	return;
}