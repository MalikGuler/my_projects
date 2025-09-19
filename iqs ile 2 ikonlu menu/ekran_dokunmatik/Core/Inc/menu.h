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

#include "main.h"
#include "icons.h"


// Fonksiyonlar
void drawMenu(uint8_t selectedIndex);
void checkTouch(void);
void SendUART(void);

#endif



