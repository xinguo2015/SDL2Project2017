/******************************************************************
File name: Word.c
Date:	2015/7/19
Description:	实现加载字典,随机生成单词以及其他单词工具类函数
*******************************************************************/
#include "hitword.h"

extern char *FullPath(char filename[]);

/************************************************************************/
/*函数原型：int read_dic(IDX **ppdic)									*/
/*传入参数：指向字典指针的指针											*/
/*返 回 值：字典中的单词个数						   					*/
/*函数功能：从字典文件中读取数据到内存									*/
/************************************************************************/
int read_dic(IDX **ppdic)
{
	FILE *fp;
	IDX *pdic;
	int len, entries;
	int i, begin;
	fp = fopen(FullPath("/media/dict.idx"), "rb");
	if (fp == NULL)
		return 0;
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	entries = len / sizeof(IDX);
	pdic = (IDX*)malloc(entries * sizeof(IDX));
	if (pdic == NULL)
		return 0;
	fread(pdic, sizeof(IDX), entries, fp);
	fclose(fp);
	*ppdic = pdic;
	begin = 0;
	for (i = 0; i < 26; i++)
	{
		while (begin < entries)
		{
			if ((pdic[begin].entry[0] | 0x20) == 'a' + i)
				break;
			begin++;
		}
		theApp.index[i][0] = begin;
		if (i > 0)
			theApp.index[i - 1][1] = begin - theApp.index[i - 1][0];
	}
	theApp.index[i - 1][1] = entries - begin;
	return entries;
}

/************************************************************************/
/*函数原型：BOOL all_alpha(char *p)										*/
/*传入参数：字符串														*/
/*返 回 值：字符串完全由小写字母构成返回TRUE,否则返回FALSE				*/
/*函数功能：判断字符串是否由小写字母构成								*/
/************************************************************************/
BOOL all_alpha(char *p)
{
	int i, n = strlen(p);
	for (i = 0; i < n; i++)
	{
		if (!islower(p[i]))
			break;
	}
	return i == n;
}

/************************************************************************/
/*函数原型：int letterW(char ch)										*/
/*传入参数：无															*/
/*返 回 值：单个小写英文字母纹理宽度			   						*/
/*函数功能：获取单个小写英文字母纹理宽度								*/
/************************************************************************/
int letterW(char ch) {
	int i = ch - 'a';
	return theApp.gletters[i].mWidth;
}

/************************************************************************/
/*函数原型：int letterH()												*/
/*传入参数：无															*/
/*返 回 值：单个小写英文字母纹理高度			   						*/
/*函数功能：获取单个小写英文字母纹理高度								*/
/************************************************************************/
int letterH() {

	return theApp.gletters[1].mHeight;
}

/************************************************************************/
/*函数原型：int wordX(int index)										*/
/*传入参数：WORD数组下标												*/
/*返 回 值：word[i]包围盒x坐标					   						*/
/*函数功能：获取word[i]包围盒x坐标										*/
/************************************************************************/
int wordX(int index) {

	return theApp.word[index].rect.x;
}

/************************************************************************/
/*函数原型：int wordX(int index,int x)									*/
/*传入参数：WORD数组下标，x坐标											*/
/*返 回 值：无					   										*/
/*函数功能：设定word[index]包围盒x坐标									*/
/************************************************************************/
void setWordX(int index, int x) {

	theApp.word[index].rect.x = x;
}

/************************************************************************/
/*函数原型：int wordX(int index,int y)									*/
/*传入参数：WORD数组下标，y坐标											*/
/*返 回 值：无					   										*/
/*函数功能：设定word[index]包围盒y坐标									*/
/************************************************************************/
void setWordY(int index, int y) {

	theApp.word[index].rect.y = y;
}

/************************************************************************/
/*函数原型：int wordY(int index)										*/
/*传入参数：WORD数组下标												*/
/*返 回 值：word[i]包围盒y坐标					   						*/
/*函数功能：获取word[i]包围盒y坐标										*/
/************************************************************************/
int wordY(int index) {

	return theApp.word[index].rect.y;
}

/************************************************************************/
/*函数原型：int wordWidth(int index)									*/
/*传入参数：WORD数组下标												*/
/*返 回 值：word[i]包围盒宽度					   						*/
/*函数功能：获取word[i]包围盒宽度										*/
/************************************************************************/
int wordWidth(int index) {

	return theApp.word[index].rect.w;
}

/************************************************************************/
/*函数原型：int wordHeight(int index)									*/
/*传入参数：WORD数组下标												*/
/*返 回 值：word[i]包围盒高度					   						*/
/*函数功能：获取word[i]包围盒高度										*/
/************************************************************************/
int wordHeight(int index) {

	return theApp.word[index].rect.h;
}

/************************************************************************/
/*函数原型：void generate_one_word()									*/
/*传入参数：无															*/
/*返 回 值：无															*/
/*函数功能：随机生成一个单词											*/
/***************************** *******************************************/
void generate_one_word()
{
	int i, j, d, k;
	char ch;
	int mark[26] = { 0 };
	srand((int)time(NULL));
	for (j = 0; j < theApp.screen_words; j++)
	{
		mark[(theApp.word[j].word[0] | 0x20) - 'a'] = 1;
	}
	d = (rand() % 100) & 1;
	if (d == 0)
	{
		for (j = 0; j < 26; j++)
			if (mark[j] == 0)
				break;
	}
	else
	{
		for (j = 25; j >= 0; j--)
			if (mark[j] == 0)
				break;
	}
	do
	{
		i = rand() % (theApp.index[j][1]) + theApp.index[j][0];
	} while (!all_alpha(theApp.pdic[i].entry));

	memset(theApp.word[theApp.screen_words].word, 0, sizeof(WORD));
	for (k = 0; k < 20; k++)
	{
		ch = theApp.pdic[i].entry[k];
		theApp.word[theApp.screen_words].word[k] = ch;
		if (ch == '\0')
			break;
		theApp.word[theApp.screen_words].rect.w += letterW(ch);
	}
	theApp.word[theApp.screen_words].rect.h = letterH();
	setWordX(theApp.screen_words, rand() % (theApp.SCREEN_WIDTH - wordWidth(theApp.screen_words)));
	setWordY(theApp.screen_words, -planetHeight());

	theApp.screen_words++;
}

/************************************************************************/
/*函数原型：void delete_one_word(int index)								*/
/*传入参数：WORD数组下标												*/
/*返 回 值：无															*/
/*函数功能：在数组中删除指定下标的单词									*/
/************************************************************************/
void delete_one_word(int wordIndex)
{
	int i;
	if (wordIndex >= theApp.screen_words)
		return;

	//在数组中删除指定单词
	memcpy(&theApp.word[wordIndex], &theApp.word[wordIndex + 1], sizeof(WORD)*(theApp.screen_words - 1 - wordIndex));
	theApp.screen_words--;

	//修改每个炮弹目标单词下标
	for (i = 0; i < theApp.screen_bombs; i++)
	{
		if (bombAim(i) > wordIndex)
			theApp.bombs[i].aim--;
		else if (bombAim(i) == wordIndex)
			theApp.bombs[i].aim = -1;
	}

	//修改正在拼写单词的标记
	if (theApp.hit == wordIndex)
	{
		//如果删除的是正在拼写中的单词
		theApp.hitlen = 0;
		theApp.hit = -1;
	}
	if (theApp.hit > wordIndex)
	{
		theApp.hit--;
	}
}

/************************************************************************/
/*函数原型：void draw_one_word(int index)								*/
/*传入参数：WORD数组下标												*/
/*返 回 值：无															*/
/*函数功能：绘制指定下标的单词											*/
/************************************************************************/
void draw_one_word(int wordIndex) {
	int i, j, x, y;
	char* word;
	if (wordIndex >= theApp.screen_words || wordIndex < 0)
		return;
	//绘制单词背景（小星球）
	x = wordX(wordIndex) + wordWidth(wordIndex) / 2 - planetWidth() / 2;
	y = wordY(wordIndex) + wordHeight(wordIndex) / 2 - planetHeight() / 2;
	renderTexture(theApp.gplanet, x, y);
	//书写每个单词
	x = wordX(wordIndex);
	y = wordY(wordIndex);
	word = theApp.word[wordIndex].word;
	for (i = 0; i < strlen(word); i++)
	{
		j = word[i] - 'a';
		//未拼出的字母，使用背景色
		if (theApp.hit != wordIndex || i >= theApp.hitlen)
			renderTexture(theApp.gletters[j], x, y);
		else//已拼出的字母，使用前景色
			renderTexture(theApp.gletters[j + 26], x, y);
		x += letterW(word[i]);
	}
}

/************************************************************************/
/*函数原型：void updateWords()											*/
/*传入参数：无															*/
/*返 回 值：无															*/
/*函数功能：更新所有单词的信息											*/
/************************************************************************/
void updateWords() {

	int i;
	//单词已到达屏幕底行，删除该单词
	if (wordY(0) + wordHeight(0) / 2 - planetHeight() / 2 >= theApp.SCREEN_HEIGHT)
		delete_one_word(0);

	for (i = 0; i < theApp.screen_words; i++)
	{
		theApp.word[i].rect.y++;
		//单词撞上UFO
		if (IsConflict(theApp.ufo.rect, theApp.word[i].rect))
		{
			//产生爆炸
			fire_a_Explosion(wordX(i) + wordWidth(i) / 2, wordY(i) + wordHeight(i));
			//删除单词
			delete_one_word(i);
			//减生命值
			theApp.blood.hp -= 10;
			if (!UFOHP())
			{
				theApp.gameover = TRUE;
			}
		}
	}


}

/************************************************************************/
/*函数原型：void clearWords()											*/
/*传入参数：无															*/
/*返 回 值：无															*/
/*函数功能：清空单词数组												*/
/************************************************************************/
void clearWords() {

	memset(theApp.word, 0, sizeof(WORD) * 25);
}
