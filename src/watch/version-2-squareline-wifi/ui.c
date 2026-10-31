// Adapted from a SquareLine Studio 1.6.3 export.
// The original LVGL 8.3.11 output was manually updated for LVGL 9.3.0.
// Project name: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////

// EVENTS
lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_display_t * dispp = lv_display_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_display_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_screen_load(ui_Screen1);
}

void ui_destroy(void)
{
    ui_Screen1_screen_destroy();
}
