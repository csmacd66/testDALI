#ifndef __SSD1306_TEST_H__
#define __SSD1306_TEST_H__

void ssd1306_TestBorder(void);
void ssd1306_TestFonts(void);
void ssd1306_TestFPS(void);
void ssd1306_TestAll(void);
void ssd1306_TestLine(void);
void ssd1306_TestRectangle(void);
void ssd1306_TestCircle(void);
void ssd1306_TestArc(void);
void ssd1306_TestPolyline(void);
void ssd1306_showPart_sensor_ini(void);
void ssd1306_showPart_sensor301(uint32_t);
void ssd1306_showPart_sensor302_switch(uint32_t);
void ssd1306_showPart_sensor302_slider(uint32_t);
void ssd1306_showPart_sensor303(uint32_t);
void ssd1306_showPart_sensor303_warmup(uint8_t);
void ssd1306_showPart_sensor304(uint32_t);
#endif // __SSD1306_TEST_H__
