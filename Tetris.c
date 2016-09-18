#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Tetris.h"

/**
*�������ƣ�Init_Game_Show
*�������ܣ���ʼ����Ϸ���棬��ӡ����Ϸ�߿�
*/

void Init_Game_Show()
{
	int i;
	printf("\033[2J");
	printf("\033[%d;%dH",FIRST_Y,FIRST_X);	//������ƶ���ָ������
	for (i = 0; i < (2+BOX_WIDTH+2+MENU_WIDTH+2); i++)	//��ӡ�ϱ߽�
		printf("=");
	printf("\033[%d;%dH\033[1B",FIRST_Y,FIRST_X);	//������ƶ�����ʼ��������һ��
	for (i = 0; i < BOX_HEIGHT; i++)
		printf("||\033[1B\033[2D");
	printf("\033[%d;%dH\033[1B",FIRST_Y,FIRST_X+1+BOX_WIDTH+1);
	for (i = 0; i < BOX_HEIGHT; i++)
		printf("||\033[1B\033[2D");
	printf("\033[%d;%dH\033[1B",FIRST_Y,FIRST_X+1+BOX_WIDTH+2+MENU_WIDTH+1);
	for (i = 0; i < BOX_HEIGHT; i++)
		printf("||\033[1B\033[2D");
	printf("\033[%d;%dH",FIRST_Y+BOX_HEIGHT+1,FIRST_X);
	for (i = 0; i < (2+BOX_WIDTH+2+MENU_WIDTH+2); i++)	//��ӡ�±߽�
		printf("=");
	printf("\033[?25l");
	fflush(stdout);
}


/**
*�������ƣ�Show_Block
*�������ܣ���ӡ����������Ķ���˹����;
*�����߼���1���������еĶ���˹�����Ӧ����Ϸ����ֵ��Ϊ1;
*		   2������Ϸ������ֵΪ1��λ�ô�ӡ"[]";
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
*�������ƣ�Block_Value
*�������ܣ�������˹������ֵĳ�ʼλ��ֵ��Ϊ1;
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