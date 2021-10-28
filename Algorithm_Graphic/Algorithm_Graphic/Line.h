#ifndef _LINE_H_
#define _LINE_H_

#define CURVE  0
#define DOT    1

class Line
{
public:
	Line();
	~Line();

	//数据相关
public:
	FileInfo data;
	int PointNum;
	enum {NOT_START , ORIGIN , SMOOTH , SZA_G , SZA_SW , AMPLITUDE, SELECT , DELECT} LineName;
	//曲线名，用于区别不同曲线和Setting,Delete

	//画图相关
public:
	int LineWidth;			//线宽
	COLORREF LineColor;		//线的颜色
	int PenStyle;			//线型
	int type;				//图类型 CURVE线型图；DOT散点图

//public:
//	BOOL IsSelect();
//	void DrawSquare();
//	void MarkSelected();

};

typedef struct LINE
{
	Line line;
	struct LINE *next;
}*LineList,LineNode;

//extern LineList Head;
//extern LineList Rear;

//void RecordLineList( LineList &Head , LineList &Rear , Line *curline );

#endif