#ifndef ICONS_H
#define ICONS_H

#include <stdint.h>
#include <ST7735.h>

 typedef struct {
     const uint32_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;

extern const uint32_t icon1[24 * 24];
extern const uint32_t icon2[24 * 24];

extern const tImage home_icon;
extern const tImage light_bulb_icon;

void draw_image_mono(int x, int y, const tImage* img, uint16_t fgColor, uint16_t bgColor);

#endif
