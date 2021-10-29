#ifndef _LINE_H_
#define _LINE_H_

#define CURVE  0
#define DOT    1

#define BLACK		RGB(0,0,0)
#define RED			RGB(255,0,0)
#define ORANGE		RGB(250,128,114)
#define YELLOW		RGB(255,255,0)
#define GREEN		RGB(0,150,0)
#define LTBLUE		RGB(0,210,210)
#define BLUE		RGB(0,43,150)
#define PINK		RGB(220,94,133)
#define PURPLE		RGB(160,32,240)

enum LINENAME{NOT_START , ORIGIN , SMOOTH , SZA_G , SZA_SW , AMPLITUDE,
	FILTERAMP, FILTERAVG, FILTERMEDIAN, FILTERDEBNC, SELECT , DELECT};

class Line
{
public:
	Line();
	~Line();

	//数据相关
public:
	FileInfo data;
	int PointNum;
	LINENAME LineName;
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