#include <stdlib.h>
#include <gtk/gtk.h>

gint ROW =10;/*雷区行数*/
gint COL =10;/*雷区列数*/
gint MINE_TOTAL =10;/*雷区总共埋雷数*/

typedef struct mine_block	/*用于雷区的每个雷块的结构体*/
{
	gint mine_around;/*周围有多少雷*/
	gboolean has_mine;/*是否埋有雷*/
	gboolean marked;/*是否用右键标记*/
	gboolean opened;/*是否用左键挖开*/
	GtkWidget *button;/*按钮*/
}mine_block,*map;

typedef struct MineAreaSet/*用于设置窗口的结构体,方便信号处理时的多参数传递*/
{
	GtkWidget *set_window;
	GtkWidget *radio[4];/*4个单选按钮(包含一个自定义单选按钮)*/
	GtkWidget *entry_mine;/*用于自定义输入时的3个输入文本框*/
	GtkWidget *entry_row;
	GtkWidget *entry_col;
	GSList *group;/*用于单选按钮的链表指针*/
}Mine_Area_Set;

gboolean gaming=FALSE;/*用于表示游戏的进行,方便控制时间函数的运行*/
gboolean game_over=FALSE;/*用于表示游戏的结束*/
gint mine_block_opened=0;/*用于表示总共被掀开的雷块数目,方便于表示游戏的成功*/
gint mine_not_marked=99;/*用于表示右键未使用的标记次数,默认99*/
gint game_time=0;/*用于表示游戏运行的时间*/
mine_block **mine_area;/*靠malloc等来动态申请整个雷区储存空间*/
GtkWidget *window;/*游戏的主窗口*/
GtkWidget *table;/*用于容纳雷区的每个按钮的表*/
GtkWidget *entry_time;/*用于显示时间的文本框*/
GtkWidget *entry_mine;/*用于显示为标记雷数的文本框*/
GtkWidget *vbox;/*用于在主窗口容纳菜单栏,表,水平盒子(容纳显示时间的文本框,标签和显示为标记雷数的文本框等)的纵向盒子*/

gboolean mouse_click(GtkWidget *widget ,GdkEventButton *event,gpointer data);/*对雷区中鼠标单击事件的信号处理*/

void new_game_Handler(GtkMenuItem *menu_iterms ,gpointer data);/*菜单new_game按钮的信号处理*/

void about_Handler(GtkMenuItem *menu_iterms ,gpointer data);/*菜单about对话框的信号处理*/

void set_Handler(GtkMenuItem *menu_iterms ,gpointer data);/*菜单setting按钮的信号处理*/

void set_window_cancel(GtkButton *button ,gpointer data);/*设置窗口的取消按钮的信号处理*/

void set_window_apply(GtkButton *button ,gpointer data);/*设置窗口的确认按钮的信号处理*/

void do_nothing(GtkWidget set_window ,gpointer data);/*为防止点击设置窗口右上角小叉而恶意退出的函数*/

void build_interface();/*用于创建游戏界面的函数*/

void game_init();/*游戏初始化函数*/

void new_game();/*游戏开始函数*/

void open_block(gint row,gint col);/*用于左键打开雷块的递归函数*/

gint mid_click(gint row,gint col);/*用于中键单击的函数，返回是否被标记，是则返回1，否则返回0*/

void mid_click2(gint row,gint col);/*用于中键单击的函数，用于满足相应要求时打开雷块*/

gboolean time_box(gpointer data);/*用于记时*/

void mine_area_set();/*创建设置窗口的界面用于设置雷区大小*/

void Game_over(gboolean result);/*游戏结束函数*/

gint main( gint argc, gchar *argv[] )
{
	gtk_init(&argc, &argv);
	build_interface();/* 创建一个游戏主界面*/
	game_init();	/*游戏初始化,开始游戏*/
	gtk_widget_show_all(window);
	g_timeout_add(1000,(GSourceFunc)time_box,NULL);/*计时器启动*/
	gtk_main();
	return 0;
}

void build_interface()
{
	
	gchar buffer[4];/*存储mine_not_marked文本信息以便在entry中显示*/
	
	GtkWidget *menu_bar;
	
	GtkWidget *game_menu;/*菜单栏上的game按钮*/
	
	GtkWidget *help_menu;/*菜单栏上的help按钮*/
	
	GtkWidget *menu_items;/*各个菜单里的按钮*/
	
	GtkWidget *menu;/*容纳各个菜单里的按钮的菜单列表*/
	
	GtkWidget *hbox;/*容纳容纳显示时间的文本框,标签和显示为标记雷数的文本框等*/
	
	GtkWidget *label;/*用于显示Time：和Not Mark的标签*/
	
/* 创建一个新窗口 */
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	
	gtk_window_set_title (GTK_WINDOW (window), "GTK Mine");/*设置窗口标题*/
	
	gtk_widget_set_usize(window,COL*25,ROW*25+55);
	
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);/*最大化按钮失效，用户不能调整窗口大小*/
	
	g_signal_connect (G_OBJECT (window), "delete_event",G_CALLBACK (gtk_main_quit), NULL);/*窗口退出的信号处理*/
	
/* 创建一个纵向盒子,并放到主窗口： */
	
	vbox = gtk_vbox_new (FALSE, 0);
	
	gtk_container_add (GTK_CONTAINER (window), vbox);
	
/* 创建一个水平向盒子,并放到纵向盒子里： */
	
	hbox=gtk_hbox_new(TRUE,2);/*平均分布*/
	
	gtk_box_pack_end(GTK_BOX(vbox),hbox,FALSE,FALSE,0);/*添加到纵向盒子底端,该水平盒子用于添放label和entry*/
	
	label=gtk_label_new("Time:");
	
	entry_time=gtk_entry_new_with_max_length(2);
	
	gtk_entry_set_text(GTK_ENTRY(entry_time),"0");
	
	gtk_box_pack_start(GTK_BOX(hbox),label,FALSE,TRUE,6);
	
	gtk_box_pack_start(GTK_BOX(hbox),entry_time,FALSE,TRUE,6);
	
	label=gtk_label_new("Not Mark");
	
	entry_mine=gtk_entry_new_with_max_length(2);
	
	g_snprintf(buffer,4,"%d",mine_not_marked);
	
	gtk_entry_set_text(GTK_ENTRY(entry_mine),buffer);
	
	gtk_box_pack_end(GTK_BOX(hbox),entry_mine,FALSE,TRUE,6);
	
	gtk_box_pack_end(GTK_BOX(hbox),label,TRUE,TRUE,6);
	
/*建立菜单的语段*/
	
	menu_bar=gtk_menu_bar_new();
	
	gtk_box_pack_start (GTK_BOX (vbox), menu_bar, FALSE, FALSE, 0);
	
	game_menu = gtk_menu_item_new_with_label("Game");
	
	help_menu =gtk_menu_item_new_with_label("Help");
	
/*将游戏菜单键和帮助菜单键添加到菜单栏*/
	
	gtk_menu_bar_append(GTK_MENU_BAR(menu_bar),game_menu);
	
	gtk_menu_bar_append(GTK_MENU_BAR(menu_bar),help_menu);
	
	menu=gtk_menu_new();/*用于生成点击game_menu菜单按钮展开的菜单*/
	
	menu_items=gtk_menu_item_new_with_label("New Game");
	
	gtk_menu_append(GTK_MENU(menu),menu_items);
	
	g_signal_connect(GTK_OBJECT(menu_items),"activate",G_CALLBACK(new_game_Handler),NULL);/*重新开始信号*/
	
	menu_items=gtk_menu_item_new();
	
	gtk_container_add(GTK_CONTAINER(menu_items),gtk_hseparator_new());
	
	gtk_menu_append(GTK_MENU(menu),menu_items);/*添加水平隔离线*/
	
	menu_items=gtk_menu_item_new_with_label("Setting...");
	
	gtk_menu_append(GTK_MENU(menu),menu_items);
	
	g_signal_connect(G_OBJECT(menu_items),"activate",G_CALLBACK(set_Handler),NULL);/*雷区大小设置信号*/
	
	menu_items=gtk_menu_item_new();
	
	gtk_container_add(GTK_CONTAINER(menu_items),gtk_hseparator_new());
	
	gtk_menu_append(GTK_MENU(menu),menu_items);/*添加水平隔离线*/
	
	menu_items=gtk_menu_item_new_with_label("Exit");
	
	gtk_menu_append(GTK_MENU(menu),menu_items);
	
	g_signal_connect(GTK_OBJECT(menu_items),"activate",G_CALLBACK(gtk_main_quit),NULL);/*游戏退出信号*/
	
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(game_menu),menu); /*将菜单放入game_menu菜单按钮下*/
	
	menu=gtk_menu_new();/*用于生成点击game_menu菜单按钮展开的菜单*/
	
	menu_items=gtk_menu_item_new_with_label("How to...");
	
	gtk_menu_append(GTK_MENU(menu),menu_items);
	
	menu_items=gtk_menu_item_new();
	
	gtk_container_add(GTK_CONTAINER(menu_items),gtk_hseparator_new());
	
	gtk_menu_append(GTK_MENU(menu),menu_items);/*添加水平隔离线*/
	
	menu_items=gtk_menu_item_new_with_label("About");
	
	gtk_menu_append(GTK_MENU(menu),menu_items);
	
	g_signal_connect(GTK_OBJECT(menu_items),"activate",G_CALLBACK(about_Handler),NULL);/*游戏about信号*/
	
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_menu),menu);/*将菜单放入help_menu菜单按钮下*/
	
	return ;
	
}

void game_init()

{	static gint temp_row=0;

gint row,col;

static gboolean first_time=TRUE;

gint n=-1;/*用于计算第几个mine_block 应该从0开始算，到ROE*COL-1*/

if(first_time==FALSE)

{
	
	gtk_widget_destroy(table);/*原来表格抹去，重新建立表格*/
	
	table=gtk_table_new(ROW,COL,TRUE);
	
/*回收原来二维数组空间*/
	
	for(row=0;row<temp_row;row++)
	
	{
		
		free(mine_area[row]);
		
		mine_area[row]=NULL;
		
	}
	
	free(mine_area);
	
	mine_area=NULL;
	
	gtk_box_pack_start(GTK_BOX(vbox),table,TRUE,TRUE,2);/*将容纳雷区的表格放入纵向盒子*/
	
}

else

{
	
	table=gtk_table_new(ROW,COL,TRUE);
	
	gtk_box_pack_start(GTK_BOX(vbox),table,TRUE,TRUE,2);/*将容纳雷区的表格放入 纵向盒子*/
	
}

/*显示表格，申请二维数组*/

gtk_widget_show(table);

mine_area=(map *)malloc(ROW*sizeof(map));

for(row=0;row<ROW;row++){mine_area[row]=(mine_block *)malloc(COL*sizeof(mine_block) );}

for(row=0;row<ROW;row++)

{
	
	for(col=0;col<COL;col++)
	
	{
		
		n++;
		
		mine_area[row][col].button=gtk_toggle_button_new();
		
		g_signal_connect(GTK_OBJECT(mine_area[row][col].button),"button-press-event",G_CALLBACK(mouse_click),(gpointer)(n));/*鼠标单击事件的信号*/
		
		gtk_table_attach_defaults(GTK_TABLE(table),mine_area[row][col].button,col,col+1,row,row+1);
		
		g_object_set(GTK_OBJECT(mine_area[row][col].button),"can-focus", FALSE, NULL);/*失去焦点*/
		
		gtk_widget_set_usize(mine_area[row][col].button,25,25);
		
		gtk_widget_show(mine_area[row][col].button);
		
	}
	
}

first_time=FALSE;

temp_row=ROW;

new_game();/*调用游戏开始函数*/

}

void new_game()/*重新开始游戏，重新布雷*/

{
	
	gint i=0;/*累计随机布雷数*/
	
	gint row,col;
	
	gint random;
	
	gchar buffer[4];/*存储mine_not_marked文本信息以便在entry中显示*/
	
	game_over=FALSE;
	
	game_time=0;
	
	mine_not_marked=MINE_TOTAL;
	
	mine_block_opened=0;
	
	gaming=FALSE;
	
	g_snprintf(buffer,4,"%d",mine_not_marked);
	
	gtk_entry_set_text(GTK_ENTRY(entry_time),"0");
	
	gtk_entry_set_text(GTK_ENTRY(entry_mine),buffer);
	
	for(row=0;row<ROW;row++)
	
	{
		
		for(col=0;col<COL;col++)
		
		{
			
			mine_area[row][col].mine_around=0;
			
			mine_area[row][col].has_mine=FALSE;
			
			mine_area[row][col].opened=FALSE;
			
			mine_area[row][col].marked=FALSE;
			
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mine_area[row][col].button),FALSE);
			
			gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"");/*默认雷块按钮显示空白*/
			
		}
		
	}
	
	while(i<MINE_TOTAL)/*以下为布雷语段*/
	
	{
		
		random =g_random_int_range(0,ROW*COL);
		
		row=random/COL;
		
		col=random%COL;
		
		if(mine_area[row][col].has_mine==TRUE)/*如果已经埋过雷了*/
		
		{continue;}
		
		mine_area[row][col].has_mine=TRUE;
		
		i++;
		
		if(row>0)
		
		{
			
			mine_area[row-1][col].mine_around++;/*上++*/
			
			if(col>0){mine_area[row-1][col-1].mine_around++;}/*左上++*/
			
			if(col<COL-1){mine_area[row-1][col+1].mine_around++;}/*右上++*/
			
		}
		
		if(row<ROW-1)
		
		{
			
			mine_area[row+1][col].mine_around++;/*下++*/
			
			if(col>0){mine_area[row+1][col-1].mine_around++;}/*左下++*/
			
			if(col<COL-1){mine_area[row+1][col+1].mine_around++;}/*右下++*/
			
		}
		
		if(col>0){mine_area[row][col-1].mine_around++;}/*左++*/
		
		if(col<COL-1){mine_area[row][col+1].mine_around++;}/*右++*/
		
	}
	
}

void open_block(gint row,gint col)

{
	
	static gchar buffer[4];
	
	if(game_over==TRUE || mine_area[row][col].marked==TRUE || mine_area[row][col].opened==TRUE)
	
	{return;}/*如果游戏结束,或者被右键标记过,或者已经被打开过,则不能再进行打开操作*/
	
	mine_area[row][col].opened=TRUE;/*不是以上情况的,标记为打开*/
	
	gaming=TRUE;/*游戏开始*/
	
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mine_area[row][col].button),TRUE);/*设置按钮呈按下状*/
	
	if(mine_area[row][col].has_mine==TRUE)/*如果刚好下面埋有雷，游戏结束*/
	
	{
		
		gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"*");
		
		Game_over(FALSE);
		
		return;
		
	}
	
	if(mine_area[row][col].mine_around!=0)/*如果当下无雷，但周围
	
	有雷,则显示周围雷数*/
	
	{
		
		g_snprintf(buffer,4,"%d",mine_area[row][col].mine_around);
		
		gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),buffer);
		
	}
	
	mine_block_opened++;/*雷块又被掀开一个*/
	
	if(mine_block_opened+MINE_TOTAL==ROW*COL)/*游戏胜利*/
	
	{
		
		Game_over(TRUE);return;
		
	}
	
	if(mine_area[row][col].mine_around==0)/*当下无雷，但周围亦无雷,则自动打开周围雷块*/
	
	{
		
		if(row>0)
		
		{
			
			open_block(row-1,col);/*打开上*/
			
			if(col>0){open_block(row-1,col-1);}/*打开左上*/
			
			if(col<COL-1){open_block(row-1,col+1);}/*打开右上*/
			
		}
		
		if(row<ROW-1)
		
		{
			
			open_block(row+1,col);/*打开下*/
			
			if(col>0){open_block(row+1,col-1);}/*打开左下*/
			
			if(col<COL-1){open_block(row+1,col+1);}/*打开右下*/
			
		}
		
		if(col>0){open_block(row,col-1);}/*打开左*/
		
		if(col<COL-1){open_block(row,col+1);}/*打开右*/
		
	}
	
	return;
	
}

gboolean time_box(gpointer data)

{
	
	gchar buffer[4];
	
	if (game_over==FALSE && gaming==TRUE)/*游戏没结束才工作,如果游戏开始进行，则时间增加*/
	
	{
		
		game_time++;
		
		g_snprintf(buffer,4,"%d",game_time);
		
		gtk_entry_set_text(GTK_ENTRY(entry_time),buffer);
		
	}
	
	return TRUE;/*返回TRUE继续计时*/
	
}

void mine_area_set()

{
	
/*typedef struct MineAreaSet
	
	{GtkWidget *set_window;
	
	GtkWidget *radio[4];
	
	GtkWidget *entry_mine;
	
	GtkWidget *entry_row;
	
	GtkWidget *entry_col;
	
	GSList *group;
	
}Mine_Area_Set;*/

Mine_Area_Set *p;

p=(Mine_Area_Set *)malloc(sizeof(Mine_Area_Set));

p->group=NULL;

GtkWidget *label;

GtkWidget *button;

GtkWidget *table;

/*建立设置窗口*/

p->set_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_usize(p->set_window, 420, 300);

gtk_widget_set_uposition(p->set_window,420, 300);

gtk_window_set_title (GTK_WINDOW (p->set_window), "Mine_Area_Set");

gtk_window_set_resizable(GTK_WINDOW(p->set_window),FALSE);/*最大化按钮失效，用户不能调整窗口大小*/

g_signal_connect (G_OBJECT (p->set_window), "delete_event",G_CALLBACK (do_nothing), NULL);

gtk_window_set_modal(GTK_WINDOW(p->set_window),TRUE);/*鼠标不能回到主界面，必须给出信号*/

table=gtk_table_new(5,5,TRUE);

gtk_container_add(GTK_CONTAINER(p->set_window),table);

/*四个单选按钮*/

p->radio[0]=gtk_radio_button_new_with_label(p->group,"10*10 10 mines");

gtk_table_attach_defaults(GTK_TABLE(table),p->radio[0],0,2,1,2);

p->group=(gtk_radio_button_group(GTK_RADIO_BUTTON(p->radio[0])));

p->radio[1]=gtk_radio_button_new_with_label(p->group,"16*16 40 mines");

gtk_table_attach_defaults(GTK_TABLE(table),p->radio[1],0,2,2,3);

p->group=(gtk_radio_button_group(GTK_RADIO_BUTTON(p->radio[1])));

p->radio[2]=gtk_radio_button_new_with_label(p->group,"16*30 99 mines");

gtk_table_attach_defaults(GTK_TABLE(table),p->radio[2],0,2,3,4);

p->group=(gtk_radio_button_group(GTK_RADIO_BUTTON(p->radio[2])));

p->radio

[3]=gtk_radio_button_new_with_label(p->group,"custom");

gtk_table_attach_defaults(GTK_TABLE(table),p->radio[3],3,4,0,1);

p->group=(gtk_radio_button_group(GTK_RADIO_BUTTON(p->radio[3])));

label=gtk_label_new("set mine_area");

gtk_table_attach_defaults(GTK_TABLE(table),label,0,2,0,1);

label=gtk_label_new("MINE_TOTAL [5,99]");

gtk_table_attach_defaults(GTK_TABLE(table),label,2,4,1,2);

label=gtk_label_new("ROW [10,16]");

gtk_table_attach_defaults(GTK_TABLE(table),label,3,4,2,3);

label=gtk_label_new("COL [10,30]");

gtk_table_attach_defaults(GTK_TABLE(table),label,3,4,3,4);

/*自定义设置的3个文本框*/

p->entry_mine=gtk_entry_new_with_max_length(2);

gtk_entry_set_text(GTK_ENTRY(p->entry_mine),"99");

gtk_table_attach_defaults(GTK_TABLE(table),p->entry_mine,4,5,1,2);

p->entry_row=gtk_entry_new_with_max_length(2);

gtk_entry_set_text(GTK_ENTRY(p->entry_row),"16");

gtk_table_attach_defaults(GTK_TABLE(table),p->entry_row,4,5,2,3);

p->entry_col=gtk_entry_new_with_max_length(2);

gtk_entry_set_text(GTK_ENTRY(p->entry_col),"30");

gtk_table_attach_defaults(GTK_TABLE(table),p->entry_col,4,5,3,4);

button=gtk_button_new_with_label("Apply");/*确认按钮及其信号*/

gtk_table_attach_defaults(GTK_TABLE(table),button,3,4,4,5);

g_signal_connect (G_OBJECT (button), "pressed",G_CALLBACK (set_window_apply), p);

button=gtk_button_new_with_label("Cancel");/*取消按钮及其信号*/

gtk_table_attach_defaults(GTK_TABLE(table),button,4,5,4,5);

g_signal_connect (G_OBJECT (button),"pressed",G_CALLBACK (set_window_cancel), p->set_window);

gtk_widget_show_all(p->set_window);

return ;

}

void Game_over(gboolean result)

{
	
	GtkWidget *dialog;
	
	gint row,col;
	
	gint num=0;/*用于失败时统计成功标记的雷数*/
	
	gchar msg[40];/*存放消息*/
	
	game_over=TRUE;/*游戏结束了*/
	
	for(row=0;row<ROW;row++)/*游戏结束显示雷区所有信息*/
	
	{
		
		for(col=0;col<COL;col++)
		
		{
			
			if(mine_area[row][col].opened==FALSE)/*对于没被打开的雷块*/
			
			{
				
/*触发雷块所在按钮的toggle*/
				
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mine_area[row][col].button),TRUE);
				
				gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"");/*默认label不显示信息*/
				
				if(mine_area[row][col].has_mine==FALSE)
				
				{
					
/*if(mine_area[row][col].mine_around>0)
					
					{
						
						g_snprintf(buffer,4,"%d",mine_area[row][col].mine_around);
						
						gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),buffer);
						
					}*/
					
					if(mine_area[row][col].marked==TRUE)/*无雷但右键标记*/
					
					{gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"W");}
					
				}
				
				else/*有雷*/
				
				{
					
					if(mine_area[row][col].marked==FALSE && result==FALSE)/*有雷但无右键标记且失败*/
					
					{
						
						gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mine_area[row][col].button),FALSE);
						
						gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"*?");
						
					}
					
					else/*有雷又有标记 或者 胜利*/
					
					{gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"R");num++;}
					
				}
				
			}
			
		}
		
	}
	
	if(result==TRUE)/*如果胜利*/
	
	{
		
		g_snprintf(msg,40,"You Are Great!Congratulations!");
		
	}
	
	else/*如果失败*/
	
	{
		
		g_snprintf(msg,40,"You Failed!So Poor!\nMark %d mines",num);
		
	}
	
/*显示对话框*/
	
	dialog=gtk_message_dialog_new(GTK_WINDOW(window),0,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,msg);
	
	gtk_dialog_run(GTK_DIALOG(dialog));
	
	gtk_widget_destroy(dialog);
	
}

gboolean mouse_click(GtkWidget *widget ,GdkEventButton *event,gpointer data)

{
	
	gint k=0;/*k用于记录某个雷块周围有多少标记数*/
	
	gint n;/*令n=(gint)data,则可以知道是第n个雷块上发生的鼠标点击事件*/
	
	gint row,col;
	
	gchar buffer[4];/*存放mine_not_marked 的内容，以便用于在entry中显示*/
	
	n=(gint)data;
	
	row= n/COL;
	
	col=n%COL;
	
	if(game_over==FALSE)/*以下情况都是在游戏没结束情况下进行*/
	
	switch(event->button)
	
	{
		
		case 1:/*左键单击*/
		
		{
			
			open_block(row,col);/*左键打开雷块*/
			
			break;
			
		}
		
		case 2:/*中键单击*/
		
		{
			
			if(mine_area[row][col].marked==FALSE && mine_area[row][col].mine_around>0 &&mine_area[row][col].opened==TRUE)
			
			{
				
				if(row>0)
				
				{
					
					k+=mid_click(row-1,col);/*打开上*/
					
					if(col>0){k+=mid_click(row-1,col-1);}/*打开左上*/
					
					if(col<COL-1){k+=mid_click(row-1,col+1);}/*打开右上*/
					
				}
				
				if(row<ROW-1)
				
				{
					
					k+=mid_click(row+1,col);/*打开下*/
					
					if(col>0){k+=mid_click(row+1,col-1);}/*打开左下*/
					
					if(col<COL-1){k+=mid_click(row+1,col+1);}/*打开右下*/
					
				}
				
				if(col>0){k+=mid_click(row,col-1);}/*打开左*/
				
				if(col<COL-1){k+=mid_click(row,col+1);}/*打开右*/
				
				if(k>=mine_area[row][col].mine_around)/*考虑存在过标的情况*/
				
				{
					
					if(row>0)
					
					{
						
						mid_click2(row-1,col);/*打开上*/
						
						if(col>0){mid_click2(row-1,col-1);}/*打开左上*/
						
						if(col<COL-1){mid_click2(row-1,col+1);}/*打开右上*/
						
					}
					
					if(row<ROW-1)
					
					{
						
						mid_click2(row+1,col);/*打开下*/
						
						if(col>0){mid_click2(row+1,col-1);}/*打开左下*/
						
						if(col<COL-1){mid_click2(row+1,col+1);}/*打开右下*/
						
					}
					
					if(col>0){mid_click2(row,col-1);}/*打开左*/
					
					if(col<COL-1){mid_click2(row,col+1);}/*打开右*/
					
				}
				
			}
			
			break;
			
		}
		
		case 3:/*右键单击*/
		
		{
			
			if(mine_area[row][col].opened==TRUE)/*已经被左键打开过*/
			
			{break;}
			
			if(mine_area[row][col].marked==TRUE)/*已经被右键标记过，再右击则取消标记*/
			
			{
				
				mine_area[row][col].marked=FALSE;
				
				mine_not_marked++;
				
				gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"");
				
			}
			
			else if(mine_not_marked>0)
			
			{ /*否则显示 M 表示被标记了*/
			
			mine_area[row][col].marked=TRUE;
			
			gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"M");
			
			mine_not_marked--;
			
		}
		
	}
	
}

g_snprintf(buffer,4,"%d",mine_not_marked);/*存放mine_not_marked 的内容，以便用于在entry中显示*/

gtk_entry_set_text(GTK_ENTRY(entry_mine),buffer);

return TRUE;

}

void new_game_Handler(GtkMenuItem *menu_iterms ,gpointer data)

{
	
	new_game();
	
}

void about_Handler(GtkMenuItem *menu_iterms ,gpointer data)

{
	
	GtkWidget *dialog;
	
	dialog=gtk_message_dialog_new(GTK_WINDOW(window),0,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,
	
	"I used the GTK development tools to develop the mine game,\nthe game was ported from linux to windows, so\n it may run slower on windows (do not doubt your memory). \nIf you encounter problems in the test(Some buttons are just adornments), please let me know.\n Thank you.");
	
	gtk_dialog_run(GTK_DIALOG(dialog));
	
	gtk_widget_destroy(dialog);
	
}

void set_Handler(GtkMenuItem *menu_iterms ,gpointer data)

{
	
	mine_area_set();
	
}

void set_window_cancel(GtkButton *button ,gpointer data)

{
	
	GtkWidget *set_window;
	
	set_window=(GtkWidget*)data;
	
	gtk_widget_destroy(set_window);
	
}

void set_window_apply(GtkButton *button ,gpointer data)

{
	
/*typedef struct MineAreaSet
	
	{GtkWidget *set_window;
	
	GtkWidget *radio[4];
	
	GtkWidget *entry_mine;
	
	GtkWidget *entry_row;
	
	GtkWidget *entry_col;
	
	GSList *group;
	
}Mine_Area_Set;*/

gint temp_mine;/*设置雷区大小时的临时雷区行数*/

gint temp_row;/*设置雷区大小时的临时雷区列数*/

gint temp_col;/*设置雷区大小时的临时雷区埋雷总数*/

Mine_Area_Set *p=(Mine_Area_Set *)data;

if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(p->radio[0]))==TRUE)

{
	
	MINE_TOTAL=10;ROW=10;COL=10;
	
}

if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(p->radio[1]))==TRUE)

{
	
	MINE_TOTAL=40;ROW=16;COL=16;
	
}

if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(p->radio[2]))==TRUE)

{
	
	MINE_TOTAL=99;ROW=16;COL=30;
	
}

if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(p->radio[3]))==TRUE)

{
	
	temp_mine=atoi(gtk_entry_get_text(GTK_ENTRY(p->entry_mine)));
	
	if(temp_mine>=5&&temp_mine<=99){MINE_TOTAL=temp_mine;}else{MINE_TOTAL=99;}
	
	temp_row=atoi(gtk_entry_get_text(GTK_ENTRY(p->entry_row)));
	
	if(temp_row>=10&&temp_row<=16){ROW=temp_row;}else{ROW=16;}
	
	temp_col=atoi(gtk_entry_get_text(GTK_ENTRY(p->entry_col)));
	
	if(temp_col>=10&&temp_col<=30){COL=temp_col;} else{COL=30;}
	
}

gtk_widget_set_usize(window,COL*25,ROW*25+55);

gtk_widget_destroy(p->set_window);

game_init();

}

void do_nothing(GtkWidget set_window ,gpointer data)

{return;}

gint mid_click(gint row,gint col)

{
	if(mine_area[row][col].marked==TRUE)
	{
		return 1;
	}
	else return 0;
}

void mid_click2(gint row,gint col)
{
	if(game_over==FALSE && mine_area[row][col].opened==FALSE && mine_area[row][col].marked==FALSE)
	{
		open_block(row,col);
	}
	else 
	return;
}
