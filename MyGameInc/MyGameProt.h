#ifndef _MYGAMEPROT_H   
#define _MYGAMEPROT_H
#include "MyGameGprot.h"

typedef struct
{
    kal_uint32 save_score;
    kal_bool save_is_new_game;
    kal_uint32 save_time;
    kal_uint32 save_a[4][4];
}MYGAME_STRUCT;

typedef struct
{
    kal_uint32 grade_score1;
	kal_uint32 grade_score2;
	kal_uint32 grade_score3;
	kal_uint32 grade_score4;
	kal_uint32 grade_score5;
}MYGAME_GRADE_STRUCT;

typedef struct
{
    kal_uint8 grade_name1[20];
	kal_uint8 grade_name2[20];
	kal_uint8 grade_name3[20];
	kal_uint8 grade_name4[20];
	kal_uint8 grade_name5[20];
}MYGAME_GRADE_NAME_STRUCT;



extern void mmi_mygame_entry(void);
extern void enter_mygame_screen(void);
extern void mygame_cui_menu_enter(void);
static mmi_ret mygame_proc(mmi_event_struct *evt);
extern void highlight_mygame_new(void);
extern void close_mygame_scrna(void);
extern void mygame_add_num(void);
extern void mygame_show_bg(void);
extern void mygame_moveLeft(void);
extern void mygame_score(void);
extern void mygame_time(void);
extern void mygame_moveRight(void);
extern void mygame_moveUp(void);
extern void mygame_moveDown(void);
extern void mygame_init_a(void);
extern void mygame_check(void);
extern void mmi_mygame_grade_entry(void);
extern void enter_mygame_grade_screen(void);
extern void close_mygame_grade_scrna(void);
extern void mygame_best_grade(void);
extern void mygame_input_a(void);
extern void mygame_input_b(void);
extern void mygame_input_c(void);
extern void mygame_input_d(void);
extern void mygame_input_e(void);
extern void mygame_input_f(void);
extern void mygame_input_g(void);
extern void mygame_input_h(void);
extern void mygame_input_i(void);
extern void mygame_input_save(void);
extern void mmi_mygame_continue_entry(void);
extern void enter_mygame_continue_screen(void);
extern void mygame_status_save(void);
extern void mygame_status_read(void);
extern void mygame_save_body(void);
extern void mygame_read_body(void);
extern void mygame_save_time(void);
extern void mygame_read_time(void);
extern void mygame_show_pic(void);
extern void mygame_save_grade(void);
extern void mygame_read_grade(void);
extern void mygame_save_score(void);
extern void mygame_read_score(void);
extern int mygame_null_count();
extern void mygame_show_score(void);
extern int mygame_over();
extern void mygame_init_grade(void);


	
#endif  /*_MYGAMEPROT_H*/



