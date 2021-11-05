#include "include.h"

TCHAR szFilterDeBnc[128] = {0};

int DebounceFilter( fileInfo *data ,int Num )
{
	if( data == NULL )
	{
		return -1;
	}

	int n = 0;
	FileInfo dataNew = (fileInfo*)calloc( Num , sizeof(fileInfo) );
	double temp = data[0].Y;

	for(int i=0; i < Num; i++ )
	{
		dataNew[i].X = data[i].X;
		if( temp != (data[i].Y))
		{
			n++;
			if (n>N)
			{
				n=0;
				temp = data[i].Y;
			}
			dataNew[i].Y = temp;
		}
		else 
		{
			n = 0;
			dataNew[i].Y = temp;
		}
	}

	GenerateFileName( szFilterDeBnc );
	WriteFileInfo( Num , dataNew , szFilterDeBnc );

	free(dataNew);
	dataNew = NULL;

	return 0;
}