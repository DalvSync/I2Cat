#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include "i2cat.h"
#include "font.h"   

#define I2C_ADDR 0x3C
int i2c_file;
unsigned char buffer[1024]; 

void I2C_Write(unsigned char* data, int len) {
    if (write(i2c_file, data, len) != len) {
        printf("Error! Can`t send data to the screen.\n");
    }
}

void Disp_Draw(uint8_t x, uint8_t y, uint8_t color) {
    // Check screen borders
    if (x >= 128 || y >= 64) {
        return;
    }

    int page = y / 8;
    int bit = y % 8;
    int idx = x + (page * 128);

    if (color == 1) {
        buffer[idx] |= (1 << bit);  // ON bit
    } else {
        buffer[idx] &= ~(1 << bit); // OFF bit
    }
}

//Screen initialization
void Disp_Init() {
    char *filename = "/dev/i2c-1";
    if ((i2c_file = open(filename, O_RDWR)) < 0) {
        printf("I can't open the I2C bus. Check the connection.\n");
        return;
    }

    if (ioctl(i2c_file, I2C_SLAVE, I2C_ADDR) < 0) {
        printf("Could not find the screen at 0x3C\n");
        return;
    }

    //Setings from Datasheet
    unsigned char cmds[] = {
        0x00, 0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40, 0x8D, 0x14, 
        0x20, 0x00, 0xA1, 0xC8, 0xDA, 0x12, 0x81, 0xCF, 0xD9, 0xF1, 
        0xDB, 0x40, 0xA4, 0xA6, 0xAF
    };
    
    I2C_Write(cmds, sizeof(cmds));

    Disp_Clear();
    Disp_Print(49, 10, "I2Cat", 1);
    Disp_Print(46, 25, "v. 1.3", 1);
    Disp_Print(31, 45, "by DalvSync", 1);
    
    Disp_Update();
    usleep(2000000);
    
    Disp_Clear();
    Disp_Update();
    printf("I2Cat: Screen init complete\n");
}

//Screen cleaner
void Disp_Clear() {
    memset(buffer, 0, 1024);
}

//Draw letter with a color selection
void Disp_DrawChar(uint8_t x, uint8_t y, unsigned char c, uint8_t color) {
    if (c < 32 || c > 126) c = 32;

    int font_index = ((c - 32) * 6) + 4;

    for (int i = 0; i < 6; i++) {
        unsigned char line = font6x8[font_index + i];

        for (int j = 0; j < 8; j++) {
            if (line & (1 << j)) {
                Disp_Draw(x + i, y + j, color);
            }
        }
    }
}

// Draw a line with a color selection
void Disp_Print(uint8_t x, uint8_t y, char *str, uint8_t color) {
    int cursor_x = x;
    while (*str) {
        Disp_DrawChar(cursor_x, y, *str, color);
        cursor_x += 6; 
        str++;
    }
}

void Disp_Update() {
    unsigned char reset_cursor[] = { 0x00, 0x21, 0, 127, 0x22, 0, 7 };
    I2C_Write(reset_cursor, sizeof(reset_cursor));

    unsigned char data_packet[1025];
    data_packet[0] = 0x40; // Data
    memcpy(&data_packet[1], buffer, 1024);

    I2C_Write(data_packet, 1025);
}
void Disp_PlayAnim(const unsigned char **frames, int count, int delay_ms, int loops) {
    int current_loop = 0;

    while (1) {
        if (loops > 0 && current_loop >= loops) {
            break;
        }

        for (int i = 0; i < count; i++) {
            memcpy(buffer, frames[i], 1024); 
            Disp_Update();
            usleep(delay_ms * 1000);
        }
        current_loop++;
    }
}
void Disp_ForceOn(uint8_t fl) {
    unsigned char cmd[2];

    cmd[0] = 0x00;

    if (fl){
        cmd[1] = 0xA5;
    } else {
        cmd[1] = 0xA4;
    }

    I2C_Write(cmd, 2);
}
void Disp_Invert(uint8_t inv) {
    unsigned char cmd[2];

    cmd[0] = 0x00;

    if (inv) {
        cmd[1] = 0xA7;
    } else {
        cmd[1] = 0xA6;
    }

    I2C_Write(cmd, 2);
}
void Disp_DLine(int x0, int y0, int x1, int y1, uint8_t color) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx, sy;

    if (x0 < x1) sx = 1; else sx = -1;
    if (y0 < y1) sy = 1; else sy = -1;

    int err = dx - dy;

    while (1) {
        Disp_Draw(x0, y0, color);
        if (x0 == x1 && y0 == y1){
            break;
        }
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}