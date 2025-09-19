#include "menu.h"
#include "ST7735.h"
#include "IQS7222A.h"
#include <string.h>
#include <stdio.h>

#define MENU_ITEM_COUNT 2

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;

uint8_t selectedIndex = 0;
uint8_t last_touch = 0;
uint32_t lastTapTime = 0;
uint8_t tapCount = 0;


typedef struct {
    const tImage* icon;
    const char* label;
} MenuItem;

MenuItem menu_items[MENU_ITEM_COUNT] = {
    { &home_icon, "Home" },
    { &light_bulb_icon, "Lamba" }
};

void SendUART(void) {
	  if (selectedIndex < MENU_ITEM_COUNT) {
	        const char* text = menu_items[selectedIndex].label;
	        HAL_UART_Transmit(&huart1, (uint8_t*)text, strlen(text), HAL_MAX_DELAY);
	    }
}


void drawMenu(uint8_t selectedIndex) {
    FillScreen(0x8410);

    uint16_t spacing = 20;
    uint16_t icon_width = menu_items[0].icon->width;
    uint16_t icon_height = menu_items[0].icon->height;

    uint16_t start_x = (128 - (MENU_ITEM_COUNT * icon_width + (MENU_ITEM_COUNT - 1) * spacing)) / 2;
    uint16_t y = (64 - icon_height) / 2;

    for (uint8_t i = 0; i < MENU_ITEM_COUNT; i++) {
        uint16_t x = start_x + i * (icon_width + spacing);

        if (i == selectedIndex) {
        	ST7735_FillRectangle(x - 2, y - 2, icon_width + 4, icon_height + 4, 0xC618);
        }

        ST7735_DrawImage(x, y, icon_width, icon_height, (const uint16_t*)menu_items[i].icon->data);
    }
}

bool IQS7222A_anyChannelTouched(void)
{
    // Kanal ID'lerini sırayla kontrol ediyoruz
    for (iqs7222a_channel_e ch = IQS7222A_CH0; ch <= IQS7222A_HALL; ch++)
    {
        if (IQS7222A_channelTouchState(ch))
            return true;  // Dokunma algılandı
    }

    return false ;  // Hiçbir kanalda dokunma yok
}

void checkTouch(void)
{
    static uint32_t debounceTime = 0;
    uint32_t now = HAL_GetTick();

    // Debounce için 200ms bekle
    if (now - debounceTime < 200)
        return;

    // Belirli bir kanala dokunulup dokunulmadığını kontrol et
    bool touch = IQS7222A_anyChannelTouched() ;

    if (touch && !last_touch)
    {
        if (now - lastTapTime < 500)  // İki dokunuş arası süre 500ms'den küçükse çift tık
        {
            tapCount++;

            if (tapCount == 2)
            {
                SendUART();  // Seçili menü öğesini UART ile gönder
                tapCount = 0;
            }
        }
        else
        {
            tapCount = 1;
            selectedIndex++;

            if (selectedIndex >= MENU_ITEM_COUNT)
            	selectedIndex = 0;

            drawMenu(selectedIndex);  // Menüyü yeniden çiz
        }

        lastTapTime = now;
        debounceTime = now;
    }

    last_touch = touch;
}


