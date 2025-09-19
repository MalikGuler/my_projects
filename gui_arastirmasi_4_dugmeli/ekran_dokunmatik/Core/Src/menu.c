#include <stdio.h>
#include <string.h>
#include "stm32f0xx_hal.h"
#include "ST7735.h"
#include "GFX_FUNCTIONS.h"
#include "menu.h"
#include "ugui.h"




extern I2C_HandleTypeDef hi2c1;



UG_GUI gui;

Room rooms[MAX_ROOMS];
uint8_t room_count = 0;

Page currentPage = PAGE_MAIN;
uint8_t selectedIndex = 0;        // Menüde seçili satır
uint8_t currentRoomIndex = 0;     // Odalar menüsündeysek bu odadır
uint8_t settingsRoomIndex = 0;    // Ayarlar menüsündeysek hangi oda seçili
uint8_t settingsRoomDeviceIndex = 0; // Ayarlar -> Oda detay cihaz listesinde seçili cihaz

uint8_t add_room_step = 0;
uint8_t add_room_lamp = 1;
uint8_t add_room_curtain = 0;
uint8_t add_room_ac = 0;


const char* deviceNames[DEVICE_COUNT] = {
    "Lamba",
    "Perde",
    "Klima"
};

// Buton tanımları
#define BUTTON_UP     1
#define BUTTON_DOWN   2
#define BUTTON_SELECT 3
#define BUTTON_BACK   4



void Menu_Init(void)
{
    UG_Init(&gui, UG_draw_pixel, 128, 64);
    UG_FontSelect(&FONT_6X8);
    UG_SetBackcolor(C_BLACK);
    UG_SetForecolor(C_WHITE);
    UG_FillScreen(C_BLACK);

    room_count = 0;

    currentPage = PAGE_MAIN;
    selectedIndex = 0;
    currentRoomIndex = 0;
    settingsRoomIndex = 0;
    settingsRoomDeviceIndex = 0;

    add_room_step = 0;
    add_room_lamp = 1;
    add_room_curtain = 0;
    add_room_ac = 0;

    Menu_Draw(currentPage);
}


void Menu_Draw(Page currentPage)
{
    UG_FillScreen(C_BLACK);

    switch(currentPage)
    {
        case PAGE_MAIN:
            drawMainMenu();
            break;
        case PAGE_ROOM_DETAIL:
            drawRoomDetail();
            break;
        case PAGE_SETTINGS_ROOM_LIST:
            drawSettingsRoomList();
            break;
        case PAGE_ADD_ROOM:
            Menu_AddRoom();
            break;
    }
    UG_Update();
}

const tImage* getDeviceIcon(DeviceType d, uint8_t state)
{
    switch(d)
    {
    	case DEVICE_LAMP:    return state ? &lightbulb_open : &lightbulb_closed;
    	case DEVICE_CURTAIN: return state ? &perde_acik : &perde_kapali;
       // case DEVICE_AC: return state ? ac_on : ac_off;
        default: return NULL;
    }
}

void drawMainMenu(void)
{
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_BLACK);
    UG_FontSelect(&FONT_6X8);

    const char* items[] = {"Odalar", "Ayarlar"};
    for(uint8_t i = 0; i < 2; i++)
    {
        int y = 10 + i * 14;
        if(selectedIndex == i)
        {
            UG_FillRoundFrame(2, y-2, 126, y+12, 3, C_WHITE);
            UG_SetForecolor(C_BLACK);
        }
        else
        {
            UG_SetForecolor(C_WHITE);
        }
        UG_PutString(10, y, items[i]);
    }
}

void drawRoomDetail(void)
{
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_BLACK);
    UG_FontSelect(&FONT_6X8);

    Room* room = &rooms[currentRoomIndex];
    UG_PutString(4, 4, room->name);

    for(uint8_t i = 0; i < room->device_count; i++)
    {
        int y = 20 + i * 14;
        if(selectedIndex == i)
        {
            UG_FillRoundFrame(2, y-2, 126, y+12, 3, C_WHITE);
            UG_SetForecolor(C_BLACK);
        }
        else
        {
            UG_SetForecolor(C_WHITE);
        }
        UG_PutString(24, y, deviceNames[room->devices[i].type]);

        const tImage* icon = getDeviceIcon(room->devices[i].type, room->devices[i].state);
        UG_BMP bmp;
        if(icon){
            bmp = convertToUGBMP(icon);
            UG_DrawBMP(4, y, &bmp);
        }
    }
}

void drawSettingsRoomList(void)
{
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_BLACK);
    UG_FontSelect(&FONT_6X8);

    for(uint8_t i = 0; i < room_count; i++)
    {
        int y = 10 + i * 14;
        if(selectedIndex == i)
        {
            UG_FillRoundFrame(2, y-2, 126, y+12, 3, C_WHITE);
            UG_SetForecolor(C_BLACK);
        }
        else
        {
            UG_SetForecolor(C_WHITE);
        }
        UG_PutString(10, y, rooms[i].name);
    }

    int addY = 10 + room_count * 14;
    if(selectedIndex == room_count)
    {
        UG_FillRoundFrame(2, addY-2, 126, addY+12, 3, C_WHITE);
        UG_SetForecolor(C_BLACK);
    }
    else
    {
        UG_SetForecolor(C_WHITE);
    }
    UG_PutString(10, addY, "Oda Ekle");
}


void drawSettingsRoomDetail(void)
{
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_BLACK);
    UG_FontSelect(&FONT_6X8);

    Room* room = &rooms[settingsRoomIndex];
    UG_PutString(4, 4, room->name);

    for(uint8_t i = 0; i < room->device_count; i++)
    {
        int y = 20 + i * 14;
        if(settingsRoomDeviceIndex == i)
        {
            UG_FillRoundFrame(2, y-2, 126, y+12, 3, C_WHITE);
            UG_SetForecolor(C_BLACK);
        }
        else
        {
            UG_SetForecolor(C_WHITE);
        }
        UG_PutString(24, y, deviceNames[room->devices[i].type]);

        const tImage* icon = getDeviceIcon(room->devices[i].type, room->devices[i].state);
        UG_BMP bmp;
        if(icon){
            bmp = convertToUGBMP(icon);
            UG_DrawBMP(4, y, &bmp);
        }
    }

    // "Odayı Sil" butonu - en altta
    int deleteY = 20 + room->device_count * 14;
    if(settingsRoomDeviceIndex == room->device_count)
    {
        UG_FillRoundFrame(2, deleteY-2, 126, deleteY+12, 3, C_WHITE);
        UG_SetForecolor(C_BLACK);
    }
    else
    {
        UG_SetForecolor(C_WHITE);
    }
    UG_PutString(10, deleteY, "Odayı Sil");
}

void Menu_AddRoom(void)
{
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_BLACK);
    UG_FontSelect(&FONT_6X8);

    UG_PutString(10, 4, "Yeni Oda Ekle");

    int y = 20;
    if (add_room_step == 0) {
        UG_PutString(10, y, "Lamba:");
        UG_PutString(60, y, add_room_lamp ? "Evet" : "Hayir");
    } else if (add_room_step == 1) {
        UG_PutString(10, y, "Perde:");
        UG_PutString(60, y, add_room_curtain ? "Evet" : "Hayir");
    } else if (add_room_step == 2) {
        UG_PutString(10, y, "Klima:");
        UG_PutString(60, y, add_room_ac ? "Evet" : "Hayir");
    } else if (add_room_step == 3) {
        UG_PutString(10, y, "Kaydetmek istiyor musun?");
        if (selectedIndex == 0) {
            UG_FillRoundFrame(10, y+14, 60, y+28, 3, C_WHITE);
            UG_SetForecolor(C_BLACK);
        } else {
            UG_SetForecolor(C_WHITE);
        }
        UG_PutString(20, y+16, "EVET");

        if (selectedIndex == 1) {
            UG_FillRoundFrame(70, y+14, 120, y+28, 3, C_WHITE);
            UG_SetForecolor(C_BLACK);
        } else {
            UG_SetForecolor(C_WHITE);
        }
        UG_PutString(80, y+16, "HAYIR");
    }
}

void drawConfirmDeleteRoom(void)
{
    UG_SetForecolor(C_WHITE);
    UG_SetBackcolor(C_BLACK);
    UG_FontSelect(&FONT_6X8);

    UG_PutString(10, 20, "Odayi silmek istiyor musun?");

    if(selectedIndex == 0)
    {
        UG_FillRoundFrame(20, 40, 70, 54, 3, C_WHITE);
        UG_SetForecolor(C_BLACK);
        UG_PutString(35, 42, "EVET");
    }
    else
    {
        UG_SetForecolor(C_WHITE);
        UG_PutString(35, 42, "EVET");
    }

    if(selectedIndex == 1)
    {
        UG_FillRoundFrame(80, 40, 120, 54, 3, C_WHITE);
        UG_SetForecolor(C_BLACK);
        UG_PutString(90, 42, "HAYIR");
    }
    else
    {
        UG_SetForecolor(C_WHITE);
        UG_PutString(90, 42, "HAYIR");
    }
}

// Odaları silme fonksiyonu
void Menu_RemoveRoom(uint8_t index)
{
    if(index >= room_count) return;
    for(uint8_t i = index; i < room_count - 1; i++)
    {
        rooms[i] = rooms[i + 1];
    }
    room_count--;
    if(settingsRoomIndex >= room_count && room_count > 0)
        settingsRoomIndex = room_count - 1;
}

// Ana input handler fonksiyonu

void Menu_HandleInput(Button button)
{
    switch (currentPage)
    {
        case PAGE_MAIN:
            if (button == BUTTON_DOWN && selectedIndex < 1)
                selectedIndex++;
            else if (button == BUTTON_UP && selectedIndex > 0)
                selectedIndex--;
            else if (button == BUTTON_SELECT) {
                if (selectedIndex == 0) {
                    currentPage = PAGE_ROOM_DETAIL;
                    selectedIndex = 0;
                } else if (selectedIndex == 1) {
                    currentPage = PAGE_SETTINGS_ROOM_LIST;
                    selectedIndex = 0;
                }
            }
            break;

        case PAGE_ROOM_DETAIL: {
            Room* room = &rooms[currentRoomIndex];
            uint8_t maxIndex = room->device_count;

            if (button == BUTTON_DOWN && selectedIndex < maxIndex)
                selectedIndex++;
            else if (button == BUTTON_UP && selectedIndex > 0)
                selectedIndex--;
            else if (button == BUTTON_SELECT) {
                if (selectedIndex < room->device_count) {
                    // Toggle cihaz durumu
                    room->devices[selectedIndex].state ^= 1;
                } else if (selectedIndex == room->device_count) {
                    // Oda sil
                    Menu_RemoveRoom(currentRoomIndex);
                    selectedIndex = 0;
                    currentPage = PAGE_SETTINGS_ROOM_LIST;
                }
            }
            else if (button == BUTTON_BACK) {
                selectedIndex = 0;
                currentPage = PAGE_ROOM_DETAIL;
            }
            break;
        }

        case PAGE_SETTINGS_ROOM_LIST:
            if (button == BUTTON_DOWN && selectedIndex < room_count)
                selectedIndex++;
            else if (button == BUTTON_UP && selectedIndex > 0)
                selectedIndex--;
            else if (button == BUTTON_SELECT) {
                if (selectedIndex == room_count) {
                    currentPage = PAGE_ADD_ROOM;
                    add_room_step = 0;
                    selectedIndex = 0;
                } else {
                    currentRoomIndex = selectedIndex;
                    selectedIndex = 0;
                    currentPage = PAGE_ROOM_DETAIL;
                }
            }
            else if (button == BUTTON_BACK) {
                selectedIndex = 0;
                currentPage = PAGE_MAIN;
            }
            break;

        case PAGE_ADD_ROOM:
            if (add_room_step <= 2) {
                if (button == BUTTON_SELECT) {
                    if (add_room_step == 0)
                        add_room_lamp = !add_room_lamp;
                    else if (add_room_step == 1)
                        add_room_curtain = !add_room_curtain;
                    else if (add_room_step == 2)
                        add_room_ac = !add_room_ac;
                }
                else if (button == BUTTON_DOWN || button == BUTTON_UP) {
                    add_room_step++;
                    if (add_room_step > 3) add_room_step = 3;
                }
                else if (button == BUTTON_BACK) {
                    add_room_step = 0;
                    currentPage = PAGE_SETTINGS_ROOM_LIST;
                    selectedIndex = 0;
                }
            } else if (add_room_step == 3) {
                if (button == BUTTON_LEFT || button == BUTTON_UP)
                    selectedIndex = 0;
                else if (button == BUTTON_RIGHT || button == BUTTON_DOWN)
                    selectedIndex = 1;
                else if (button == BUTTON_BACK) {
                    add_room_step = 0;
                    currentPage = PAGE_SETTINGS_ROOM_LIST;
                    selectedIndex = 0;
                }
                else if (button == BUTTON_SELECT) {
                    if (selectedIndex == 0) {
                        if (room_count < MAX_ROOMS) {
                            char name[16];
                            sprintf(name, "YeniOda%d", room_count + 1);

                            Room* r = &rooms[room_count++];
                            strncpy(r->name, name, sizeof(r->name));
                            r->device_count = 0;
                            if (add_room_lamp)   r->devices[r->device_count++] = (Device){DEVICE_LAMP, 0};
                            if (add_room_curtain)r->devices[r->device_count++] = (Device){DEVICE_CURTAIN, 0};
                            if (add_room_ac)     r->devices[r->device_count++] = (Device){DEVICE_AC, 0};
                        }
                    }
                    // Sıfırla ve geri dön
                    add_room_step = 0;
                    add_room_lamp = 1;
                    add_room_curtain = 0;
                    add_room_ac = 0;
                    currentPage = PAGE_SETTINGS_ROOM_LIST;
                    selectedIndex = 0;
                }
            }
            break;
    }

    Menu_Draw(currentPage);
}


void UG_draw_pixel(UG_S16 x, UG_S16 y, UG_COLOR color)
{
    ST7735_DrawPixel((uint16_t)x, (uint16_t)y, (uint16_t)color);
}

UG_BMP convertToUGBMP(const tImage* img)
{
    UG_BMP bmp;
    bmp.p      = (void*)img->data;
    bmp.width  = img->width;
    bmp.height = img->height;
    bmp.bpp    = BMP_BPP_16;
    bmp.colors = BMP_RGB565;
    return bmp;
}


void show_startup_screen(void)
{
    FillScreen(WHITE);

    const char* text1 = "ino";
    const char* text2 = "hom";

    uint16_t x_start = 30;
    uint16_t y = 28;

    // "ino" (kırmızı)
    ST7735_WriteString(x_start, y, text1, Font_11x18, RED, WHITE);

    // "hom" (siyah)
    ST7735_WriteString(x_start + 11 * strlen(text1), y, text2, Font_11x18, BLACK, WHITE);
    HAL_Delay(3000);

}










