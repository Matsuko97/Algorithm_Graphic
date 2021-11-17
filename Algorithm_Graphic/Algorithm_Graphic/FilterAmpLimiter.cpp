#include "include.h"

double VARIATION_RANGE = 0.5;

TCHAR szAmpFilter[128] = {0};

int AmplitudeLimiterFilter( fileInfo *data ,int Num )
{
	if( data == NULL )
	{
		return -1;
	}

	VARIATION_RANGE = VARIATION_RANGE * Factor ;
	//根据源数据的数量级重新计算VARIATION_RANGE

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
	dataNew = NULL;

	return 0;
}