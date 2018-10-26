#include"MMI_features.h"
#include"PixtelDataTypes.h"
#include"gdi_include.h"
#include"wgui.h"
#include"gui.h"
#include"Globaldefs.h"
#include"CustDataRes.h"
#include"gui_themes.h"
#include"wgui_categories.h"
#include"wgui_softkeys.h"
#include"HistoryGprot.h"
#include"MyGameProt.h"
#include"mmi_rp_app_mainmenu_def.h"
//#include"mmi_rp_app_myapp_def.h"
#include "mmi_rp_app_idle_def.h"
#include "menucuigprot.h"
#include "mmi_rp_app_phonesetting_new_def.h"
#include "gui_inputs.h"
#include "common_nvram_editor_data_item.h"
#include "nvram_common_defs.h"


typedef enum num{
	num0 = 0,
	num2 = 2,
	num4 = 4,
	num8 = 8,
	num16 = 16,
	num32 = 32,
	num64 = 64,
	num128 = 128,
	num256 = 256,
	num512 = 512,
	num1024 = 1024,
	num2048 = 2048
}num;

#define disparity   52
#define mygame_y    70
#define mygame_x    20
#define score_x     30
#define score_y     30
#define failed_x1   20
#define failed_x2   220
#define failed_y1   120
#define failed_y2   220
#define name_input_w  100
#define name_input_h  30
#define name_input_length  1024
#define time_x1     170
#define time_x2     233
#define time_y1     30
#define time_y2     50
#define over_x      70
#define over_y      150
#define grade_x     70
#define grade_y     30
#define grade_x1    90
#define grade_y1    100
#define grade_y2    140
#define grade_y3    180
#define grade_y4    220
#define grade_y5    260
#define a_row       4  //行数
#define a_column    4   //列数
#define add_Proba    5   //概率


MYGAME_STRUCT mygame_struct;
MYGAME_GRADE_STRUCT mygame_grade_struct;
MYGAME_GRADE_NAME_STRUCT mygame_grade_name_struct;

int a[4][4] = {
				{0,0,0,0},
				{0,0,0,0},
				{0,0,0,0},
				{0,0,0,0}
				};
int x,y;
int t = 0;
int memory_num;
int null_num = 16;
BOOL mygame_is_continue = FALSE;
int mygame_is_over; 
int score1 = 0;
int score2 = 0;
int score3 = 0;
int score4 = 0;
int score5 = 0;
char name1[20] = "null";
char name2[20] = "null";
char name3[20] = "null";
char name4[20] = "null";
char name5[20] = "null";
char input_name[20] = "";
char init_name[20] = "";
BOOL add_grade = FALSE;
int game_over;
single_line_input_box b;
int total_score = 0;

static mmi_ret mygame_proc(mmi_event_struct *evt)
{
	cui_menu_event_struct *menu_evt = (cui_menu_event_struct*)evt;//将事件强制类型转换
		 switch(menu_evt->evt_id)
		 {
		 	case EVT_ID_CUI_MENU_LIST_ENTRY:
				mygame_status_read();
				
				cui_menu_set_currlist_flags(menu_evt->sender_id,CUI_MENU_NORMAL_LIST);
			
				if(mygame_is_continue == FALSE){
					cui_menu_set_item_hidden(menu_evt->sender_id, MENU_ID_MYGAME_CONTINUE, MMI_TRUE);
				}
				else{
					cui_menu_set_item_hidden(menu_evt->sender_id, MENU_ID_MYGAME_CONTINUE, MMI_FALSE);
					}
				
			break;
			case EVT_ID_CUI_MENU_ITEM_SELECT:
				if(menu_evt->highlighted_menu_id == MENU_ID_MYGAME_NEWGAME){
					mmi_mygame_entry();
					}
				if(menu_evt->highlighted_menu_id == MENU_ID_MYGAME_CONTINUE){
					mygame_read_body();
					mygame_read_time();
					mygame_read_score();
					mmi_mygame_continue_entry();
					}
				if(menu_evt->highlighted_menu_id == MENU_ID_MYGAME_GRADE){
					mygame_read_grade();
					mmi_mygame_grade_entry();
					}
			break;
			case EVT_ID_CUI_MENU_CLOSE_REQUEST:
			{
				cui_menu_close(menu_evt->sender_id);		
				}
			break;
			  default: break;
		 }
		 return MMI_RET_OK;
}

void mygame_cui_menu_enter()
{
	mmi_id mygame_cui_menu_id;
	mmi_frm_group_create(GRP_ID_ROOT, GRP_ID_MY_GROUPA, mygame_proc, NULL);
	mmi_frm_group_enter(GRP_ID_MY_GROUPA, MMI_FRM_NODE_SMART_CLOSE_FLAG);
	mygame_cui_menu_id = cui_menu_create(GRP_ID_MY_GROUPA, 
							CUI_MENU_SRC_TYPE_RESOURCE,
							CUI_MENU_TYPE_FROM_RESOURCE,
							MENU_ID_MYGAME,
							MMI_FALSE,
							NULL);
	cui_menu_run(mygame_cui_menu_id);
}

void mmi_mygame_entry(void){
	mmi_frm_group_create(GRP_ID_MY_GROUPA,GRP_ID_MY_GROUPB,NULL,NULL);
	mmi_frm_group_enter(GRP_ID_MY_GROUPB,MMI_FRM_NODE_SMART_CLOSE_FLAG);
	enter_mygame_screen();
}
void mmi_mygame_continue_entry(void){
	mmi_frm_group_create(GRP_ID_MY_GROUPA,GRP_ID_MY_GROUPB,NULL,NULL);
	mmi_frm_group_enter(GRP_ID_MY_GROUPB,MMI_FRM_NODE_SMART_CLOSE_FLAG);
	enter_mygame_continue_screen();
}

void mmi_mygame_grade_entry(void){
	mmi_frm_group_create(GRP_ID_MY_GROUPA,GRP_ID_MY_GROUPC,NULL,NULL);
	mmi_frm_group_enter(GRP_ID_MY_GROUPC,MMI_FRM_NODE_SMART_CLOSE_FLAG);
	enter_mygame_grade_screen();
}
void enter_mygame_screen(void){
	mmi_frm_scrn_enter(GRP_ID_MY_GROUPB,SCR_ID_MY_SCREENA, close_mygame_scrna,enter_mygame_screen, MMI_FRM_FULL_SCRN);
	clear_screen();
	mygame_show_bg();
	mygame_init_a();
	mygame_add_num();
	mygame_time();
	mygame_score();
	strcpy(input_name,init_name);
	mygame_is_continue = TRUE;
	mygame_status_save();
	gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
	SetKeyHandler(close_mygame_scrna, KEY_RSK, KEY_EVENT_UP);
	SetKeyHandler(mygame_moveLeft, KEY_LEFT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mygame_moveRight, KEY_RIGHT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mygame_moveUp, KEY_UP_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mygame_moveDown, KEY_DOWN_ARROW, KEY_EVENT_UP);
}
void close_mygame_scrna(void){
	StopTimer(MYGAME_TIME);
	mygame_save_body();
	mmi_frm_scrn_close_active_id();
}

void enter_mygame_continue_screen(void){
	mmi_frm_scrn_enter(GRP_ID_MY_GROUPB,SCR_ID_MY_SCREENC, close_mygame_scrna,enter_mygame_continue_screen, MMI_FRM_FULL_SCRN);
	clear_screen();
	mygame_show_bg();
	mygame_time();
	mygame_score();
	mygame_show_pic();
	strcpy(input_name,init_name);
	gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
	SetKeyHandler(close_mygame_scrna, KEY_RSK, KEY_EVENT_UP);
	SetKeyHandler(mygame_moveLeft, KEY_LEFT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mygame_moveRight, KEY_RIGHT_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mygame_moveUp, KEY_UP_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mygame_moveDown, KEY_DOWN_ARROW, KEY_EVENT_UP);
	SetKeyHandler(mygame_input_save,KEY_LSK,KEY_EVENT_UP);
}
void enter_mygame_grade_screen(void){
	UI_filled_area filler={0};
	S32 w,h;
	mmi_frm_scrn_enter(GRP_ID_MY_GROUPC,SCR_ID_MY_SCREENB, close_mygame_grade_scrna,enter_mygame_grade_screen, MMI_FRM_FULL_SCRN);
	clear_screen();
	filler.flags = UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_BORDER|UI_FILLED_AREA_SHADOW;
	filler.c = UI_COLOR_GREY;
	filler.border_color = UI_COLOR_DARK_GREY;
	filler.shadow_color = UI_COLOR_3D_FILLER;
	gui_draw_filled_area(0, 0, UI_device_width-1, UI_device_height-1,&filler);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_measure_string((UI_string_type)GetString(MYGAME_MENU_GRADE_TEXT),&w,&h);
	gui_move_text_cursor((UI_device_width-w)/2,(UI_device_height-h)/6);
	gui_set_font(&MMI_large_font);
	gui_set_text_border_color(UI_COLOR_RED);
	gui_print_bordered_text((UI_string_type)GetString(MYGAME_MENU_GRADE_TEXT));
	mygame_best_grade();
	gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
	SetKeyHandler(close_mygame_grade_scrna, KEY_RSK, KEY_EVENT_UP);
	SetKeyHandler(mygame_init_grade, KEY_LSK, KEY_EVENT_UP);
}
void close_mygame_grade_scrna(void){
	mmi_frm_scrn_close_active_id();
}

void mygame_show_bg(void){
	UI_filled_area filler={0};
	filler.flags = UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_BORDER|UI_FILLED_AREA_SHADOW;
	filler.c = UI_COLOR_GREY;
	filler.border_color = UI_COLOR_DARK_GREY;
	filler.shadow_color = UI_COLOR_3D_FILLER;
	gui_draw_filled_area(0, 0, UI_device_width-1, UI_device_height-1,&filler);
	gdi_image_draw_resized_id(0,0,UI_device_width-1, UI_device_height-1, IMG_MYGAME_0);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_move_text_cursor(score_x,score_y);
	gui_set_font(&MMI_large_font);
	gui_set_text_border_color(UI_COLOR_RED);
	gui_print_bordered_text((UI_string_type)GetString(MYGAME_SCORE_TEXT));
	gui_draw_rectangle(mygame_x-1,mygame_y-1,mygame_x+4*disparity-1,mygame_y+4*disparity-1,UI_COLOR_RED);
	gui_line(mygame_x-1,mygame_y+disparity-1,mygame_x+4*disparity-1,mygame_y+disparity-1,UI_COLOR_RED);
	gui_line(mygame_x-1,mygame_y+2*disparity-1,mygame_x+4*disparity-1,mygame_y+2*disparity-1,UI_COLOR_RED);
	gui_line(mygame_x-1,mygame_y+3*disparity-1,mygame_x+4*disparity-1,mygame_y+3*disparity-1,UI_COLOR_RED);
	gui_line(mygame_x+disparity-1,mygame_y-1,mygame_x+disparity-1,mygame_y+4*disparity-1,UI_COLOR_RED);
	gui_line(mygame_x+2*disparity-1,mygame_y-1,mygame_x+2*disparity-1,mygame_y+4*disparity-1,UI_COLOR_RED);
	gui_line(mygame_x+3*disparity-1,mygame_y-1,mygame_x+3*disparity-1,mygame_y+4*disparity-1,UI_COLOR_RED);
	
	gui_set_font(&MMI_small_font);
	gui_set_text_color(UI_COLOR_BLACK );
	gui_move_text_cursor(UI_device_width-50,UI_device_height - 25);
    gui_print_text((UI_string_type)GetString(MYGAME_EXIT_TEXT));
}
void mygame_init_a(void){
	int i,j;
	t = 0;
	for(i=0; i<a_row; i++){
		for(j=0; j<a_column; j++){
			a[i][j]=0;
		}
	}
	total_score = 0;
}
void mygame_init_grade(void){
	score1 = score2 = score3 = score4 = score5 =0;
	strcpy(name1,"null");
	strcpy(name2,"null");
	strcpy(name3,"null");
	strcpy(name4,"null");
	strcpy(name5,"null");
	mygame_save_grade();
	mygame_read_grade();
	mygame_best_grade();
	gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}
void mygame_add_num(void){
	int i, j, x1, y1, n,num;
	srand((unsigned int)time(0));
	while(1)
	{
		i=rand()%a_row;
		j=rand()%a_column;
		n=rand()%add_Proba; //设置生成随机24的概率
		if(n==1){
			num=4;
		}else {
			num=2;
		}
		memory_num =  num;
		if(a[i][j]==0)
		{
			a[i][j]=num;
			break;
		}
	}
	if(i==0){
		y1 = mygame_y;
	}else if(i== 1){
		y1 = mygame_y + disparity;
	}else if(i == 2){
		y1 = mygame_y + 2*disparity;
	}else if(i == 3){
		y1 = mygame_y + 3*disparity;
	}
	if(j == 0){
		x1 = mygame_x;
	}else if(j == 1){
		x1 = mygame_x + disparity;
	}else if(j == 2){
		x1 = mygame_x + 2*disparity;
	}else if(j == 3){
		x1 = mygame_x + 3*disparity;
	}
	if(a[i][j] == num0){
		gdi_image_draw_id(x1, y1, IMG_MYGAME_0);
	}
	if(a[i][j] == num2){
		gdi_image_draw_id(x1, y1, IMG_MYGAME_2);
	}
	if(a[i][j] == num4){
		gdi_image_draw_id(x1, y1, IMG_MYGAME_4);
	}
	gui_BLT_double_buffer(mygame_x,mygame_y,mygame_x+4*disparity,mygame_y+4*disparity);
}

void mygame_score(void){
	CHAR score[20] = {0};
	U16 temp[40];
	UI_filled_area filler={0};
	
	total_score = total_score + memory_num;
	mygame_save_score();
	mygame_show_score();
	memory_num = 0;
}
void mygame_time(void){
	int ss,mm;
	CHAR ss1[20] = {0};
	U16 temp1[40];
	CHAR mm1[20] = {0};
	U16 temp2[40];
	UI_filled_area filler={0};
	t = t+1;
	mm = t % 3600 / 60;
	ss = t % 60;
	filler.c = UI_COLOR_GREY;
	gui_draw_filled_area(time_x1,time_y1,time_x2,time_y2,&filler);
	gui_move_text_cursor(time_x1,time_y1);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_set_font(&MMI_large_font);
	sprintf(mm1,"%d",mm);
	mmi_asc_to_ucs2((CHAR *) temp2, (CHAR *) mm1);
	gui_print_text(temp2);

	gui_move_text_cursor(time_x1+20,time_y1);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_set_font(&MMI_large_font);
	gui_print_text(L" : ");

	gui_move_text_cursor(time_x1+40,time_y1);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_set_font(&MMI_large_font);
	sprintf(ss1,"%d",ss);
	mmi_asc_to_ucs2((CHAR *) temp1, (CHAR *) ss1);
	gui_print_text(temp1);
	
	mygame_save_time();
	gui_BLT_double_buffer(time_x1,time_y1,time_x2,time_y2);
	StartTimer(MYGAME_TIME, 1000, mygame_time);
}
int mygame_null_count()
{
	int i,j;
	null_num = a_row*a_column;
	for(i=0; i<a_row; i++){
		for(j=0;j<a_column;j++){
			if(a[i][j] != 0)
				null_num--;
		}
	}
	return null_num;
}
void mygame_moveLeft(void){
	int i,n,j,k;
	for(i=0; i<a_row; i++){
		k=0;
		for(n=0;n<a_column;n++){
			if(a[i][n] != 0){
				for(j=n;j>k;j--){
					if(a[i][j-1] == 0){
						a[i][j-1] = a[i][j];
						a[i][j] = 0;
					}
					else if(a[i][j-1] == a[i][j]){
						a[i][j-1] = 2*a[i][j];
						a[i][j] = 0;
						k=j;//标记，单次已经合并过的不再继续合并
					}
					}
				}
		}
	}
	mygame_show_pic();
	mygame_null_count();
	if(null_num != 0){
		mygame_add_num();
		mygame_over();
		mygame_check();
	}
	mygame_score();
	
	/*UI_filled_area filler={0};
	game_over = 1;
	if(game_over == 1){
		filler.flags = UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_BORDER|UI_FILLED_AREA_SHADOW;
		filler.c = UI_COLOR_GREY;
		filler.border_color = UI_COLOR_DARK_GREY;
		filler.shadow_color = UI_COLOR_3D_FILLER;
		gui_draw_filled_area(failed_x1, failed_y1, failed_x2, failed_y2,&filler);
		gui_set_text_color(UI_COLOR_BLACK);
		gui_move_text_cursor(over_x,over_y);
		gui_set_font(&MMI_large_font);
		gui_set_text_border_color(UI_COLOR_RED);
		gui_print_bordered_text((UI_string_type)GetString(MYGAME_OVER_TEXT));
		gui_set_font(&MMI_small_font);
		gui_set_text_color(UI_COLOR_BLACK );
		gui_move_text_cursor(10,UI_device_height - 25);
   		gui_print_text((UI_string_type)GetString(MYGAME_SAVE_TEXT));
		
		StopTimer(MYGAME_TIME);
		if(total_score > score5){
			gui_create_single_line_input_box(&b,over_x,over_y+35,name_input_w,name_input_h,name_input_length);
			gui_show_single_line_input_box(&b);
			SetKeyHandler(mygame_input_a,KEY_1 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_b,KEY_2 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_c,KEY_3 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_d,KEY_4 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_e,KEY_5 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_f,KEY_6 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_g,KEY_7 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_h,KEY_8 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_i,KEY_9 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_save,KEY_LSK,KEY_EVENT_UP);
		}
		mygame_is_continue = FALSE;
		mygame_status_save();
		add_grade = TRUE;
		gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
	}*/
}
void mygame_moveRight(void){
	int i,n,j,k;
	for(i=0; i<a_row; i++){
		k=a_column-1;
		for(n=a_column-1;n>=0;n--){
			if(a[i][n] != 0){
				for(j=n;j<k;j++){
					if(a[i][j+1] == 0){
						a[i][j+1] = a[i][j];
						a[i][j] = 0;
					}
					else if(a[i][j+1] == a[i][j]){
						a[i][j+1] = 2*a[i][j];
						a[i][j] = 0;
						k=j;
					}
					}
				}
		}
	}
	mygame_show_pic();
	mygame_null_count();
	if(null_num != 0){
		mygame_add_num();
		mygame_over();
		mygame_check();
	}
	mygame_score();
}
void mygame_moveUp(void){
	int i,n,j,k;
	for(n=0; n<a_column; n++){
		k=0;
		for(i=0;i<a_row; i++){
			if(a[i][n] != 0){
				for(j=i;j>k;j--){
					if(a[j-1][n] == 0){
						a[j-1][n] = a[j][n];
						a[j][n] = 0;
					}
					else if(a[j-1][n] == a[j][n]){
						a[j-1][n] = 2*a[j][n];
						a[j][n] = 0;
						k=j;
					}
					}
				}
		}
	}
	mygame_show_pic();
	mygame_null_count();
	if(null_num != 0){
		mygame_add_num();
		mygame_over();
		mygame_check();
	}
	mygame_score();
}
void mygame_moveDown(void){
	int i,n,j,k;
	for(n=0; n<a_column; n++){
		k=3;
		for(i=a_row;i>=0; i--){
			if(a[i][n] != 0){
				for(j=i;j<k;j++){
					if(a[j+1][n] == 0){
						a[j+1][n] = a[j][n];
						a[j][n] = 0;
					}
					else if(a[j+1][n] == a[j][n]){
						a[j+1][n] = 2*a[j][n];
						a[j][n] = 0;
						k=j;
					}
					}
				}
		}
	}
	mygame_show_pic();
	mygame_null_count();
	if(null_num != 0){
		mygame_add_num();
		mygame_over();
		mygame_check();
	}
	mygame_score();
}
int mygame_over(){
	int i;
	for (i = 0; i < a_row; i++) {
		int j;
		for (j = 0; j < a_column-1; j++) {
			if (a[i][j] == a[i][j + 1] || a[j][i] == a[j + 1][i]||a[i][j] == 0||a[a_row-1][a_column-1]==0){
				game_over = 0;
				return game_over;
			}
       	}
    }
	game_over = 1;
	return game_over;
}
void mygame_check(void){
	int j,k;
	int n,m;
	UI_filled_area filler={0};
	
	if(game_over == 1){
		filler.flags = UI_FILLED_AREA_TYPE_COLOR|UI_FILLED_AREA_BORDER|UI_FILLED_AREA_SHADOW;
		filler.c = UI_COLOR_GREY;
		filler.border_color = UI_COLOR_DARK_GREY;
		filler.shadow_color = UI_COLOR_3D_FILLER;
		gui_draw_filled_area(failed_x1, failed_y1, failed_x2, failed_y2,&filler);
		gui_set_text_color(UI_COLOR_BLACK);
		gui_move_text_cursor(over_x,over_y);
		gui_set_font(&MMI_large_font);
		gui_set_text_border_color(UI_COLOR_RED);
		gui_print_bordered_text((UI_string_type)GetString(MYGAME_OVER_TEXT));
		gui_set_font(&MMI_small_font);
		gui_set_text_color(UI_COLOR_BLACK );
		gui_move_text_cursor(10,UI_device_height - 25);
   		gui_print_text((UI_string_type)GetString(MYGAME_SAVE_TEXT));
		
		StopTimer(MYGAME_TIME);
		if(total_score > score5){
			gui_create_single_line_input_box(&b,over_x,over_y+35,name_input_w,name_input_h,name_input_length);
			gui_show_single_line_input_box(&b);
			SetKeyHandler(mygame_input_a,KEY_1 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_b,KEY_2 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_c,KEY_3 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_d,KEY_4 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_e,KEY_5 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_f,KEY_6 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_g,KEY_7 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_h,KEY_8 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_i,KEY_9 ,KEY_EVENT_UP);
			SetKeyHandler(mygame_input_save,KEY_LSK,KEY_EVENT_UP);
			
		}
		mygame_is_continue = FALSE;
		mygame_status_save();
		add_grade = TRUE;
		gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
	}
}
void mygame_input_save(void){
	int i,j;
	int new_score = 0;
	
	for(i=0;i<a_row;i++){
		for(j=0;j<a_column;j++){
			new_score = new_score + a[i][j];
		}
	}
	if(score1==0&&score2==0&&score3==0&&score4==0&&score5==0){
		score1 = new_score;
		strcpy(name1,input_name);
	}else if(score1!=0&&score2==0&&score3==0&&score4==0&&score5==0){
		if(new_score>score1){
			score2 = score1;
			score1 = new_score;
			strcpy(name2,name1);
			strcpy(name1,input_name);
		}else {
			score2 = new_score;
			strcpy(name2,input_name);
		}
	}else if(score1!=0&&score2!=0&&score3==0&&score4==0&&score5==0){
		if(new_score>score1){
			score3 = score2;
			score2 = score1;
			score1 = new_score;
			strcpy(name3,name2);
			strcpy(name2,name1);
			strcpy(name1,input_name);
		}else if(new_score<=score1&&new_score>score2){
			score3 = score2;
			score2 = new_score;
			strcpy(name3,name2);
			strcpy(name2,input_name);
		}else if(new_score<=score2){
			score3 = new_score;
			strcpy(name3,input_name);
		}
	}else if(score1!=0&&score2!=0&&score3!=0&&score4==0&&score5==0){
		if(new_score > score1){
			score4 = score3;
			score3 = score2;
			score2 = score1;
			score1 = new_score;
			strcpy(name4,name3);
			strcpy(name3,name2);
			strcpy(name2,name1);
			strcpy(name1,input_name);
		}else if(new_score <= score1&&new_score>score2){
			score4 = score3;
			score3 = score2;
			score2 = new_score;
			strcpy(name4,name3);
			strcpy(name3,name2);
			strcpy(name2,input_name);
		}else if(new_score <= score2&&new_score>score3){
			score4 = score3;
			score3 = new_score;
			strcpy(name4,name3);
			strcpy(name3,input_name);
		}else if(new_score <= score3){
			score4 = new_score;
			strcpy(name4,input_name);
		}
		
	}else if(score1!=0&&score2!=0&&score3!=0&&score4!=0&&score5==0){
		if(new_score > score1){
			score5 = score4;
			score4 = score3;
			score3 = score2;
			score2 = score1;
			score1 = new_score;
			strcpy(name5,name4);
			strcpy(name4,name3);
			strcpy(name3,name2);
			strcpy(name2,name1);
			strcpy(name1,input_name);
		}else if(new_score <= score1&&new_score>score2){
			score5 = score4;
			score4 = score3;
			score3 = score2;
			score2 = new_score;
			strcpy(name5,name4);
			strcpy(name4,name3);
			strcpy(name3,name2);
			strcpy(name2,input_name);
		}else if(new_score <= score2&&new_score>score3){
			score5 = score4;
			score4 = score3;
			score3 = new_score;
			strcpy(name5,name4);
			strcpy(name4,name3);
			strcpy(name3,input_name);
		}else if(new_score <= score3&&new_score>score4){
			score5 = score4;
			score4 = new_score;
			strcpy(name5,name4);
			strcpy(name4,input_name);
		}else if(new_score <= score4){
			score5 = new_score;
			strcpy(name5,input_name);
		}
	}
	else if(score1!=0&&score2!=0&&score3!=0&&score4!=0&&score5!=0){
		if(new_score > score1){
			score5 = score4;
			score4 = score3;
			score3 = score2;
			score2 = score1;
			score1 = new_score;
			strcpy(name5,name4);
			strcpy(name4,name3);
			strcpy(name3,name2);
			strcpy(name2,name1);
			strcpy(name1,input_name);
		}else if(new_score <= score1&&new_score>score2){
			score5 = score4;
			score4 = score3;
			score3 = score2;
			score2 = new_score;
			strcpy(name5,name4);
			strcpy(name4,name3);
			strcpy(name3,name2);
			strcpy(name2,input_name);
		}else if(new_score <= score2&&new_score>score3){
			score5 = score4;
			score4 = score3;
			score3 = new_score;
			strcpy(name5,name4);
			strcpy(name4,name3);
			strcpy(name3,input_name);
		}else if(new_score <= score3&&new_score>score4){
			score5 = score4;
			score4 = new_score;
			strcpy(name5,name4);
			strcpy(name4,input_name);
		}else if(new_score <= score4&&new_score>score5){
			score5 = new_score;
			strcpy(name5,input_name);
		}
	}
	mygame_save_grade();
	close_mygame_scrna();
}
void mygame_input_a(void){
	gui_single_line_input_box_insert_character(&b,'a');
	gui_show_single_line_input_box(&b);
	strcat(input_name, "a");
	gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}
void mygame_input_b(void){
	gui_single_line_input_box_insert_character(&b,'b');
	gui_show_single_line_input_box(&b);
	strcat(input_name, "b");
	gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}
void mygame_input_c(void){
	gui_single_line_input_box_insert_character(&b,'c');
	gui_show_single_line_input_box(&b);
	strcat(input_name, "c");
	gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}
void mygame_input_d(void){
	gui_single_line_input_box_insert_character(&b,'d');
	gui_show_single_line_input_box(&b);
	strcat(input_name, "d");
	gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}
void mygame_input_e(void){
	gui_single_line_input_box_insert_character(&b,'e');
	gui_show_single_line_input_box(&b);
	strcat(input_name, "e");
	gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}
void mygame_input_f(void){
	gui_single_line_input_box_insert_character(&b,'f');
	gui_show_single_line_input_box(&b);
	strcat(input_name, "f");
	gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}
void mygame_input_g(void){
	gui_single_line_input_box_insert_character(&b,'g');
	gui_show_single_line_input_box(&b);
	strcat(input_name, "g");
	gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}
void mygame_input_h(void){
	gui_single_line_input_box_insert_character(&b,'h');
	gui_show_single_line_input_box(&b);
	strcat(input_name, "h");
	gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}
void mygame_input_i(void){
	gui_single_line_input_box_insert_character(&b,'i');
	gui_show_single_line_input_box(&b);
	strcat(input_name, "i");
	gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}
void mygame_best_grade(void){

	int i,j;
	int new_score = 0;
	CHAR score_x1[20] = {0};
	CHAR score_x2[20] = {0};
	CHAR score_x3[20] = {0};
	CHAR score_x4[20] = {0};
	CHAR score_x5[20] = {0};
	U16 temp1[40];
	U16 temp2[40];
	U16 temp3[40];
	U16 temp4[40];
	U16 temp5[40];
	U16 name_temp1[40];
	U16 name_temp2[40];
	U16 name_temp3[40];
	U16 name_temp4[40];
	U16 name_temp5[40];
	UI_filled_area filler={0};

	filler.c = UI_COLOR_GREY;
	gui_draw_filled_area((UI_device_width-1)/2-50,grade_y1,grade_x1+170,grade_y5+20,&filler);
	gui_move_text_cursor((UI_device_width-1)/2-30,grade_y1);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_set_font(&MMI_small_font);
	mmi_asc_to_ucs2((CHAR *) name_temp1, (CHAR *) name1);
	gui_print_text(name_temp1);
	gui_move_text_cursor((UI_device_width-1)/2+30,grade_y1);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_set_font(&MMI_small_font);
	sprintf(score_x1,"%d",score1);
	mmi_asc_to_ucs2((CHAR *) temp1, (CHAR *) score_x1);
	gui_print_text(temp1);

	gui_move_text_cursor((UI_device_width-1)/2-30,grade_y2);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_set_font(&MMI_small_font);
	mmi_asc_to_ucs2((CHAR *) name_temp2, (CHAR *) name2);
	gui_print_text(name_temp2);
	gui_move_text_cursor((UI_device_width-1)/2+30,grade_y2);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_set_font(&MMI_small_font);
	sprintf(score_x2,"%d",score2);
	mmi_asc_to_ucs2((CHAR *) temp2, (CHAR *) score_x2);
	gui_print_text(temp2);

	gui_move_text_cursor((UI_device_width-1)/2-30,grade_y3);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_set_font(&MMI_small_font);
	mmi_asc_to_ucs2((CHAR *) name_temp3, (CHAR *) name3);
	gui_print_text(name_temp3);
	gui_move_text_cursor((UI_device_width-1)/2+30,grade_y3);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_set_font(&MMI_small_font);
	sprintf(score_x3,"%d",score3);
	mmi_asc_to_ucs2((CHAR *) temp3, (CHAR *) score_x3);
	gui_print_text(temp3);

	gui_move_text_cursor((UI_device_width-1)/2-30,grade_y4);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_set_font(&MMI_small_font);
	mmi_asc_to_ucs2((CHAR *) name_temp4, (CHAR *) name4);
	gui_print_text(name_temp4);
	gui_move_text_cursor((UI_device_width-1)/2+30,grade_y4);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_set_font(&MMI_small_font);
	sprintf(score_x4,"%d",score4);
	mmi_asc_to_ucs2((CHAR *) temp4, (CHAR *) score_x4);
	gui_print_text(temp4);

	gui_move_text_cursor((UI_device_width-1)/2-30,grade_y5);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_set_font(&MMI_small_font);
	mmi_asc_to_ucs2((CHAR *) name_temp5, (CHAR *) name5);
	gui_print_text(name_temp5);
	gui_move_text_cursor((UI_device_width-1)/2+30,grade_y5);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_set_font(&MMI_small_font);
	sprintf(score_x5,"%d",score5);
	mmi_asc_to_ucs2((CHAR *) temp5, (CHAR *) score_x5);
	gui_print_text(temp5);

	gui_set_font(&MMI_small_font);
	gui_set_text_color(UI_COLOR_BLACK );
	gui_move_text_cursor(UI_device_width-50,UI_device_height - 25);
    gui_print_text((UI_string_type)GetString(MYGAME_EXIT_TEXT));

	gui_set_font(&MMI_small_font);
	gui_set_text_color(UI_COLOR_BLACK );
	gui_move_text_cursor(10,UI_device_height - 25);
   	gui_print_text((UI_string_type)GetString(MYGAME_RESET_TEXT));
	
	add_grade = FALSE;
	gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}

void mygame_status_save(void)
{
    S16 error;

	mygame_struct.save_is_new_game = mygame_is_continue;
	
	WriteRecord(
		NVRAM_EF_MYGAME_IS_NEW_LID,
        1,
        &mygame_struct.save_is_new_game,
        8,
        &error);
}
void mygame_status_read(void)
{
	S16 error;
	
	ReadRecord(
        NVRAM_EF_MYGAME_IS_NEW_LID,
        1,
        &mygame_struct.save_is_new_game,
        8,
        &error);
	mygame_is_continue = mygame_struct.save_is_new_game;
}
void mygame_save_body(void)
{
	S16 error;
	int i,j;
	for(i=0; i<a_row; i++){
		for(j=0; j<a_column; j++){
			mygame_struct.save_a[i][j] = a[i][j];
		}
	}
	WriteRecord(
		NVRAM_EF_MYGAME_BODY_LID,
        1,
        &mygame_struct.save_a,
        64,
        &error);
}
void mygame_read_body(void)
{
	S16 error;
	int i,j;
	
	ReadRecord(
		NVRAM_EF_MYGAME_BODY_LID,
        1,
        &mygame_struct.save_a,
        64,
        &error);
	
	for(i=0; i<a_row; i++){
		for(j=0; j<a_column; j++){
			a[i][j] = mygame_struct.save_a[i][j];
		}
	}
}
void mygame_save_score(void)
{
	S16 error;
	mygame_struct.save_score = total_score;
	WriteRecord(
		NVRAM_EF_MYGAME_SCORE_LID,
        1,
        &mygame_struct.save_score,
        32,
        &error);
}
void mygame_read_score(void)
{
	S16 error;
	
	ReadRecord(
		NVRAM_EF_MYGAME_SCORE_LID,
        1,
        &mygame_struct.save_score,
        32,
        &error);
	total_score = mygame_struct.save_score;
}

void mygame_save_time(void)
{
	S16 error;
	mygame_struct.save_time = t;
	WriteRecord(
		NVRAM_EF_MYGAME_TIME_LID,
        1,
        &mygame_struct.save_time,
        32,
        &error);
}
void mygame_read_time(void)
{
	S16 error;
	
	ReadRecord(
		NVRAM_EF_MYGAME_TIME_LID,
        1,
        &mygame_struct.save_time,
        32,
        &error);
	t = mygame_struct.save_time;
}
void mygame_save_grade(void)
{
	S16 error;
	mygame_grade_struct.grade_score1 = score1;
	mygame_grade_struct.grade_score2 = score2;
	mygame_grade_struct.grade_score3 = score3;
	mygame_grade_struct.grade_score4 = score4;
	mygame_grade_struct.grade_score5 = score5;
	strcpy(mygame_grade_name_struct.grade_name1, name1);
	strcpy(mygame_grade_name_struct.grade_name2, name2);
	strcpy(mygame_grade_name_struct.grade_name3, name3);
	strcpy(mygame_grade_name_struct.grade_name4, name4);
	strcpy(mygame_grade_name_struct.grade_name5, name5);
	WriteRecord(
		NVRAM_EF_MYGAME_SCORE1_LID,
        1,
        &mygame_grade_struct.grade_score1,
        32,
        &error);
	WriteRecord(
		NVRAM_EF_MYGAME_SCORE2_LID,
        1,
        &mygame_grade_struct.grade_score2,
        32,
        &error);
	WriteRecord(
		NVRAM_EF_MYGAME_SCORE3_LID,
        1,
        &mygame_grade_struct.grade_score3,
        32,
        &error);
	WriteRecord(
		NVRAM_EF_MYGAME_SCORE4_LID,
        1,
        &mygame_grade_struct.grade_score4,
        32,
        &error);
	WriteRecord(
		NVRAM_EF_MYGAME_SCORE5_LID,
        1,
        &mygame_grade_struct.grade_score5,
        32,
        &error);
	WriteRecord(
		NVRAM_EF_MYGAME_NAME1_LID,
        1,
        &mygame_grade_name_struct.grade_name1,
        20,
        &error);
	WriteRecord(
		NVRAM_EF_MYGAME_NAME2_LID,
        1,
        &mygame_grade_name_struct.grade_name2,
        20,
        &error);
	WriteRecord(
		NVRAM_EF_MYGAME_NAME_THREE_LID,
        1,
        &mygame_grade_name_struct.grade_name3,
        20,
        &error);
	WriteRecord(
		NVRAM_EF_MYGAME_NAME_FOUR_LID,
        1,
        &mygame_grade_name_struct.grade_name4,
        20,
        &error);
	WriteRecord(
		NVRAM_EF_MYGAME_NAME_FIVE_LID,
        1,
        &mygame_grade_name_struct.grade_name5,
        20,
        &error);
}

void mygame_read_grade(void)
{
	S16 error;
	
	ReadRecord(
		NVRAM_EF_MYGAME_SCORE1_LID,
        1,
        &mygame_grade_struct.grade_score1,
        32,
        &error);
	ReadRecord(
		NVRAM_EF_MYGAME_SCORE2_LID,
        1,
        &mygame_grade_struct.grade_score2,
        32,
        &error);
	ReadRecord(
		NVRAM_EF_MYGAME_SCORE3_LID,
        1,
        &mygame_grade_struct.grade_score3,
        32,
        &error);
	ReadRecord(
		NVRAM_EF_MYGAME_SCORE4_LID,
        1,
        &mygame_grade_struct.grade_score4,
        32,
        &error);
	ReadRecord(
		NVRAM_EF_MYGAME_SCORE5_LID,
        1,
        &mygame_grade_struct.grade_score5,
        32,
        &error);
	ReadRecord(
		NVRAM_EF_MYGAME_NAME1_LID,
        1,
        &mygame_grade_name_struct.grade_name1,
        20,
        &error);
	ReadRecord(
		NVRAM_EF_MYGAME_NAME2_LID,
        1,
        &mygame_grade_name_struct.grade_name2,
        20,
        &error);
	ReadRecord(
		NVRAM_EF_MYGAME_NAME_THREE_LID,
        1,
        &mygame_grade_name_struct.grade_name3,
        20,
        &error);
	ReadRecord(
		NVRAM_EF_MYGAME_NAME_FOUR_LID,
        1,
        &mygame_grade_name_struct.grade_name4,
        20,
        &error);
	ReadRecord(
		NVRAM_EF_MYGAME_NAME_FIVE_LID,
        1,
        &mygame_grade_name_struct.grade_name5,
        20,
        &error);
	
	score1 = mygame_grade_struct.grade_score1;
	score2 = mygame_grade_struct.grade_score2;
	score3 = mygame_grade_struct.grade_score3;
	score4 = mygame_grade_struct.grade_score4;
	score5 = mygame_grade_struct.grade_score5;
	strcpy(name1,mygame_grade_name_struct.grade_name1);
	strcpy(name2,mygame_grade_name_struct.grade_name2);
	strcpy(name3,mygame_grade_name_struct.grade_name3);
	strcpy(name4,mygame_grade_name_struct.grade_name4);
	strcpy(name5,mygame_grade_name_struct.grade_name5);
}

void mygame_show_pic(void){
	int i,n;

	for(i=0;i<a_row;i++){
		for(n=0;n<a_column;n++){
			int x2=0,y2=0;
					if(i==0){
						y2 = mygame_y;
					}else if(i== 1){
						y2 = mygame_y + disparity;
					}else if(i == 2){
						y2 = mygame_y + 2*disparity;
					}else if(i == 3){
						y2 = mygame_y + 3*disparity;
					}
					if(n == 0){
						x2 = mygame_x;
					}else if(n == 1){
						x2 = mygame_x + disparity;
					}else if(n == 2){
						x2 = mygame_x + 2*disparity;
					}else if(n == 3){
						x2 = mygame_x + 3*disparity;
					}
					if(a[i][n] == num0){
						gdi_image_draw_id(x2, y2, IMG_MYGAME_0);
					}
					if(a[i][n] == num2){
						gdi_image_draw_id(x2, y2, IMG_MYGAME_2);
					}
					if(a[i][n] == num4){
						gdi_image_draw_id(x2, y2, IMG_MYGAME_4);
					}
					if(a[i][n] == num8){
						gdi_image_draw_id(x2, y2, IMG_MYGAME_8);
					}
					if(a[i][n] == num16){
						gdi_image_draw_id(x2, y2, IMG_MYGAME_16);
					}
					if(a[i][n] == num32){
						gdi_image_draw_id(x2, y2, IMG_MYGAME_32);
					}
					if(a[i][n] == num64){
						gdi_image_draw_id(x2, y2, IMG_MYGAME_64);
					}
					if(a[i][n] == num128){
						gdi_image_draw_id(x2, y2, IMG_MYGAME_128);
					}
					if(a[i][n] == num256){
						gdi_image_draw_id(x2, y2, IMG_MYGAME_256);
					}
					if(a[i][n] == num512){
						gdi_image_draw_id(x2, y2, IMG_MYGAME_512);
					}
					if(a[i][n] == num1024){
						gdi_image_draw_id(x2, y2, IMG_MYGAME_1024);
					}
					if(a[i][n] == num2048){
						gdi_image_draw_id(x2, y2, IMG_MYGAME_2048);
					}
		}
	}
}
void mygame_show_score(void)
{
	CHAR score[20] = {0};
	U16 temp[40];
	UI_filled_area filler={0};
	
	filler.c = UI_COLOR_GREY;
	gui_draw_filled_area(score_x+70,score_y,score_x+125,score_y+20,&filler);
	gui_move_text_cursor(score_x+70,score_y);
	gui_set_text_color(UI_COLOR_BLACK);
	gui_set_font(&MMI_large_font);
	sprintf(score,"%d",total_score);
	mmi_asc_to_ucs2((CHAR *) temp, (CHAR *) score);
	gui_print_text(temp);
	gui_BLT_double_buffer(0,0,UI_device_width-1,UI_device_height-1);
}




