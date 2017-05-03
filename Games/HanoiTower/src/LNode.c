/****************************************************************** 
File name: LNode.c
Date: 2015/7/31
Description: 链表相关函数 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/
#include "hanoi.h"


/************************************************************************/
/*函数原型：void MakeNode(int index)									*/                                                                   
/*传入参数：结点数据index												*/                                                                   
/*返 回 值：无													   		*/                                                                   
/*函数功能：创建一个数据为index的结点									*/                                                                   
/************************************************************************/
LNode* MakeNode(int index){
	LNode *node;
	node=(LNode *)malloc(sizeof(LNode));
	node->index=index;
	node->next=NULL;  
	return node;
}

/************************************************************************/
/*函数原型：void AddNode(LNode* header,LNode *node)						*/                                                                   
/*传入参数：链表头结点指针，待入链结点指针								*/                                                                   
/*返 回 值：无													   		*/                                                                   
/*函数功能：在链表头加入一个新结点										*/                                                                   
/************************************************************************/	
void AddNode(LNode* header,LNode *node){
	
	if (!header||!node)
		return; 
	node->next=header->next;
	header->next=node;
	header->index++;
}

/************************************************************************/
/*函数原型：void RejoinNode(int x,int y)								*/                                                                   
/*传入参数：X坐标,Y坐标													*/                                                                   
/*返 回 值：空													   		*/                                                                   
/*函数功能：在指定位置释放时，查找链表，将移动结点放回					*/                                                                   
/************************************************************************/
void RejoinNode(int x,int y){

	LNode* header=FindTouchedTower(x,y,theApp.discFocus.node->index); 
	 
	if (!header)  
		header=theApp.discFocus.h;  
		  
	if (header!=theApp.discFocus.h)
		incMove();
	  
	AddNode(header,theApp.discFocus.node);
	theApp.discFocus.node=NULL;
	theApp.discFocus.h=NULL; 
}

/************************************************************************/
/*函数原型：void RejoinNode(int t)										*/                                                                   
/*传入参数：塔t															*/                                                                   
/*返 回 值：空													   		*/                                                                   
/*函数功能：将移动结点加到塔t中											*/                                                                   
/************************************************************************/
void JoinNode(int t){ 
	
	AddNode(theApp.tower[t].header,theApp.discFocus.node);
	theApp.discFocus.node=NULL;
	theApp.discFocus.h=NULL; 
}

/************************************************************************/
/*函数原型：LNode* deleteNode(LNode* header)							*/                                                                   
/*传入参数：链表头结点指针												*/                                                                   
/*返 回 值：被摘下结点											   		*/                                                                   
/*函数功能：摘下链表第一个结点											*/                                                                   
/************************************************************************/	
LNode* deleteNode(LNode* header){
 
	LNode* node;
	if (!header->next)
		return NULL;
	
	node=header->next;
	header->next=node->next;
	node->next=NULL;
	header->index--;

	return node;
}

/************************************************************************/
/*函数原型：void detachNode(LNode* header,int x,int y)					*/                                                                   
/*传入参数：链表头结点指针,X坐标,Y坐标									*/                                                                   
/*返 回 值：空													   		*/                                                                   
/*函数功能：摘下链表第一个结点设为移动结点								*/                                                                   
/************************************************************************/
void detachNode(LNode* header,int x,int y){

	theApp.discFocus.node=deleteNode(header);
	theApp.discFocus.h=header;
	theApp.discFocus.x=x;
	theApp.discFocus.y=y;  
}

/************************************************************************/
/*函数原型：void initList()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无													   		*/                                                                   
/*函数功能：初始化三个链表头结点										*/                                                                   
/************************************************************************/
void initList(){

	int i; 
	for (i=0;i<3;i++) 
		theApp.tower[i].header=MakeNode(0);  
}

/************************************************************************/
/*函数原型：void freeList(LNode* header)								*/                                                                   
/*传入参数：链表头结点指针												*/                                                                   
/*返 回 值：无													   		*/                                                                   
/*函数功能：清空一个链表												*/                                                                   
/************************************************************************/
void freeList(LNode* header){
	
	LNode *node;
	while (header->next)
	{
		node=header->next;
		header->next=node->next;
		free(node);
	}
	header->index=0;
}
 
/************************************************************************/
/*函数原型：void clearLists()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无													   		*/                                                                   
/*函数功能：清空游戏中三个链表											*/                                                                   
/************************************************************************/
void clearLists(){
	int i;
	for (i=0;i<3;i++)
	{
		freeList(theApp.tower[i].header);  
	} 
}

/************************************************************************/
/*函数原型：void clearHeaders()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无													   		*/                                                                   
/*函数功能：释放所有头结点												*/                                                                   
/************************************************************************/
void clearHeaders(){

	int i;
	for (i=0;i<3;i++)
	{
		free(theApp.tower[i].header);
	}
	 
}

/************************************************************************/
/*函数原型：void clearNodes()											*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：无													   		*/                                                                   
/*函数功能：释放所有结点												*/                                                                   
/************************************************************************/
void clearNodes(){

	clearLists();
	clearHeaders();
	if (theApp.discFocus.node)
	{
		free(theApp.discFocus.node);
	}
}