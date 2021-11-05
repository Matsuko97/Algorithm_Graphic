#ifndef _SNIP_H_
#define _SNIP_H_

#define FixedWidth		51

void BasicSNIP( fileInfo *dataNew, int num , int m , int i , bool type);
void ImprovedSNIP(fileInfo *dataNew, int num , LinkList Head);
void ObtainInfo(LinkList &p , int &start , int &end , int &width , int num);

extern TCHAR szSNIPBase[128];

#endif