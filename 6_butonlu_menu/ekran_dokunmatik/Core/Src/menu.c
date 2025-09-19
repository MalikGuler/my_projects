#include "menu.h"
#include "ST7735.h"
#include <string.h>
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;

uint8_t currentPage = 0;
uint32_t lastButtonPressTime = 0;
uint32_t debounceDelay = 200;

bool startupDone = false;
uint32_t startupTime = 0;

uint8_t currentTheme = 0;

const uint16_t themeColors[] = {RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE};
const uint8_t themeCount = sizeof(themeColors) / sizeof(themeColors[0]);




MenuItem menu_items[MENU_ITEM_COUNT] = {
    { &lightbulb_open, &lightbulb_closed, "Salon", false },
    { &lightbulb_open, &lightbulb_closed, "Yatak Od", false },
    { &lightbulb_open, &lightbulb_closed, "Cocuk Od", false },
    { &lightbulb_open, &lightbulb_closed, "Mutfak", false },
    { &lightbulb_open, &lightbulb_closed, "Tuvalet", false },
    { &lightbulb_open, &lightbulb_closed, "Banyo", false }
};




void drawMenu(uint8_t startIndex)
{
    if (startIndex == THEME_PAGE_INDEX) {
        drawThemePage();
        return;
    }
	  FillScreen(BLACK);

	    for (int i = 0; i < 2; i++) {
	        int index = (startIndex + i) % MENU_ITEM_COUNT;
	        int y = i * 32 + 2;

	        MenuItem* item = &menu_items[index];
	        const tImage* icon = item->state ? item->icon_on : item->icon_off;
	        const char* label = item->room_name;

	        // Oda ismi
	        ST7735_WriteString(4, y + 10, label, Font_7x10, themeColors[currentTheme], BLACK);

	        // Lamba ikonu (sağda)
	        if (icon)
	            draw_image_mono(90, y, icon, themeColors[currentTheme], BLACK);
	    }
}



void animateMenuTransition(uint8_t oldIndex, uint8_t newIndex, bool directionRight)
{
    const int stepSize = 8;
    const int totalSteps = 128 / stepSize;

    for (int i = 0; i <= totalSteps; i++) {
       FillScreen(BLACK);

        int offset = i * stepSize;

        for (int j = 0; j < 2; j++) {
            int indexOld = (oldIndex + j) % MENU_ITEM_COUNT;
            int y = j * 32 + 2;
            int x_old = directionRight ? -offset : offset;
            const tImage* iconOld = menu_items[indexOld].state ? menu_items[indexOld].icon_on : menu_items[indexOld].icon_off;
            const char* labelOld = menu_items[indexOld].room_name;

            ST7735_WriteString(4 + x_old, y + 10, labelOld, Font_7x10, themeColors[currentTheme], BLACK);
            if (iconOld)
                draw_image_mono(90 + x_old, y, iconOld, themeColors[currentTheme], 0x0000);

            int indexNew = (newIndex + j) % MENU_ITEM_COUNT;
            int x_new = directionRight ? (128 - offset) : (-128 + offset);
            const tImage* iconNew = menu_items[indexNew].state ? menu_items[indexNew].icon_on : menu_items[indexNew].icon_off;
            const char* labelNew = menu_items[indexNew].room_name;

            ST7735_WriteString(4 + x_new, y + 10, labelNew, Font_7x10, themeColors[currentTheme], BLACK);
            if (iconNew)
                draw_image_mono(90 + x_new, y, iconNew, themeColors[currentTheme], 0x0000);
        }

        HAL_Delay(10);
    }
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

    startupTime = HAL_GetTick();
    startupDone = false;
}


void drawThemePage(void)
{
    FillScreen(BLACK);

    const char* title = "Tema Secimi";
    ST7735_WriteString(10, 5, title, Font_7x10, themeColors[currentTheme], BLACK);

    draw_image_mono(90, 25, &lightbulb_open, themeColors[currentTheme], BLACK);


}

void theme_page_tuslari(void){

	  void drawThemePage();

	  uint32_t now = HAL_GetTick();


	  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) || HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET) {
		  currentTheme = (currentTheme + 1) % themeCount;
		  drawThemePage();
		  lastButtonPressTime = now;
		  return;
	  }

	  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3 || HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)) == GPIO_PIN_RESET) {
		  if (currentTheme == 0)
			  currentTheme = themeCount - 1;
		  else
			  currentTheme--;
		  drawThemePage();
		  lastButtonPressTime = now;
		  return;
	  }

	  // Sağ sol menü geçişi
	  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) {
		  uint8_t newPage = (currentPage + 2) % MENU_ITEM_COUNT;
		  animateMenuTransition(currentPage, newPage, true);
		  currentPage = newPage;
		  lastButtonPressTime = now;
		  return;
	  }

	  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET) {
		  uint8_t newPage = (currentPage + MENU_ITEM_COUNT - 2) % MENU_ITEM_COUNT;
		  animateMenuTransition(currentPage, newPage, false);
		  currentPage = newPage;
		  lastButtonPressTime = now;
		  return;
	  }

	  return;
	}









