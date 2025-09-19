#ifndef ICONS_H
#define ICONS_H

#include <stdint.h>
#include <ST7735.h>


 typedef struct {
     const uint8_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;


extern const tImage lightbulb_open;
extern const tImage lightbulb_closed;
extern const tImage perde_acik;
extern const tImage perde_kapali;

void draw_image_mono(int x, int y, const tImage* img, uint16_t fgColor, uint16_t bgColor);



#endif
