#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Tetris.h"

/**
*函数名称：Init_Game_Show
*函数功能：初始化游戏界面，打印出游戏边框；
*/

void Init_Game_Show()
{
	int i;
	printf("\033[2J");
	printf("\033[%d;%dH",FIRST_Y,FIRST_X);	//将光标移动到指定坐标
	for (i = 0; i < (2+BOX_WIDTH+2+MENU_WIDTH+2); i++)	//打印上边界
		printf("=");
	printf("\033[%d;%dH\033[1B",FIRST_Y,FIRST_X);	//将光标移动到初始坐标下移一个
	for (i = 0; i < BOX_HEIGHT; i++)
		printf("||\033[1B\033[2D");
	printf("\033[%d;%dH\033[1B",FIRST_Y,FIRST_X+1+BOX_WIDTH+1);
	for (i = 0; i < BOX_HEIGHT; i++)
		printf("||\033[1B\033[2D");
	printf("\033[%d;%dH\033[1B",FIRST_Y,FIRST_X+1+BOX_WIDTH+2+MENU_WIDTH+1);
	for (i = 0; i < BOX_HEIGHT; i++)
		printf("||\033[1B\033[2D");
	printf("\033[%d;%dH",FIRST_Y+BOX_HEIGHT+1,FIRST_X);
	for (i = 0; i < (2+BOX_WIDTH+2+MENU_WIDTH+2); i++)	//打印下边界
		printf("=");
	printf("\033[?25l");
	fflush(stdout);
}


/**
*函数名称：Show_Block
*函数功能：打印出正在下落的俄罗斯方块;
*函数逻辑：1、将下落中的俄罗斯方块对应的游戏区域值置为1;
*		   2、在游戏区域中值为1的位置打印"[]";
*/

void Show_Block()
{
	int i, j;
	printf("\033[%d;%dH",FIRST_Y+1,FIRST_X+2);
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (game_area[i][j] == 1)
				printf("[]");
			else
				printf("  ");
		}
		printf("\033[20D\033[1B");
	}
	fflush(stdout);
}				

/**
*函数名称：Block_Value
*函数功能：将俄罗斯方块出现的初始位置值设为1;
*/

void Block_Value()
{
	srand((unsigned) time(NULL));
	mode = rand()%4;
	int i;
	for (i = 0; i < 16; i++)
	{
		//if (shape[0][mode][i] == 1)
			game_area[i/4][i%4+3] = shape[0][mode][i];
	}
	//if (game_area[0][0]
	Show_Block();
}

void Move_Down()
{
	int i, j, m = 0;
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (game_area[i][j] == 1)
			{
				game_area[i][j] = 0;
				tetris[m][0] = i;
				tetris[m][1] = j;
				m++;
			}
		}
	}
	for (m = 0; m < 4; m++)
	{		
		i = tetris[m][0]+1;
		j = tetris[m][1];
		game_area[i][j] = 1;
	}
	Show_Block();
}

Move_Left()
{
	int i, j, m = 0;
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (game_area[i][j] == 1)
			{
				game_area[i][j] = 0;
				tetris[m][0] = i;
				tetris[m][1] = j;
				m++;
			}
		}
	}
	for (m = 0; m < 4; m++)
	{		
		i = tetris[m][0];
		j = tetris[m][1]-1;
		game_area[i][j] = 1;
	}
	Show_Block();
}

Move_Right()
{
	int i, j, m = 0;
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 10; j++)
		{
			if (game_area[i][j] == 1)
			{
				game_area[i][j] = 0;
				tetris[m][0] = i;
				tetris[m][1] = j;
				m++;
			}
		}
	}
	for (m = 0; m < 4; m++)
	{		
		i = tetris[m][0];
		j = tetris[m][1]+1;
		game_area[i][j] = 1;
	}
	Show_Block();
}

void Change_Shape()
{
	mode = (mode + 1)%4;
}

int main()
{
	char c;
	Init_Game_Show();
	Block_Value();
	while (1)
	{
		
		sleep(1);
		Move_Down();
		//printf("i love you so much!\n");
		//if ((c = getchar()) == 'q')
		//	break;	
	}
	return 0;
}