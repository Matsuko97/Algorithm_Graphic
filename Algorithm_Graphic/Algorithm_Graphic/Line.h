#ifndef _LINE_H_
#define _LINE_H_

#define CURVE  0
#define DOT    1

class Line
{
public:
	Line();
	~Line();

	//�������
public:
	FileInfo data;
	int PointNum;
	enum {NOT_START , ORIGIN , SMOOTH , SZA_G , SZA_SW , AMPLITUDE, SELECT , DELECT} LineName;
	//����������������ͬ���ߺ�Setting,Delete

	//��ͼ���
public:
	int LineWidth;			//�߿�
	COLORREF LineColor;		//�ߵ���ɫ
	int PenStyle;			//����
	int type;				//ͼ���� CURVE����ͼ��DOTɢ��ͼ

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