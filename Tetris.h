#ifndef _TETRIS_H_
#define _TETRIS_H_

#define FIRST_X 10	//��Ϸ�������Ͻǵ������ǣ�10,5��
#define FIRST_Y	5

#define BOX_WIDTH	20	//��Ϸ�ռ�20*20
#define BOX_HEIGHT	20

#define MENU_WIDTH	12	//�Ҳ�Ĳ˵���ʾ�ռ�
#define MENU_HEIGHT 20
#define MENU_MID	8

static int mode;

static const int shape[1][4][16] = {
									{{0,0,0,0, 0,1,0,0, 1,1,1,0, 0,0,0,0},
									 {0,0,0,0, 0,1,0,0, 0,1,1,0, 0,1,0,0},
									 {0,0,0,0, 0,0,0,0, 1,1,1,0, 0,1,0,0},
									 {0,0,0,0, 0,1,0,0, 1,1,0,0, 0,1,0,0}}
								   };
								   
static int game_area[20][10] = {0};	//������¼��Ϸ�����ڵ�ֵ��Ϊ1��ӡ���飻
static int tetris[4][2];	//������¼��ǰ����˹���������
								   
#endif
