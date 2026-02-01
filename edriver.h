#ifndef OLED_H
#define OLED_H

//Function to initialise display
void Disp_Init();

//Function for clear display (all pixels turn to off)
void Disp_Clear();

//Function to draw pixels (x,y)
//'color' - its pixel status; 1 - pixel on, 0 - pixel off
void Disp_Draw(int x, int y, int color);

//Function to send information to display
void Disp_Update();

//Function to draw 1 letter 
void Disp_DrawChar(int x, int y, char c, int color);

//Function to print words on screen
void Disp_Print(int x, int y, char *str, int color);

#endif