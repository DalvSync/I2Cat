#ifndef I2CAT_H
#define I2CAT_H

//Function to initialise display
void Disp_Init();

//Function for clear display (all pixels turn to off)
void Disp_Clear();

//Function to draw pixels (x,y)
//'color' - its pixel status; 1 - pixel on, 0 - pixel off
void Disp_Draw(uint8_t x, uint8_t y, uint8_t color);

//Function to send image on display
void Disp_Update();

//Function to draw 1 letter (Example: Disp_DrawChar(10, 25, 'A', 1);)
void Disp_DrawChar(uint8_t x, uint8_t y, char c, uint8_t color);

//Function to print words on screen (Example: Disp_Print(10, 25, "Hello World!", 1);)
void Disp_Print(uint8_t x, uint8_t y, char *str, uint8_t color);

//Function to play gif (Example: Disp_Anim(my_anim_array, define_in_anim, 100(in ms),0);)
//For infinity animation write 0 in 'loops'
void Disp_Anim(const unsigned char **frames, int count, int delay_ms, int loops);

//Function to ON all pixels on screen
//Example: Disp_ForceOn(1); -- Display ON all pixels on screen, ignore buffer; Disp_ForceOn(0); -- Disp go back to information in buffer
void Disp_ForceOn(uint8_t fl);

//Function to invert image on display
//Example: Disp_Invert(1); -- Display has been inverted; Disp_Invert(0); -- Display in normal mode
void Disp_Invert(uint8_t inv);

//Function to draw diagonal line
//Example: Disp_DLine(0, 0, 127, 63, 1); From left-top to right-bottom
void Disp_DLine(int x0, int y0, int x1, int y1, uint8_t color);

//Function to draw circle
//Example: Disp_DrawCircle(64, 32, 30, 1);
void Disp_DrawCircle(int x0, int y0, int r, uint8_t color);

#endif