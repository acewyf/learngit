#ifndef _TETRIS_H_
#define _TETRIS_H_

#define FIRST_X 10	//游戏界面左上角的坐标是（10,5）
#define FIRST_Y	5

#define BOX_WIDTH	20	//游戏空间20*20
#define BOX_HEIGHT	20

#define MENU_WIDTH	12	//右侧的菜单显示空间
#define MENU_HEIGHT 20
#define MENU_MID	8

static int mode;

static const int shape[1][4][16] = {
									{{0,0,0,0, 0,1,0,0, 1,1,1,0, 0,0,0,0},
									 {0,0,0,0, 0,1,0,0, 0,1,1,0, 0,1,0,0},
									 {0,0,0,0, 0,0,0,0, 1,1,1,0, 0,1,0,0},
									 {0,0,0,0, 0,1,0,0, 1,1,0,0, 0,1,0,0}}
								   };
								   
static int game_area[20][10] = {0};	//用来记录游戏区域内的值，为1打印方块；
static int tetris[4][2];	//用来记录当前俄罗斯方块的坐标
								   
#endif
