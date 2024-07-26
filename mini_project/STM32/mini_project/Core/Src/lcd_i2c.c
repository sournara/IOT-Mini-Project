/*
 * lcd_i2c.c
 *
 *  Created on: Jul 24, 2024
 *      Author: iot
 */


#include "lcd_i2c.h"

extern I2C_HandleTypeDef hi2c1;  // I2C 핸들러 선언

#define LCD_ADDR 0x4E // I2C 주소 (예시)

void lcd_send_cmd(char cmd)
{
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd & 0xf0);
    data_l = ((cmd << 4) & 0xf0);
    data_t[0] = data_u | 0x0C;
    data_t[1] = data_u | 0x08;
    data_t[2] = data_l | 0x0C;
    data_t[3] = data_l | 0x08;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t *)data_t, 4, 100);
}

void lcd_send_data(char data)
{
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (data & 0xf0);
    data_l = ((data << 4) & 0xf0);
    data_t[0] = data_u | 0x0D;
    data_t[1] = data_u | 0x09;
    data_t[2] = data_l | 0x0D;
    data_t[3] = data_l | 0x09;
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, (uint8_t *)data_t, 4, 100);
}

void lcd_clear(void)
{
    lcd_send_cmd(0x01);
    HAL_Delay(2);
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }
    lcd_send_cmd(col);
}

void lcd_init(void)
{
    HAL_Delay(50);
    lcd_send_cmd(0x30);
    HAL_Delay(5);
    lcd_send_cmd(0x30);
    HAL_Delay(1);
    lcd_send_cmd(0x30);
    HAL_Delay(10);
    lcd_send_cmd(0x20);
    HAL_Delay(10);

    lcd_send_cmd(0x28);
    HAL_Delay(1);
    lcd_send_cmd(0x08);
    HAL_Delay(1);
    lcd_send_cmd(0x01);
    HAL_Delay(1);
    lcd_send_cmd(0x06);
    HAL_Delay(1);
    lcd_send_cmd(0x0C);
}

void lcd_send_string(char *str)
{
    while (*str)
    {
        lcd_send_data(*str++);
    }
}
