// Adapted from a SquareLine Studio 1.6.3 export.
// The original LVGL 8.3.11 output was manually updated for LVGL 9.3.0.
// Project name: SquareLine_Project

#include "ui.h"

lv_obj_t * ui_Screen1 = NULL;
lv_obj_t * ui_Switch1 = NULL;
lv_obj_t * ui_Label1 = NULL;
lv_obj_t * ui_Label2 = NULL;
lv_obj_t * ui_WIFILIST = NULL;
lv_obj_t * ui_SCANNINGFOUND = NULL;

// build funtions

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Switch1 = lv_switch_create(ui_Screen1);
    lv_obj_set_width(ui_Switch1, 50);
    lv_obj_set_height(ui_Switch1, 25);
    lv_obj_set_x(ui_Switch1, -65);
    lv_obj_set_y(ui_Switch1, -233);
    lv_obj_set_align(ui_Switch1, LV_ALIGN_CENTER);

    ui_Label1 = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label1, -113);
    lv_obj_set_y(ui_Label1, -233);
    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1, "WIFI");

    ui_Label2 = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label2, 3);
    lv_obj_set_y(ui_Label2, -234);
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "DISABLED");

    ui_WIFILIST = lv_obj_create(ui_Screen1);
    lv_obj_remove_style_all(ui_WIFILIST);
    lv_obj_set_width(ui_WIFILIST, 345);
    lv_obj_set_height(ui_WIFILIST, 415);
    lv_obj_set_x(ui_WIFILIST, 0);
    lv_obj_set_y(ui_WIFILIST, 15);
    lv_obj_set_align(ui_WIFILIST, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_WIFILIST, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(ui_WIFILIST, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_scroll_dir(ui_WIFILIST, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(ui_WIFILIST, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_set_flex_flow(ui_WIFILIST, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_WIFILIST,
                          LV_FLEX_ALIGN_START,
                          LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(ui_WIFILIST, 4, 0);
    lv_obj_set_style_pad_row(ui_WIFILIST, 8, 0);

    ui_SCANNINGFOUND = lv_label_create(ui_Screen1);
    lv_obj_set_width(ui_SCANNINGFOUND, 345);
    lv_obj_set_height(ui_SCANNINGFOUND, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_SCANNINGFOUND, 0);
    lv_obj_set_y(ui_SCANNINGFOUND, -206);
    lv_obj_set_align(ui_SCANNINGFOUND, LV_ALIGN_CENTER);
    lv_label_set_long_mode(ui_SCANNINGFOUND, LV_LABEL_LONG_DOT);
    lv_obj_set_style_text_align(ui_SCANNINGFOUND, LV_TEXT_ALIGN_LEFT, 0);
    lv_label_set_text(ui_SCANNINGFOUND, "");

}

void ui_Screen1_screen_destroy(void)
{
    if(ui_Screen1) lv_obj_delete(ui_Screen1);

    // NULL screen variables
    ui_Screen1 = NULL;
    ui_Switch1 = NULL;
    ui_Label1 = NULL;
    ui_Label2 = NULL;
    ui_WIFILIST = NULL;
    ui_SCANNINGFOUND = NULL;

}
