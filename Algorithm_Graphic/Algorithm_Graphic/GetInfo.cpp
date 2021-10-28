#include "include.h"

int end=1;
int fileend=1;
int lineNum;
LONG offset=0;
LONG WriteOffset=0;

int GetLineNum(const TCHAR* path)
{
	int num = 0;
	char c;
	
	FILE *fp = NULL;
	
	if (_tfopen_s( &fp, path, TEXT("r")))
		return -1;

	while(( c=fgetc(fp)) !=EOF)
	{
		if(c=='\n')
			++num;
		else
		{
			c=fgetc(fp);
			if( c==EOF )
			{
				num++;
				break;
			}
		}
	}

	return num;
}

FileInfo GetData( const TCHAR* path)
{
	lineNum = GetLineNum(path);
	fileInfo *data;
	data = (fileInfo*)calloc( lineNum , sizeof(fileInfo) );

	FILE *fp = NULL;
	
	if (_tfopen_s( &fp, path, TEXT("r")))
		return NULL;

	fseek( fp, offset, SEEK_SET );

	for (int i = 0; i < lineNum; i++)
	{
		fwscanf_s( fp, TEXT("%lf %lf\n"), &data[i].X, &data[i].Y );  
		if(0==i)
			offset = ftell(fp);

		if (feof(fp))  
		{	
			i++;
			offset = 0;
			end = 0;
			break;
		}
	}
	fclose(fp);

	return data;
}