#ifndef __LCD_I2C_H
#define __LCD_I2C_H

#include "stm32f1xx_hal.h"

void lcd_init(void);
void lcd_send_cmd(char cmd);
void lcd_send_data(char data);
void lcd_putc(char *str);
void lcd_clear(void);
void lcd_goto(int row, int col); 
void lcd_effectWait(int row, int col);

#endif
