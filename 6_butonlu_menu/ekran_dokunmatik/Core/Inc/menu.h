/*
#ifndef __MENU_H
#define __MENU_H

#define MENU_ITEM_COUNT 3

const char* menu_items[MENU_ITEM_COUNT] = {
    "SICAKLIK GONDER",
    "EKRANI TEMIZLE",
    "CIHAZ BILGISI"
};

#endif*/



/**************************************************/

#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <stdbool.h>
#include "icons.h"

#define THEME_PAGE_INDEX 0


typedef struct {
    const tImage* icon_on;
    const tImage* icon_off;
    const char* room_name;
    bool state;
} MenuItem;

extern uint8_t currentPage;
extern uint32_t lastButtonPressTime;
extern uint32_t debounceDelay;

extern bool startupDone;
extern uint32_t startupTime;

extern uint8_t currentTheme;


#define MENU_ITEM_COUNT 7
extern MenuItem menu_items[MENU_ITEM_COUNT];
void drawMenu(uint8_t startIndex);
void animateMenuTransition(uint8_t oldIndex, uint8_t newIndex, bool directionRight);
void show_startup_screen(void);
void drawThemePage(void);
void theme_page_tuslari(void);




#endif


