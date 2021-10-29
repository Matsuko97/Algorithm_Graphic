#include "include.h"

TCHAR szAmpFilter[128] = {TEXT("AmpLimiterFilter")};

void AmplitudeLimiterFilter( fileInfo *data ,int Num )

{
	FileInfo dataNew = (fileInfo*)calloc( Num , sizeof(fileInfo) );
	double temp = 0;
	double B = 0;
	
	double start = data[0].Y;
	for( int i=0; i < Num; i++ )
	{
		dataNew[i].X = data[i].X;
		B = start - data[i].Y;
		if( fabs(B) > VARIATION_RANGE )
			dataNew[i].Y = start;
		else
			(dataNew[i].Y) = (data[i].Y);
	}

	GenerateFileName( szAmpFilter );
	WriteFileInfo( Num , dataNew , szAmpFilter );

	free(dataNew);

	return ;
}