/****************************************************************** 
Author: 刘新国，郑剑华
EMAIL：xgliu@cad.zju.edu.cn
*******************************************************************/
#include "miner.h"

/************************************************************************/
/*函数原型：void GenerateRandomBombs(MineField *fld)					*/                                                                   
/*传入参数：雷区的指针													*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：在雷块数组中随机生成地雷的位置								*/                                                                   
/************************************************************************/
void GenerateRandomBombs(MineField *fld)
{
	int N = fld->fldSize[2];
	int n = fld->bombCount, j, k;
	MineCell *p = fld->pCells;
	 
	if( !p ) return;
	memset(p,0,sizeof(MineCell)*N);
	srand((int)time(NULL));
	for( j = 0; j < n; j++ )
	{
		do { 
			k = rand()%N;
		} while( IsBomb(p[k].data) );
		p[k].data = BOMB;
	}
}

/************************************************************************/
/*函数原型：void CalcNeighbors(MineField *fld)							*/                                                                   
/*传入参数：雷区的指针													*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：地雷位置确定后，计算每个位置周围地雷的数量					*/                                                                   
/************************************************************************/
void CalcNeighbors(MineField *fld)
{
	int SX = fld->fldSize[0];
	int SY = fld->fldSize[1];
	int x, y, nx, ny, sum;
	MineCell *p = fld->pCells;
	for( x=0; x<SX; x++ ) for( y=0; y<SY; y++ )
	{
		if( IsBomb(p[FlatIndex(x,y,SX)].data) )
			continue;
		sum = 0; 
		for( nx = x-1; nx<x+2; nx++ ) for( ny=y-1; ny<y+2; ny++ )
		{
			if( nx==x && ny==y ) continue;
			if( nx<0 || nx>=SX ) continue;
			if( ny<0 || ny>=SY ) continue;
			sum += IsBomb(p[FlatIndex(nx,ny,SX)].data);
		}
		p[FlatIndex(x,y,SX)].data = (char)sum;
	}
}

/********************************************************************************************/
/*函数原型：BOOL ConstructField(MineField * fld, int fldSizeX, int fldSizeY, int bombCount)	*/                                                                   
/*传入参数：雷区的指针，雷区宽度，高度（以一个雷块为单位），地雷个数						*/                                                                   
/*返 回 值：构建成功返回TRUE,否则返回FALSE													*/                                                                   
/*函数功能：构建雷区，申请雷区所有雷块所需内存												*/                                                                   
/********************************************************************************************/
BOOL ConstructField(MineField * fld, int fldSizeX, int fldSizeY, int bombCount)
{
	if( fldSizeX<0 || fldSizeY<0 || fldSizeX>MAX_FIELD || fldSizeY>MAX_FIELD )
	{
		printf("Mine Field is too big to construct\n");
		return FALSE;
	}
	if( bombCount<0 || bombCount>fldSizeX*fldSizeY/2 )
	{
		printf("Too many bombs to construct\n");
		return FALSE;
	}

	fld->fldSize[0] = fldSizeX;
	fld->fldSize[1] = fldSizeY;
	fld->fldSize[2] = fldSizeX * fldSizeY;
	fld->bombCount = bombCount;
	if( ! (fld->pCells = (MineCell*)calloc(fldSizeX*fldSizeY, sizeof(MineCell))) )
	{
		printf("Failed to allocate mine field\n");
		return FALSE;
	}
	return TRUE;
}

/************************************************************************/
/*函数原型：int CellFinalVisualIndex(MineCell p)						*/                                                                   
/*传入参数：雷块														*/                                                                   
/*返 回 值：显示标志													*/                                                                   
/*函数功能：返回游戏结束时该雷块的显示标志（用于设定纹理）				*/                                                                   
/************************************************************************/
int CellFinalVisualIndex(MineCell p)
{
	if( IsBomb(p.data) )
	{
		if( p.state==csRevealed || p.state==csAutomated ) return cvRedBomb;
		if( p.state==csBombFlag)  return cvFlag;
		return cvBomb;
	}
	if( p.state==csBombFlag) return cvWrongBomb;
	if( p.state==csQuestioned ) return cvQsMark;
	if( p.state==csRevealed || p.state==csAutomated )
		return 15-p.data;
	return cvCovered;
}

/************************************************************************/
/*函数原型：int CellVisualIndex(MineCell p)								*/                                                                   
/*传入参数：雷块														*/                                                                   
/*返 回 值：显示标志													*/                                                                   
/*函数功能：根据雷块当前状态返回显示标志（用于设定纹理）				*/                                                                   
/************************************************************************/
int CellVisualIndex(MineCell p)
{
	switch( p.state )
	{
	case csCovered:    return 0;
	case csBombFlag:   return 1;
	case csQuestioned: return 2;
	case csPressed:    return 15;
	case csPressedQ:   return 6;
	case csRevealed:
	case csAutomated:   
		if( IsBomb(p.data) ) return 5;
		return (15-p.data);
	}
	return 0;
}

/************************************************************************/
/*函数原型：void DestroyField(MineField *fld)							*/                                                                   
/*传入参数：指向雷区的指针												*/                                                                   
/*返 回 值：无															*/                                                                   
/*函数功能：释放雷区的内存												*/                                                                   
/************************************************************************/
void DestroyField(MineField *fld)
{
	if( fld->pCells ) 
		free(fld->pCells);
	memset(fld,0,sizeof(MineField));
}

/************************************************************************/
/*函数原型：BOOL GameOver()												*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：游戏失败返回TRUE,否则返回FALSE								*/                                                                   
/*函数功能：左键在地雷处释放则游戏失败									*/                                                                   
/************************************************************************/
BOOL GameOver()
{
	MineField *fld = & theApp.mineField;
	return ( fld->state==sGameOver );
}

/************************************************************************/
/*函数原型：MineCell* GetCellAt(int x, int y)							*/                                                                   
/*传入参数：X坐标,Y坐标													*/                                                                   
/*返 回 值：指向坐标所在雷块的指针										*/                                                                   
/*函数功能：计算坐标所处的雷块											*/                                                                   
/************************************************************************/
MineCell* GetCellAt(int x, int y)
{
	MineField *fld = & theApp.mineField;
	int cx = (x - fld->rect.x)/CellsWidth();
	int cy = (y - fld->rect.y)/CellsHeight();

	if( InRange(cx,0,fld->fldSize[0]) && InRange(cy,0,fld->fldSize[1]) )
		return (theApp.mineField.pCells+FlatIndex(cx,cy,fld->fldSize[0]));
	return NULL;
}

/************************************************************************/
/*函数原型：MineCell* GetFocusCell()									*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：指向当前活跃雷块的指针  									*/                                                                   
/*函数功能：获取指向当前鼠标活动所处雷块的指针							*/                                                                   
/************************************************************************/ 
MineCell* GetFocusCell()
{
	return theApp.mineField.pFocus;
}

/************************************************************************/
/*函数原型：MineCell* SetFocusCell(MineCell *pCell)						*/                                                                   
/*传入参数：雷块的指针													*/                                                                   
/*返 回 值：雷块的指针  												*/                                                                   
/*函数功能：设定pFocuse指针为pCell										*/                                                                   
/************************************************************************/ 
MineCell* SetFocusCell(MineCell *pCell)
{
	return (theApp.mineField.pFocus=pCell);
}
 
/************************************************************************/
/*函数原型：void OpenCellBlock(MineCell *pCenCell)						*/                                                                   
/*传入参数：雷块的指针													*/                                                                   
/*返 回 值：无			  												*/                                                                   
/*函数功能：指定雷块周围八个位置中需要自动打开的则打开它们				*/                                                                   
/************************************************************************/ 
void OpenCellBlock(MineCell *pCenCell)
{
	MineField *fld = & theApp.mineField;
	Coord2D cd = CellCoord(pCenCell);
	int x, y, bombSum = 0;
	MineCell *p;

	if( !pCenCell ) return;
	if( pCenCell->state!=csRevealed && pCenCell->state!=csAutomated )
		return;
	// checking the condition
	for( x = cd.x-1; x < cd.x+2; x++ )
		for( y = cd.y-1; y < cd.y+2; y++ )
		{
			if( !InRange(x,0,fld->fldSize[0]) || !InRange(y,0,fld->fldSize[1]) )
				continue;
			p = fld->pCells + FlatIndex(x,y,fld->fldSize[0]);
			if( p->state==csBombFlag )
				bombSum++;
		}
		// do NOT match the condition
		if( bombSum != pCenCell->data )
			return; // can not open the 3x3 block

		// set the state to avoid repeating on this cell
		if( pCenCell->state!=csAutomated )	
			pCenCell->state = csAutomated;
		// open the block
		for( x = cd.x-1; x < cd.x+2; x++ )
			for( y = cd.y-1; y < cd.y+2; y++ )
			{
				if( !InRange(x,0,fld->fldSize[0]) || !InRange(y,0,fld->fldSize[1]) )
					continue;
				OpenCell( fld->pCells + FlatIndex(x,y,fld->fldSize[0]) );
			}
}
 
/************************************************************************/
/*函数原型：void PressCell(MineCell *pCell)								*/                                                                   
/*传入参数：雷块的指针													*/                                                                   
/*返 回 值：无  														*/                                                                   
/*函数功能：雷块未打开，则按下时修改其状态为csPressed					*/                                                                   
/************************************************************************/
void PressCell(MineCell *pCell)
{
	if( !pCell ) return;
	if( pCell->state==csPressed )
		return;
	if( pCell->state==csCovered )
		pCell->state = csPressed;
	else if( pCell->state==csQuestioned )
		pCell->state = csPressedQ; 
}

/************************************************************************/
/*函数原型：void MarkCell(MineCell *pCell)								*/                                                                   
/*传入参数：雷块的指针													*/                                                                   
/*返 回 值：无  														*/                                                                   
/*函数功能：右键标记雷块时修改其数据									*/                                                                   
/************************************************************************/
void MarkCell(MineCell *pCell)
{
	if( !pCell ) return;

	switch( pCell->state )
	{
	case csCovered:
		pCell->state = csBombFlag;
		postUserEvent(evtMarkMore);
		break;
	case csBombFlag:
		pCell->state = csQuestioned;
		postUserEvent(evtMarkLess);
		break;
	case csQuestioned:
		pCell->state = csCovered;
		break;
	default:
		return;
	} 
}

/************************************************************************/
/*函数原型：void OpenCell(MineCell *pCell)								*/                                                                   
/*传入参数：雷块的指针													*/                                                                   
/*返 回 值：无  														*/                                                                   
/*函数功能：初次显示雷块真实形态时的处理函数							*/                                                                   
/************************************************************************/
void OpenCell(MineCell *pCell)
{
	if( ! pCell ) return;
	if( pCell->state==csBombFlag  )
		return;
	if( IsBomb(pCell->data) )
	{
		pCell->state = csRevealed;
		Mix_PlayChannel( -1, theApp.audio_bomb, 0 ); 
		postUserEvent(evtStopTimer);
		postUserEvent(evtGameFail);
		return;
	}
	if( pCell->state!=csRevealed && pCell->state!=csAutomated)   
		pCell->state = csRevealed; // the cell is not opened

	if( pCell->data==0 && pCell->state!=csAutomated ) 
	{   // there is no bomb around
		Mix_PlayChannel( -1, theApp.audio_reveal, 0 ); 
		pCell->state = csAutomated;
		// automatically open the neighbor 3x3 block
		OpenCellBlock(pCell); 
	}
}
 
/************************************************************************/
/*函数原型：void ReleaseCell(MineCell *pCell)							*/                                                                   
/*传入参数：雷块的指针													*/                                                                   
/*返 回 值：无  														*/                                                                   
/*函数功能：在雷块处释放时修改其状态									*/                                                                   
/************************************************************************/
void ReleaseCell(MineCell *pCell)
{
	if( !pCell ) return;
	if( pCell->state == csPressed ) 
		pCell->state = csCovered;  
	else if( pCell->state == csPressedQ ) 
		pCell->state = csQuestioned;  
}

/************************************************************************/
/*函数原型：void ReleaseCellBlock(MineCell *pCell)						*/                                                                   
/*传入参数：雷块的指针													*/                                                                   
/*返 回 值：无  														*/                                                                   
/*函数功能：释放指定雷块及周围八个位置的雷块							*/                                                                   
/************************************************************************/
void ReleaseCellBlock(MineCell *pCell)
{
	MineField *fld = & theApp.mineField;
	Coord2D cen = CellCoord(pCell);
	int x, y;

	if( !pCell ) return;
	for( x = cen.x-1; x<cen.x+2; x++ )
		for( y = cen.y-1; y<cen.y+2; y++ )
		{
			if( !InRange(x,0,fld->fldSize[0]) || !InRange(y,0,fld->fldSize[1]) )
				continue;
			pCell = fld->pCells+( FlatIndex(x,y,fld->fldSize[0]) );
			ReleaseCell(pCell);
		}
}

/************************************************************************/
/*函数原型：void PressCellBlock(MineCell *pCell)						*/                                                                   
/*传入参数：雷块的指针													*/                                                                   
/*返 回 值：无  														*/                                                                   
/*函数功能：尝试将指定雷块及周围八个位置全部修改为按下状态				*/                                                                   
/************************************************************************/
void PressCellBlock(MineCell *pCell)
{
	MineField *fld = & theApp.mineField;
	Coord2D cen = CellCoord(pCell);
	int x, y;

	if( !pCell ) return;
	for( x = cen.x-1; x<cen.x+2; x++ )
		for( y = cen.y-1; y<cen.y+2; y++ )
		{
			if( !InRange(x,0,fld->fldSize[0]) || !InRange(y,0,fld->fldSize[1]) )
				continue;
			pCell = fld->pCells+( FlatIndex(x,y,fld->fldSize[0]) );
			PressCell(pCell);
		}
}

/************************************************************************/
/*函数原型：void LeftPressField(int x, int y, unsigned char flags)		*/                                                                   
/*传入参数：X坐标，Y坐标，备用参数flags									*/                                                                   
/*返 回 值：无  														*/                                                                   
/*函数功能：处理鼠标在雷区左键按下事件									*/                                                                   
/************************************************************************/
void LeftPressField(int x, int y, unsigned char flags)
{
	MineField *fld = & theApp.mineField;
	MineCell *pOld = GetFocusCell();
	MineCell *pNow = GetCellAt(x,y);

	switch ( fld->state )
	{
	case sNewGame:
		// start timing
		postUserEvent(evtStartTimer);
	case sWait:	// waiting for user input
		// chane to the state where the left button is down
		fld->state = sLeftDown;
		SetFocusCell(pNow);
		PressCell(pNow);
		break;
	case sRightDown:
		fld->state = sLRDown;
		SetFocusCell(pNow);
		PressCellBlock(pNow);
		break;
	case sGameOver:
		break;
	}
}
 
/************************************************************************/
/*函数原型：void LeftReleaseField(int x, int y, unsigned char flags)	*/                                                                   
/*传入参数：X坐标，Y坐标，备用参数flags									*/                                                                   
/*返 回 值：无  														*/                                                                   
/*函数功能：处理鼠标在雷区左键释放事件									*/                                                                   
/************************************************************************/
void LeftReleaseField(int x, int y, unsigned char flags)
{
	MineField *fld = & theApp.mineField;
	MineCell *pOld = GetFocusCell();
	MineCell *pNow = GetCellAt(x,y);

	switch ( fld->state )
	{
	case sLeftDown:	// waiting for user input
		// chane to the state where the left button is down
		fld->state = sWait;
		OpenCell(pNow);
		break;
	case sLRDown:
		fld->state = sWait;
		ReleaseCellBlock(pOld);
		OpenCellBlock(pNow);
		break;
	case sGameOver:
		break;
	}
}

/************************************************************************/
/*函数原型：void RightPressField(int x, int y, unsigned char flags)		*/                                                                   
/*传入参数：X坐标，Y坐标，备用参数flags									*/                                                                   
/*返 回 值：无  														*/                                                                   
/*函数功能：处理鼠标在雷区右键按下事件									*/                                                                   
/************************************************************************/
void RightPressField(int x, int y, unsigned char flags)
{
	MineField *fld = & theApp.mineField;
	MineCell *pOld = GetFocusCell();
	MineCell *pNow = GetCellAt(x,y);

	switch ( fld->state )
	{
	case sNewGame:
		// start timing
		postUserEvent(evtStartTimer);
	case sWait:	// waiting for user input
		// chane to the state where the left button is down
		fld->state = sRightDown;
		SetFocusCell(pNow);
		MarkCell(pNow);
		break;
	case sLeftDown:
		fld->state = sLRDown;
		SetFocusCell(pNow);
		PressCellBlock(pNow);
		break;
	}
}

/************************************************************************/
/*函数原型：void RightReleaseField(int x, int y, unsigned char flags)	*/                                                                   
/*传入参数：X坐标，Y坐标，备用参数flags									*/                                                                   
/*返 回 值：无  														*/                                                                   
/*函数功能：处理鼠标在雷区右键释放事件									*/                                                                   
/************************************************************************/
void RightReleaseField(int x, int y, unsigned char flags)
{
	MineField *fld = & theApp.mineField;
	MineCell *pOld = GetFocusCell();
	MineCell *pNow = GetCellAt(x,y);

	switch ( fld->state )
	{
	case sRightDown:
		fld->state = sWait;
		break;
	case sLRDown:
		fld->state = sWait;
		ReleaseCellBlock(pOld);
		OpenCellBlock(pNow);
		break;
	case sLeftDown:
		fld->state = sLRDown;
		PressCellBlock(pNow);
		break;
	}
}

/************************************************************************/
/*函数原型：void MoveOnField(int x, int y, unsigned char flags)			*/                                                                   
/*传入参数：X坐标，Y坐标，备用参数flags									*/                                                                   
/*返 回 值：无  														*/                                                                   
/*函数功能：处理鼠标在雷区移动事件										*/                                                                   
/************************************************************************/
void MoveOnField(int x, int y, unsigned char flags)
{
	MineField *fld = & theApp.mineField;
	MineCell *pOld = GetFocusCell();
	MineCell *pNow = GetCellAt(x,y);
	if( pNow==pOld ) return;

	switch ( fld->state )
	{
	case sLeftDown:
		ReleaseCell(pOld);
		SetFocusCell(pNow);
		PressCell(pNow);
		break;
	case sLRDown:
		ReleaseCellBlock(pOld);
		SetFocusCell(pNow);
		PressCellBlock(pNow);
		break;
	case sRightDown:
		SetFocusCell(pNow);
		break;
	case sGameOver:
		break;
	}
}

/************************************************************************/
/*函数原型：BOOL MissionCompleted()										*/                                                                   
/*传入参数：无															*/                                                                   
/*返 回 值：游戏任务完成返回TRUE,否则返回FALSE							*/                                                                   
/*函数功能：判断游戏任务是否已经完成									*/                                                                   
/************************************************************************/
BOOL MissionCompleted()
{
	MineField *fld = & theApp.mineField;
	MineCell *p = fld->pCells;
	int N = fld->fldSize[2];
	int sum = 0;

	if( theApp.taskButton.data>0 )
		return 0;

	while( N-->0 )
	{
		if( p->state==csRevealed || p->state==csAutomated )
			sum ++;
		p++;
	}
	return ( sum + fld->bombCount == fld->fldSize[2] );
}
