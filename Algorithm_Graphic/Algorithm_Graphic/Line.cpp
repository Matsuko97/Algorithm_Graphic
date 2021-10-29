#include "include.h"

//LineList Head = NULL, Rear = NULL;

Line::Line()
{
	data = NULL;
	PointNum = lineNum;
	LineWidth = 1;
	LineColor = BLACK;
	PenStyle = PS_SOLID;
	type = CURVE;
	LineName = NOT_START;
}

Line::~Line()
{

}


//void RecordLineList( LineList &Head , LineList &Rear , Line *curline )
//{
//	Line *tempLine = curline;
//
//	if(Head==NULL)
//	{
//		Head = (LineList)malloc(sizeof(Node));
//		Head->line = tempLine;
//		Head->next = NULL;
//
//		Rear = Head;
//	}
//	else
//	{
//		LineList temp = (LineList)malloc(sizeof(Node));
//		temp->line = tempLine;
//		temp->next = NULL;
//
//		Rear->next = temp;
//		Rear=temp;
//	}
//
//	return;
//}