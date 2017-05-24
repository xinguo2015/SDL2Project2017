/******************************************************************
File name: Bomb.c
Date:	2015/7/23
Description:	炮弹管理类函数
*******************************************************************/
#include "hitword.h"

/************************************************************************/
/*函数原型：int bombX(int bombIndex)									*/
/*传入参数：炮弹的数组下标												*/
/*返 回 值：炮弹的X坐标													*/
/*函数功能：获取指定下标炮弹的X坐标										*/
/************************************************************************/
int bombX(int bombIndex) {

	return theApp.bombs[bombIndex].rect.x;
}

/************************************************************************/
/*函数原型：int bombY(int bombIndex)									*/
/*传入参数：炮弹的数组下标												*/
/*返 回 值：炮弹的Y坐标													*/
/*函数功能：获取指定下标炮弹的Y坐标										*/
/************************************************************************/
int bombY(int bombIndex) {

	return theApp.bombs[bombIndex].rect.y;
}

/************************************************************************/
/*函数原型：int bombW(int bombIndex)									*/
/*传入参数：炮弹的数组下标												*/
/*返 回 值：炮弹包围盒的宽度											*/
/*函数功能：获取指定下标炮弹包围盒的宽度								*/
/************************************************************************/
int bombW(int bombIndex) {

	return theApp.bombs[bombIndex].rect.w;
}

/************************************************************************/
/*函数原型：int bombH(int bombIndex)									*/
/*传入参数：炮弹的数组下标												*/
/*返 回 值：炮弹包围盒的高度											*/
/*函数功能：获取指定下标炮弹包围盒的高度								*/
/************************************************************************/
int bombH(int bombIndex) {

	return theApp.bombs[bombIndex].rect.h;
}

/************************************************************************/
/*函数原型：int bombHeight()											*/
/*传入参数：无															*/
/*返 回 值：炮弹纹理的宽度												*/
/*函数功能：获取炮弹纹理的宽度											*/
/************************************************************************/
int bombWidth() {

	return theApp.gbomb.mWidth;
}

/************************************************************************/
/*函数原型：int bombHeight()											*/
/*传入参数：无															*/
/*返 回 值：炮弹纹理的高度												*/
/*函数功能：获取炮弹纹理的高度											*/
/************************************************************************/
int bombHeight() {

	return theApp.gbomb.mHeight;
}

/************************************************************************/
/*函数原型：int bombVelX(int bombIndex)									*/
/*传入参数：炮弹的数组下标												*/
/*返 回 值：指定下标炮弹X轴方向的速度									*/
/*函数功能：获取指定下标炮弹X轴方向的速度								*/
/************************************************************************/
int bombVelX(int bombIndex) {

	return theApp.bombs[bombIndex].vel.x;
}

/************************************************************************/
/*函数原型：int bombVelY(int bombIndex)									*/
/*传入参数：炮弹的数组下标												*/
/*返 回 值：指定下标炮弹Y轴方向的速度									*/
/*函数功能：获取指定下标炮弹Y轴方向的速度								*/
/************************************************************************/
int bombVelY(int bombIndex) {

	return theApp.bombs[bombIndex].vel.y;
}

/************************************************************************/
/*函数原型：int bombAngle(int bombIndex)								*/
/*传入参数：炮弹的数组下标												*/
/*返 回 值：炮弹的旋转角度												*/
/*函数功能：获取指定下标炮弹的旋转角度									*/
/************************************************************************/
int bombAngle(int bombIndex) {

	return theApp.bombs[bombIndex].angle;
}

/************************************************************************/
/*函数原型：int bombAim(int bombIndex)									*/
/*传入参数：炮弹的数组下标												*/
/*返 回 值：炮弹目标单词的数组下标										*/
/*函数功能：获取指定下标炮弹的目标单词									*/
/************************************************************************/
int bombAim(int bombIndex) {

	return theApp.bombs[bombIndex].aim;
}

/************************************************************************/
/*函数原型：void setBombAngle(int bombIndex,int angle)					*/
/*传入参数：目标炮弹的数组下标,旋转角度angle							*/
/*返 回 值：无															*/
/*函数功能：设定指定下标炮弹的旋转角度（顺时针）						*/
/************************************************************************/
void setBombAngle(int bombIndex, int angle) {

	theApp.bombs[bombIndex].angle = angle;
}

/************************************************************************/
/*函数原型：void setBombVel(int bombIndex,int x,int y)					*/
/*传入参数：目标炮弹的数组下标,x轴方向的速度，y轴方向的速度				*/
/*返 回 值：无															*/
/*函数功能：设定指定下标炮弹的速度										*/
/************************************************************************/
void setBombVel(int bombIndex, int x, int y) {

	theApp.bombs[bombIndex].vel.x = x;
	theApp.bombs[bombIndex].vel.y = y;
}

/************************************************************************/
/*函数原型：void moveBomb(int bombIndex)								*/
/*传入参数：目标炮弹的数组下标											*/
/*返 回 值：无															*/
/*函数功能：移动指定下标的炮弹											*/
/************************************************************************/
void moveBomb(int bombIndex) {

	int x, y;
	theApp.bombs[bombIndex].rect.x += bombVelX(bombIndex);
	theApp.bombs[bombIndex].rect.y += bombVelY(bombIndex);

	if (bombVelX(bombIndex) != 0 && bombAim(bombIndex) >= 0)
	{	//在横坐标方向已经移动到位
		if ((bombX(bombIndex) < wordX(bombAim(bombIndex)) && bombVelX(bombIndex) < 0) || (bombX(bombIndex) > wordX(bombAim(bombIndex)) && bombVelX(bombIndex) > 0))
		{
			x = wordX(bombAim(bombIndex)) + wordWidth(bombAim(bombIndex)) / 2 - bombWidth() / 2;
			y = bombY(bombIndex) - bombW(bombIndex) / 2 + bombH(bombIndex) / 2;
			//开始向上移动
			if (bombAim(bombIndex) >= 0 && bombY(bombIndex) >= wordY(bombAim(bombIndex)))
			{
				setBombVel(bombIndex, 0, -10);
				setBombAngle(bombIndex, 0);
				MakeRect(&theApp.bombs[bombIndex].rect, x, y, bombWidth(), bombHeight());
			}
			else	//开始向下移动
			{
				setBombVel(bombIndex, 0, 10);
				setBombAngle(bombIndex, 180);
				MakeRect(&theApp.bombs[bombIndex].rect, x, y, bombWidth(), bombHeight());
			}
		}
	}
}

/************************************************************************/
/*函数原型：void fire_a_Bomb(int wordIndex)								*/
/*传入参数：目标单词的数组下标											*/
/*返 回 值：无															*/
/*函数功能：发射一颗炮弹												*/
/************************************************************************/
void fire_a_Bomb(int wordIndex) {

	int x, y;
	if (theApp.screen_bombs >= 10)
	{
		printf("Total bomb exceed maximum(10)!\n");
		return;
	}
	Mix_PlayChannel(-1, theApp.wavFire, 0);
	x = theApp.SCREEN_WIDTH / 2 - bombWidth() / 2;
	y = theApp.SCREEN_HEIGHT - UFOHeight();
	theApp.bombs[theApp.screen_bombs].aim = wordIndex;

	if (x == wordX(wordIndex) + wordWidth(wordIndex) / 2 - bombWidth() / 2)//向上发射
	{
		setBombAngle(theApp.screen_bombs, 0);
		setBombVel(theApp.screen_bombs, 0, -10);
		MakeRect(&theApp.bombs[theApp.screen_bombs].rect, x, y, bombWidth(), bombHeight());
	}
	else if (x < wordX(wordIndex) + wordWidth(wordIndex) / 2 - bombWidth() / 2)//向右发射
	{
		setBombAngle(theApp.screen_bombs, 90);
		setBombVel(theApp.screen_bombs, 10, 0);
		x = x + bombWidth() / 2 - bombHeight() / 2;
		y = y + bombHeight() / 2 - bombWidth() / 2;
		MakeRect(&theApp.bombs[theApp.screen_bombs].rect, x, y, bombHeight(), bombWidth());
	}
	else //向左发射
	{
		setBombAngle(theApp.screen_bombs, -90);
		setBombVel(theApp.screen_bombs, -10, 0);
		x = x + bombWidth() / 2 - bombHeight() / 2;
		y = y + bombHeight() / 2 - bombWidth() / 2;
		MakeRect(&theApp.bombs[theApp.screen_bombs].rect, x, y, bombHeight(), bombWidth());
	}
	theApp.screen_bombs++;
}

/************************************************************************/
/*函数原型：void fire_a_Explosion()										*/
/*传入参数：x坐标，y坐标												*/
/*返 回 值：无															*/
/*函数功能：以(x,y)为中心激活一个爆炸效果								*/
/************************************************************************/
void fire_a_Explosion(int x, int y) {

	int i;
	//激活爆炸效果
	for (i = 0; i < 10; i++)
	{
		if (!theApp.expls[i].valid)
		{
			Mix_PlayChannel(-1, theApp.wavBomb, 0);
			theApp.expls[i].valid = TRUE;
			theApp.expls[i].frame = 0;
			MakeRect(&theApp.expls[i].rect,
				x - explsWidth() / 2,
				y - explsHeight() / 2,
				explsWidth(),
				explsHeight());
			break;
		}
	}

}

/************************************************************************/
/*函数原型：void updateBombs()											*/
/*传入参数：无															*/
/*返 回 值：无															*/
/*函数功能：更新所有炮弹的信息											*/
/************************************************************************/
void updateBombs() {

	int i, aim;
	for (i = 0; i < theApp.screen_bombs; i++)
	{
		aim = bombAim(i);
		moveBomb(i);
		if (aim >= 0 && IsConflict(theApp.bombs[i].rect, theApp.word[aim].rect))//与目标单词发生碰撞
		{
			// 在数组中删除打中的单词 
			delete_one_word(aim);
			//激活爆炸效果
			fire_a_Explosion(bombX(i), bombY(i));
			// 在数组中删除这颗炮弹
			memcpy(&theApp.bombs[i], &theApp.bombs[i + 1], sizeof(Bomb)*(theApp.screen_bombs - 1 - i));
			theApp.screen_bombs--;
		}
		else if (bombY(i) < -bombHeight() || bombY(i) > theApp.SCREEN_HEIGHT || bombX(i) < -bombHeight() || bombX(i) > theApp.SCREEN_WIDTH)//炮弹飞出屏幕
		{
			// 在数组中删除这颗炮弹 
			memcpy(&theApp.bombs[i], &theApp.bombs[i + 1], sizeof(Bomb)*(theApp.screen_bombs - 1 - i));
			theApp.screen_bombs--;
		}
	}
}

/************************************************************************/
/*函数原型：void clearBombs()											*/
/*传入参数：无															*/
/*返 回 值：无															*/
/*函数功能：清空炮弹数组												*/
/************************************************************************/
void clearBombs() {

	memset(theApp.bombs, 0, sizeof(Bomb) * 10);
}


