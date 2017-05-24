/****************************************************************** 
File name: Map.c
Date: 2015/8/9
Description: 地图相关函数 
*******************************************************************/
#include "maze.h"

/************************************************************************/
/*函数原型：int partW()													*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：单个墙纹理的宽度											*/                                                                   
/*函数功能：获取单个墙纹理的宽度										*/                                                                   
/************************************************************************/
int partW(){

	return theApp.partLT.mWidth;
}   

/************************************************************************/
/*函数原型：int partH()													*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：单个墙纹理的高度											*/                                                                   
/*函数功能：获取单个墙纹理的高度										*/                                                                   
/************************************************************************/
int partH(){

	return theApp.partLT.mHeight/theApp.npartSprite;
}

/************************************************************************/
/*函数原型：void initGraph(int row,int col)								*/                                                                   
/*传入参数：图的行数，图的列数 											*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：初始化图的邻接矩阵，上下左右各有一个相邻节点				*/                                                                   
/************************************************************************/
void initGraph(int row,int col){

	int i,j,vertexCur; 
	int vertex=row*col;

	for (i=0;i<vertex;i++) {
		for (j=0;j<vertex;j++){
			theApp.MGraph[i][j]=MAXCOST;
		} 
	} 
	for(i=0;i<row;i++){
		for (j=0;j<col;j++){
			//除非边界，否则上下左右均有相邻节点（每个节点代表一个空位）
			vertexCur=i*col+j;
			if (i>0)
				theApp.MGraph[vertexCur][vertexCur-col]=1; 
			if (i<row-1)
				theApp.MGraph[vertexCur][vertexCur+col]=1; 
			if (j>0)
				theApp.MGraph[vertexCur][vertexCur-1]=1; 
			if (j<col-1)
				theApp.MGraph[vertexCur][vertexCur+1]=1; 
		} 
	} 	  
}
 
/************************************************************************/
/*函数原型：int* primTree(int vertex,int col)							*/                                                                   
/*传入参数：图的节点总数，图每行的列数									*/                                                                   
/*返 回 值：生成树父子关系数组											*/                                                                   
/*函数功能：通过prim算法生成随机树										*/                                                                   
/************************************************************************/
int* primTree(int vertex,int col){

	int *lowcost=(int *)malloc(vertex*sizeof(int));//每个顶点到当前生成树的距离 
	int *mst=(int *)malloc(vertex*sizeof(int));//mst[i]为节点i在生成树中的父节点
	int *joinable=(int *)malloc(vertex*sizeof(int));//当前可以加入到生成树的顶点（因为每条边长都是1，所以一次可以有多条） 
	int i, j, temp,minid,m=0;	//minid当前选入生成树的节点，m表示joinable[]中节点个数 
	srand(time(NULL));

	for (i = 1; i < vertex; i++)  
	{  
		lowcost[i] = MAXCOST;  //设lowcost[i]为极大
		mst[i] = 0;  
	}  
	//将节点0加入生成树中
	mst[0] = -1;   
	minid = 0;
	lowcost[0]=0;
	//每次循环为生成树并一个新结点
	for (i = 1; i < vertex; i++)
	{    
		for (j = 1; j <= 4; j++)  
		{  //验证上下左右四个方向的节点是否可以加入joinable
			switch (j)
			{
			case 1:
				temp=minid-col;
				break;
			case 2:
				temp=minid-1;
				break;
			case 3:
				temp=minid+1;
				break;
			case 4:
				temp=minid+col;
				break;
			}
			if (temp<0 || temp>=vertex)//不存在temp节点则退出本次循环
				continue;
			if (theApp.MGraph[minid][temp] < lowcost[temp])  
			{	//若将该节点未加入joinable，则入joinable数组
				lowcost[temp] = theApp.MGraph[minid][temp]; 
				joinable[m++]=temp;   
				mst[temp]=minid;
			}   
			else if (theApp.MGraph[minid][temp] == lowcost[temp] && rand()%2==0)//)
			{
				//若该节点已经加入joinable,还未并入生成树，随机决定是否改变其父节点(增加迷宫地图的随机性)
				mst[temp] = minid;  
			}  
		}  
		//从joinable中随机选取一个节点并入生成树（设lowcost为0）
		temp=rand()%m;
		minid=joinable[temp]; 
		memcpy(&joinable[temp], &joinable[temp+1], sizeof(int)*(m-1-temp));
		m--;
		lowcost[minid] = 0;  
	}   
	//释放临时数组
	free(lowcost);
	free(joinable);
	 
	return mst; 
}
 
/************************************************************************/	
/*函数原型：void setGameBoard(int mst[],int n)							*/                                                                   
/*传入参数：生成树节点的父子关系数组MST，节点数n						*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：由生成树设定gameboard数组									*/                                                                   
/************************************************************************/
void setGameBoard(int *mst,int row,int col){

	int i,j,temp;
	int r,c;
	int k=0;
	 
	//初始化GameBoard
	for (i=0;i<=2*row;i++)
	{
		for (j=0;j<=2*col;j++)
		{ 
			if (i%2==1 && j%2==1) 
				theApp.GameBoard[i][j]=BLANK; 
			else 
				theApp.GameBoard[i][j]=WALL; 
				
		}
	}  
	//生成树节点间有通路则设为BLANK 
	for (i=1;i<row*col;i++)
	{
		temp=mst[i]-i;
		//节点i对应gameboard二维数组的坐标为r,c
		r=(i/row)*2+1;
		c=(i%col)*2+1;
		//把介于两节点之间的位置（原为墙壁）设置为空地
		if (temp==-col)
			theApp.GameBoard[r-1][c]=BLANK;
		else if (temp==-1)
			theApp.GameBoard[r][c-1]=BLANK;
		else if (temp==1)
			theApp.GameBoard[r][c+1]=BLANK;
		else if (temp==col)
			theApp.GameBoard[r+1][c]=BLANK;   
	}   
}

/************************************************************************/
/*函数原型：void setTiles(int row,int col)								*/                                                                   
/*传入参数：图的行数，图的列数											*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：根据迷宫数组设定墙的包围盒									*/                                                                   
/************************************************************************/
void setTiles(int row,int col){
	int i,j,k=0;
	for (i=0;i<=row*2;i++)
	{
		for (j=0;j<=col*2;j++)
		{
			if (theApp.GameBoard[i][j]==WALL)
			{	//根据GameBoard数组，设定墙的包围盒组 
				MakeRect(&theApp.tiles[k++],j*partW(),(i+1)*partH(),partW(),partH());
			} 
		}
	} 
}
  
/************************************************************************/
/*函数原型：void setDoors(int row,int col)								*/                                                                   
/*传入参数：邻接矩阵行数，列数											*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：设定出入口的位置											*/                                                                   
/************************************************************************/
void setDoors(int row,int col){
	int k;
	while (TRUE)
	{
		//入口随机放在屏幕第二列的某个空位上
		k=rand()%(2*row)+1;
		if (theApp.GameBoard[k][1]==BLANK)
		{
			MakeRect(&theApp.entryRect,partW(),(k+1)*partH(),partW(),partH());
			//将南瓜放在入口位置
			MakeRect(&theApp.pumpkin.rect,partW(),(k+1)*partH(),partW(),partH());
			break;
		}
	}
	while (TRUE)
	{
		//出口随机放在屏幕倒数第二列的某个空位上
		k=rand()%(2*row)+1;
		if (theApp.GameBoard[k][2*col-1]==BLANK)
		{
			MakeRect(&theApp.exitRect,SCREEN_WIDTH-2*partW(),(k+1)*partH(),partW(),partH());
			break;
		}
	}

}

/************************************************************************/
/*函数原型：void generateMap(int row,int col)							*/                                                                   
/*传入参数：图的行数，图的列数											*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：生成随机迷宫，设定墙体包围盒								*/                                                                   
/************************************************************************/
void generateMap(int row,int col){

	int *mst=NULL;
	int vertex=row*col;
	//初始化邻接矩阵
	initGraph(row,col);
	//生成代表通路的随机树
	mst=primTree(vertex,col);
	//设定地图标志数组
	setGameBoard(mst,row,col);
	free(mst);
	//设定墙体包围盒组
	setTiles(row,col);
	//设定出入口的包围盒
	setDoors(row,col);
	 
}

/********************************************************************************/
/*函数原型：BOOL touchesWall( SDL_Rect box, SDL_Rect tiles[], int totle_tiles )	*/                                                                   
/*传入参数：包围盒box,墙的包围盒组tiles,墙的包围盒个数							*/                                                                   
/*返 回 值：碰撞返回TRUE,否则返回FALSE											*/                                                                   
/*函数功能：判断包围盒box是否与墙体碰撞											*/                                                                   
/********************************************************************************/
BOOL touchesWall( SDL_Rect box, SDL_Rect tiles[], int totle_tiles ){
	int i;
	for (i=0;i<totle_tiles;i++)
	{ 
		 //如果包围盒碰上墙壁
		if( checkCollision( box, tiles[ i ]) )
		{  
			return TRUE;   
		} 	
	} 
	return FALSE;
}
  
/************************************************************************/	
/*函数原型：void resetGameBoard()										*/                                                                   
/*传入参数：无															*/                                                                   
/*函数功能：将GameBoard中值为STEP的重设为BLANK							*/                                                                   
/************************************************************************/
void resetGameBoard(int row,int col){
	int i,j;
	for (i=0;i<=2*row;i++)
	{
		for (j=0;j<=2*col;j++)
		{
			//删除南瓜自动播放时的足迹
			if (theApp.GameBoard[i][j]==STEP) 
				theApp.GameBoard[i][j]=BLANK;  

		}
	} 

}