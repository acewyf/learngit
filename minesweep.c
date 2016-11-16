#include <stdlib.h>
#include <gtk/gtk.h>

gint ROW =10;/*��������*/
gint COL =10;/*��������*/
gint MINE_TOTAL =10;/*�����ܹ�������*/

typedef struct mine_block	/*����������ÿ���׿�Ľṹ��*/
{
	gint mine_around;/*��Χ�ж�����*/
	gboolean has_mine;/*�Ƿ�������*/
	gboolean marked;/*�Ƿ����Ҽ����*/
	gboolean opened;/*�Ƿ�������ڿ�*/
	GtkWidget *button;/*��ť*/
}mine_block,*map;

typedef struct MineAreaSet/*�������ô��ڵĽṹ��,�����źŴ���ʱ�Ķ��������*/
{
	GtkWidget *set_window;
	GtkWidget *radio[4];/*4����ѡ��ť(����һ���Զ��嵥ѡ��ť)*/
	GtkWidget *entry_mine;/*�����Զ�������ʱ��3�������ı���*/
	GtkWidget *entry_row;
	GtkWidget *entry_col;
	GSList *group;/*���ڵ�ѡ��ť������ָ��*/
}Mine_Area_Set;

gboolean gaming=FALSE;/*���ڱ�ʾ��Ϸ�Ľ���,�������ʱ�亯��������*/
gboolean game_over=FALSE;/*���ڱ�ʾ��Ϸ�Ľ���*/
gint mine_block_opened=0;/*���ڱ�ʾ�ܹ����ƿ����׿���Ŀ,�����ڱ�ʾ��Ϸ�ĳɹ�*/
gint mine_not_marked=99;/*���ڱ�ʾ�Ҽ�δʹ�õı�Ǵ���,Ĭ��99*/
gint game_time=0;/*���ڱ�ʾ��Ϸ���е�ʱ��*/
mine_block **mine_area;/*��malloc������̬����������������ռ�*/
GtkWidget *window;/*��Ϸ��������*/
GtkWidget *table;/*��������������ÿ����ť�ı�*/
GtkWidget *entry_time;/*������ʾʱ����ı���*/
GtkWidget *entry_mine;/*������ʾΪ����������ı���*/
GtkWidget *vbox;/*���������������ɲ˵���,��,ˮƽ����(������ʾʱ����ı���,��ǩ����ʾΪ����������ı����)���������*/

gboolean mouse_click(GtkWidget *widget ,GdkEventButton *event,gpointer data);/*����������굥���¼����źŴ���*/

void new_game_Handler(GtkMenuItem *menu_iterms ,gpointer data);/*�˵�new_game��ť���źŴ���*/

void about_Handler(GtkMenuItem *menu_iterms ,gpointer data);/*�˵�about�Ի�����źŴ���*/

void set_Handler(GtkMenuItem *menu_iterms ,gpointer data);/*�˵�setting��ť���źŴ���*/

void set_window_cancel(GtkButton *button ,gpointer data);/*���ô��ڵ�ȡ����ť���źŴ���*/

void set_window_apply(GtkButton *button ,gpointer data);/*���ô��ڵ�ȷ�ϰ�ť���źŴ���*/

void do_nothing(GtkWidget set_window ,gpointer data);/*Ϊ��ֹ������ô������Ͻ�С��������˳��ĺ���*/

void build_interface();/*���ڴ�����Ϸ����ĺ���*/

void game_init();/*��Ϸ��ʼ������*/

void new_game();/*��Ϸ��ʼ����*/

void open_block(gint row,gint col);/*����������׿�ĵݹ麯��*/

gint mid_click(gint row,gint col);/*�����м������ĺ����������Ƿ񱻱�ǣ����򷵻�1�����򷵻�0*/

void mid_click2(gint row,gint col);/*�����м������ĺ���������������ӦҪ��ʱ���׿�*/

gboolean time_box(gpointer data);/*���ڼ�ʱ*/

void mine_area_set();/*�������ô��ڵĽ�����������������С*/

void Game_over(gboolean result);/*��Ϸ��������*/

gint main( gint argc, gchar *argv[] )
{
	gtk_init(&argc, &argv);
	build_interface();/* ����һ����Ϸ������*/
	game_init();	/*��Ϸ��ʼ��,��ʼ��Ϸ*/
	gtk_widget_show_all(window);
	g_timeout_add(1000,(GSourceFunc)time_box,NULL);/*��ʱ������*/
	gtk_main();
	return 0;
}

void build_interface()
{
	
	gchar buffer[4];/*�洢mine_not_marked�ı���Ϣ�Ա���entry����ʾ*/
	
	GtkWidget *menu_bar;
	
	GtkWidget *game_menu;/*�˵����ϵ�game��ť*/
	
	GtkWidget *help_menu;/*�˵����ϵ�help��ť*/
	
	GtkWidget *menu_items;/*�����˵���İ�ť*/
	
	GtkWidget *menu;/*���ɸ����˵���İ�ť�Ĳ˵��б�*/
	
	GtkWidget *hbox;/*����������ʾʱ����ı���,��ǩ����ʾΪ����������ı����*/
	
	GtkWidget *label;/*������ʾTime����Not Mark�ı�ǩ*/
	
/* ����һ���´��� */
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	
	gtk_window_set_title (GTK_WINDOW (window), "GTK Mine");/*���ô��ڱ���*/
	
	gtk_widget_set_usize(window,COL*25,ROW*25+55);
	
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);/*��󻯰�ťʧЧ���û����ܵ������ڴ�С*/
	
	g_signal_connect (G_OBJECT (window), "delete_event",G_CALLBACK (gtk_main_quit), NULL);/*�����˳����źŴ���*/
	
/* ����һ���������,���ŵ������ڣ� */
	
	vbox = gtk_vbox_new (FALSE, 0);
	
	gtk_container_add (GTK_CONTAINER (window), vbox);
	
/* ����һ��ˮƽ�����,���ŵ��������� */
	
	hbox=gtk_hbox_new(TRUE,2);/*ƽ���ֲ�*/
	
	gtk_box_pack_end(GTK_BOX(vbox),hbox,FALSE,FALSE,0);/*��ӵ�������ӵ׶�,��ˮƽ�����������label��entry*/
	
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
	
/*�����˵������*/
	
	menu_bar=gtk_menu_bar_new();
	
	gtk_box_pack_start (GTK_BOX (vbox), menu_bar, FALSE, FALSE, 0);
	
	game_menu = gtk_menu_item_new_with_label("Game");
	
	help_menu =gtk_menu_item_new_with_label("Help");
	
/*����Ϸ�˵����Ͱ����˵�����ӵ��˵���*/
	
	gtk_menu_bar_append(GTK_MENU_BAR(menu_bar),game_menu);
	
	gtk_menu_bar_append(GTK_MENU_BAR(menu_bar),help_menu);
	
	menu=gtk_menu_new();/*�������ɵ��game_menu�˵���ťչ���Ĳ˵�*/
	
	menu_items=gtk_menu_item_new_with_label("New Game");
	
	gtk_menu_append(GTK_MENU(menu),menu_items);
	
	g_signal_connect(GTK_OBJECT(menu_items),"activate",G_CALLBACK(new_game_Handler),NULL);/*���¿�ʼ�ź�*/
	
	menu_items=gtk_menu_item_new();
	
	gtk_container_add(GTK_CONTAINER(menu_items),gtk_hseparator_new());
	
	gtk_menu_append(GTK_MENU(menu),menu_items);/*���ˮƽ������*/
	
	menu_items=gtk_menu_item_new_with_label("Setting...");
	
	gtk_menu_append(GTK_MENU(menu),menu_items);
	
	g_signal_connect(G_OBJECT(menu_items),"activate",G_CALLBACK(set_Handler),NULL);/*������С�����ź�*/
	
	menu_items=gtk_menu_item_new();
	
	gtk_container_add(GTK_CONTAINER(menu_items),gtk_hseparator_new());
	
	gtk_menu_append(GTK_MENU(menu),menu_items);/*���ˮƽ������*/
	
	menu_items=gtk_menu_item_new_with_label("Exit");
	
	gtk_menu_append(GTK_MENU(menu),menu_items);
	
	g_signal_connect(GTK_OBJECT(menu_items),"activate",G_CALLBACK(gtk_main_quit),NULL);/*��Ϸ�˳��ź�*/
	
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(game_menu),menu); /*���˵�����game_menu�˵���ť��*/
	
	menu=gtk_menu_new();/*�������ɵ��game_menu�˵���ťչ���Ĳ˵�*/
	
	menu_items=gtk_menu_item_new_with_label("How to...");
	
	gtk_menu_append(GTK_MENU(menu),menu_items);
	
	menu_items=gtk_menu_item_new();
	
	gtk_container_add(GTK_CONTAINER(menu_items),gtk_hseparator_new());
	
	gtk_menu_append(GTK_MENU(menu),menu_items);/*���ˮƽ������*/
	
	menu_items=gtk_menu_item_new_with_label("About");
	
	gtk_menu_append(GTK_MENU(menu),menu_items);
	
	g_signal_connect(GTK_OBJECT(menu_items),"activate",G_CALLBACK(about_Handler),NULL);/*��Ϸabout�ź�*/
	
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_menu),menu);/*���˵�����help_menu�˵���ť��*/
	
	return ;
	
}

void game_init()

{	static gint temp_row=0;

gint row,col;

static gboolean first_time=TRUE;

gint n=-1;/*���ڼ���ڼ���mine_block Ӧ�ô�0��ʼ�㣬��ROE*COL-1*/

if(first_time==FALSE)

{
	
	gtk_widget_destroy(table);/*ԭ�����Ĩȥ�����½������*/
	
	table=gtk_table_new(ROW,COL,TRUE);
	
/*����ԭ����ά����ռ�*/
	
	for(row=0;row<temp_row;row++)
	
	{
		
		free(mine_area[row]);
		
		mine_area[row]=NULL;
		
	}
	
	free(mine_area);
	
	mine_area=NULL;
	
	gtk_box_pack_start(GTK_BOX(vbox),table,TRUE,TRUE,2);/*�����������ı������������*/
	
}

else

{
	
	table=gtk_table_new(ROW,COL,TRUE);
	
	gtk_box_pack_start(GTK_BOX(vbox),table,TRUE,TRUE,2);/*�����������ı����� �������*/
	
}

/*��ʾ��������ά����*/

gtk_widget_show(table);

mine_area=(map *)malloc(ROW*sizeof(map));

for(row=0;row<ROW;row++){mine_area[row]=(mine_block *)malloc(COL*sizeof(mine_block) );}

for(row=0;row<ROW;row++)

{
	
	for(col=0;col<COL;col++)
	
	{
		
		n++;
		
		mine_area[row][col].button=gtk_toggle_button_new();
		
		g_signal_connect(GTK_OBJECT(mine_area[row][col].button),"button-press-event",G_CALLBACK(mouse_click),(gpointer)(n));/*��굥���¼����ź�*/
		
		gtk_table_attach_defaults(GTK_TABLE(table),mine_area[row][col].button,col,col+1,row,row+1);
		
		g_object_set(GTK_OBJECT(mine_area[row][col].button),"can-focus", FALSE, NULL);/*ʧȥ����*/
		
		gtk_widget_set_usize(mine_area[row][col].button,25,25);
		
		gtk_widget_show(mine_area[row][col].button);
		
	}
	
}

first_time=FALSE;

temp_row=ROW;

new_game();/*������Ϸ��ʼ����*/

}

void new_game()/*���¿�ʼ��Ϸ�����²���*/

{
	
	gint i=0;/*�ۼ����������*/
	
	gint row,col;
	
	gint random;
	
	gchar buffer[4];/*�洢mine_not_marked�ı���Ϣ�Ա���entry����ʾ*/
	
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
			
			gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"");/*Ĭ���׿鰴ť��ʾ�հ�*/
			
		}
		
	}
	
	while(i<MINE_TOTAL)/*����Ϊ�������*/
	
	{
		
		random =g_random_int_range(0,ROW*COL);
		
		row=random/COL;
		
		col=random%COL;
		
		if(mine_area[row][col].has_mine==TRUE)/*����Ѿ��������*/
		
		{continue;}
		
		mine_area[row][col].has_mine=TRUE;
		
		i++;
		
		if(row>0)
		
		{
			
			mine_area[row-1][col].mine_around++;/*��++*/
			
			if(col>0){mine_area[row-1][col-1].mine_around++;}/*����++*/
			
			if(col<COL-1){mine_area[row-1][col+1].mine_around++;}/*����++*/
			
		}
		
		if(row<ROW-1)
		
		{
			
			mine_area[row+1][col].mine_around++;/*��++*/
			
			if(col>0){mine_area[row+1][col-1].mine_around++;}/*����++*/
			
			if(col<COL-1){mine_area[row+1][col+1].mine_around++;}/*����++*/
			
		}
		
		if(col>0){mine_area[row][col-1].mine_around++;}/*��++*/
		
		if(col<COL-1){mine_area[row][col+1].mine_around++;}/*��++*/
		
	}
	
}

void open_block(gint row,gint col)

{
	
	static gchar buffer[4];
	
	if(game_over==TRUE || mine_area[row][col].marked==TRUE || mine_area[row][col].opened==TRUE)
	
	{return;}/*�����Ϸ����,���߱��Ҽ���ǹ�,�����Ѿ����򿪹�,�����ٽ��д򿪲���*/
	
	mine_area[row][col].opened=TRUE;/*�������������,���Ϊ��*/
	
	gaming=TRUE;/*��Ϸ��ʼ*/
	
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mine_area[row][col].button),TRUE);/*���ð�ť�ʰ���״*/
	
	if(mine_area[row][col].has_mine==TRUE)/*����պ����������ף���Ϸ����*/
	
	{
		
		gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"*");
		
		Game_over(FALSE);
		
		return;
		
	}
	
	if(mine_area[row][col].mine_around!=0)/*����������ף�����Χ
	
	����,����ʾ��Χ����*/
	
	{
		
		g_snprintf(buffer,4,"%d",mine_area[row][col].mine_around);
		
		gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),buffer);
		
	}
	
	mine_block_opened++;/*�׿��ֱ��ƿ�һ��*/
	
	if(mine_block_opened+MINE_TOTAL==ROW*COL)/*��Ϸʤ��*/
	
	{
		
		Game_over(TRUE);return;
		
	}
	
	if(mine_area[row][col].mine_around==0)/*�������ף�����Χ������,���Զ�����Χ�׿�*/
	
	{
		
		if(row>0)
		
		{
			
			open_block(row-1,col);/*����*/
			
			if(col>0){open_block(row-1,col-1);}/*������*/
			
			if(col<COL-1){open_block(row-1,col+1);}/*������*/
			
		}
		
		if(row<ROW-1)
		
		{
			
			open_block(row+1,col);/*����*/
			
			if(col>0){open_block(row+1,col-1);}/*������*/
			
			if(col<COL-1){open_block(row+1,col+1);}/*������*/
			
		}
		
		if(col>0){open_block(row,col-1);}/*����*/
		
		if(col<COL-1){open_block(row,col+1);}/*����*/
		
	}
	
	return;
	
}

gboolean time_box(gpointer data)

{
	
	gchar buffer[4];
	
	if (game_over==FALSE && gaming==TRUE)/*��Ϸû�����Ź���,�����Ϸ��ʼ���У���ʱ������*/
	
	{
		
		game_time++;
		
		g_snprintf(buffer,4,"%d",game_time);
		
		gtk_entry_set_text(GTK_ENTRY(entry_time),buffer);
		
	}
	
	return TRUE;/*����TRUE������ʱ*/
	
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

/*�������ô���*/

p->set_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_widget_set_usize(p->set_window, 420, 300);

gtk_widget_set_uposition(p->set_window,420, 300);

gtk_window_set_title (GTK_WINDOW (p->set_window), "Mine_Area_Set");

gtk_window_set_resizable(GTK_WINDOW(p->set_window),FALSE);/*��󻯰�ťʧЧ���û����ܵ������ڴ�С*/

g_signal_connect (G_OBJECT (p->set_window), "delete_event",G_CALLBACK (do_nothing), NULL);

gtk_window_set_modal(GTK_WINDOW(p->set_window),TRUE);/*��겻�ܻص������棬��������ź�*/

table=gtk_table_new(5,5,TRUE);

gtk_container_add(GTK_CONTAINER(p->set_window),table);

/*�ĸ���ѡ��ť*/

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

/*�Զ������õ�3���ı���*/

p->entry_mine=gtk_entry_new_with_max_length(2);

gtk_entry_set_text(GTK_ENTRY(p->entry_mine),"99");

gtk_table_attach_defaults(GTK_TABLE(table),p->entry_mine,4,5,1,2);

p->entry_row=gtk_entry_new_with_max_length(2);

gtk_entry_set_text(GTK_ENTRY(p->entry_row),"16");

gtk_table_attach_defaults(GTK_TABLE(table),p->entry_row,4,5,2,3);

p->entry_col=gtk_entry_new_with_max_length(2);

gtk_entry_set_text(GTK_ENTRY(p->entry_col),"30");

gtk_table_attach_defaults(GTK_TABLE(table),p->entry_col,4,5,3,4);

button=gtk_button_new_with_label("Apply");/*ȷ�ϰ�ť�����ź�*/

gtk_table_attach_defaults(GTK_TABLE(table),button,3,4,4,5);

g_signal_connect (G_OBJECT (button), "pressed",G_CALLBACK (set_window_apply), p);

button=gtk_button_new_with_label("Cancel");/*ȡ����ť�����ź�*/

gtk_table_attach_defaults(GTK_TABLE(table),button,4,5,4,5);

g_signal_connect (G_OBJECT (button),"pressed",G_CALLBACK (set_window_cancel), p->set_window);

gtk_widget_show_all(p->set_window);

return ;

}

void Game_over(gboolean result)

{
	
	GtkWidget *dialog;
	
	gint row,col;
	
	gint num=0;/*����ʧ��ʱͳ�Ƴɹ���ǵ�����*/
	
	gchar msg[40];/*�����Ϣ*/
	
	game_over=TRUE;/*��Ϸ������*/
	
	for(row=0;row<ROW;row++)/*��Ϸ������ʾ����������Ϣ*/
	
	{
		
		for(col=0;col<COL;col++)
		
		{
			
			if(mine_area[row][col].opened==FALSE)/*����û���򿪵��׿�*/
			
			{
				
/*�����׿����ڰ�ť��toggle*/
				
				gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mine_area[row][col].button),TRUE);
				
				gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"");/*Ĭ��label����ʾ��Ϣ*/
				
				if(mine_area[row][col].has_mine==FALSE)
				
				{
					
/*if(mine_area[row][col].mine_around>0)
					
					{
						
						g_snprintf(buffer,4,"%d",mine_area[row][col].mine_around);
						
						gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),buffer);
						
					}*/
					
					if(mine_area[row][col].marked==TRUE)/*���׵��Ҽ����*/
					
					{gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"W");}
					
				}
				
				else/*����*/
				
				{
					
					if(mine_area[row][col].marked==FALSE && result==FALSE)/*���׵����Ҽ������ʧ��*/
					
					{
						
						gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mine_area[row][col].button),FALSE);
						
						gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"*?");
						
					}
					
					else/*�������б�� ���� ʤ��*/
					
					{gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"R");num++;}
					
				}
				
			}
			
		}
		
	}
	
	if(result==TRUE)/*���ʤ��*/
	
	{
		
		g_snprintf(msg,40,"You Are Great!Congratulations!");
		
	}
	
	else/*���ʧ��*/
	
	{
		
		g_snprintf(msg,40,"You Failed!So Poor!\nMark %d mines",num);
		
	}
	
/*��ʾ�Ի���*/
	
	dialog=gtk_message_dialog_new(GTK_WINDOW(window),0,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,msg);
	
	gtk_dialog_run(GTK_DIALOG(dialog));
	
	gtk_widget_destroy(dialog);
	
}

gboolean mouse_click(GtkWidget *widget ,GdkEventButton *event,gpointer data)

{
	
	gint k=0;/*k���ڼ�¼ĳ���׿���Χ�ж��ٱ����*/
	
	gint n;/*��n=(gint)data,�����֪���ǵ�n���׿��Ϸ�����������¼�*/
	
	gint row,col;
	
	gchar buffer[4];/*���mine_not_marked �����ݣ��Ա�������entry����ʾ*/
	
	n=(gint)data;
	
	row= n/COL;
	
	col=n%COL;
	
	if(game_over==FALSE)/*���������������Ϸû��������½���*/
	
	switch(event->button)
	
	{
		
		case 1:/*�������*/
		
		{
			
			open_block(row,col);/*������׿�*/
			
			break;
			
		}
		
		case 2:/*�м�����*/
		
		{
			
			if(mine_area[row][col].marked==FALSE && mine_area[row][col].mine_around>0 &&mine_area[row][col].opened==TRUE)
			
			{
				
				if(row>0)
				
				{
					
					k+=mid_click(row-1,col);/*����*/
					
					if(col>0){k+=mid_click(row-1,col-1);}/*������*/
					
					if(col<COL-1){k+=mid_click(row-1,col+1);}/*������*/
					
				}
				
				if(row<ROW-1)
				
				{
					
					k+=mid_click(row+1,col);/*����*/
					
					if(col>0){k+=mid_click(row+1,col-1);}/*������*/
					
					if(col<COL-1){k+=mid_click(row+1,col+1);}/*������*/
					
				}
				
				if(col>0){k+=mid_click(row,col-1);}/*����*/
				
				if(col<COL-1){k+=mid_click(row,col+1);}/*����*/
				
				if(k>=mine_area[row][col].mine_around)/*���Ǵ��ڹ�������*/
				
				{
					
					if(row>0)
					
					{
						
						mid_click2(row-1,col);/*����*/
						
						if(col>0){mid_click2(row-1,col-1);}/*������*/
						
						if(col<COL-1){mid_click2(row-1,col+1);}/*������*/
						
					}
					
					if(row<ROW-1)
					
					{
						
						mid_click2(row+1,col);/*����*/
						
						if(col>0){mid_click2(row+1,col-1);}/*������*/
						
						if(col<COL-1){mid_click2(row+1,col+1);}/*������*/
						
					}
					
					if(col>0){mid_click2(row,col-1);}/*����*/
					
					if(col<COL-1){mid_click2(row,col+1);}/*����*/
					
				}
				
			}
			
			break;
			
		}
		
		case 3:/*�Ҽ�����*/
		
		{
			
			if(mine_area[row][col].opened==TRUE)/*�Ѿ�������򿪹�*/
			
			{break;}
			
			if(mine_area[row][col].marked==TRUE)/*�Ѿ����Ҽ���ǹ������һ���ȡ�����*/
			
			{
				
				mine_area[row][col].marked=FALSE;
				
				mine_not_marked++;
				
				gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"");
				
			}
			
			else if(mine_not_marked>0)
			
			{ /*������ʾ M ��ʾ�������*/
			
			mine_area[row][col].marked=TRUE;
			
			gtk_button_set_label(GTK_BUTTON(mine_area[row][col].button),"M");
			
			mine_not_marked--;
			
		}
		
	}
	
}

g_snprintf(buffer,4,"%d",mine_not_marked);/*���mine_not_marked �����ݣ��Ա�������entry����ʾ*/

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

gint temp_mine;/*����������Сʱ����ʱ��������*/

gint temp_row;/*����������Сʱ����ʱ��������*/

gint temp_col;/*����������Сʱ����ʱ������������*/

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
