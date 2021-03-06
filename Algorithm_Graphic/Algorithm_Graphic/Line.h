#ifndef _LINE_H_
#define _LINE_H_

#define CURVE  0
#define DOT    1

#define BLACK		RGB(0,0,0)
#define RED			RGB(255,0,0)
#define ORANGE		RGB(250,128,114)
#define YELLOW		RGB(220,220,0)
#define GREEN		RGB(0,150,0)
#define LTBLUE		RGB(0,210,210)
#define BLUE		RGB(0,0,255)
#define PINK		RGB(220,94,133)
#define PURPLE		RGB(160,32,240)
#define LTPURPLE	RGB(117,117,227)
#define LTPINK		RGB(255,184,232)
#define DKGREEN		RGB(63,183,136)

enum LINENAME{NOT_START , ORIGIN , SMOOTH , SZA_G , SZA_SW , AMPLITUDE, COMBINEDSZA,
	FILTERAMP, FILTERAVG, FILTERMEDIAN, FILTERDEBNC, FILTERRECURSIVE, 
	BASESTRAIGHT, SNIP, SELECT , DELECT};

class Line
{
public:
	Line();
	~Line();

	//数据相关
public:
	FileInfo data;
	int PointNum;
	TCHAR szName[128];
	LINENAME LineName;		//曲线名，用于区别不同曲线和Setting,Delete

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