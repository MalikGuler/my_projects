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
#include "ugui.h"
#include "icons.h"

#define MAX_ROOMS 10
#define DEVICE_COUNT 3
#define MAX_NAME_LEN 16

typedef enum {
    DEVICE_LAMP,
    DEVICE_CURTAIN,
    DEVICE_AC
} DeviceType;

typedef struct {
    DeviceType type;
    uint8_t state;
} Device;

typedef struct {
    char name[20];
    Device devices[3];
    uint8_t device_count;
} Room;

typedef enum {
    PAGE_MAIN,              // Ana Menü: Odalar / Ayarlar
    PAGE_ROOM_DETAIL,       // Oda detayları
    PAGE_SETTINGS_ROOM_LIST,// Ayarlardaki oda listesi
    PAGE_ADD_ROOM           // Yeni oda ekleme
} Page;

typedef enum {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_SELECT,
    BUTTON_BACK,
    BUTTON_LEFT,
    BUTTON_RIGHT
} Button;


extern UG_GUI gui;

void Menu_Init(void);
void Menu_Draw(Page currentPage);
const tImage* getDeviceIcon(DeviceType d, uint8_t state);
void drawMainMenu(void);
void drawRoomDetail(void);
void drawSettingsRoomList(void);
void drawSettingsRoomDetail(void);
void Menu_AddRoom(void);
void drawConfirmDeleteRoom(void);
void Menu_RemoveRoom(uint8_t index);
void Menu_HandleInput(Button button);



void UG_draw_pixel(UG_S16 x, UG_S16 y, UG_COLOR color);
UG_BMP convertToUGBMP(const tImage* img);
void show_startup_screen(void);



#endif


