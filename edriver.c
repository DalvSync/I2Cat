#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include "edriver.h"
#include "font.h"   

#define I2C_ADDR 0x3C
int i2c_file;
unsigned char buffer[1024]; 

void I2C_Write(unsigned char* data, int len) {
    if (write(i2c_file, data, len) != len) {
        printf("Error! Can`t send data to the screen.\n");
    }
}

void Disp_Draw(int x, int y, int color) {
    // Check screen borders
    if (x < 0 || x >= 128 || y < 0 || y >= 64) {
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
    Disp_Update();
    printf("Lib: Screen init complete\n");
}

//Screen cleaner
void Disp_Clear() {
    memset(buffer, 0, 1024);
}

//Draw letter with a color selection
void Disp_DrawChar(int x, int y, char c, int color) {
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
void Disp_Print(int x, int y, char *str, int color) {
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
