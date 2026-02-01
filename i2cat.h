#ifndef I2CAT_H
#define I2CAT_H

//Function to initialise display
void Disp_Init();

//Function for clear display (all pixels turn to off)
void Disp_Clear();

//Function to draw pixels (x,y)
//'color' - its pixel status; 1 - pixel on, 0 - pixel off
void Disp_Draw(int x, int y, int color);

//Function to send image on display
void Disp_Update();

//Function to draw 1 letter (Example: Disp_DrawChar(10, 25, 'A', 1);)
void Disp_DrawChar(int x, int y, char c, int color);

//Function to print words on screen (Example: Disp_Print(10, 25, "Hello World!", 1);)
void Disp_Print(int x, int y, char *str, int color);

//Function to play gif (Example: Disp_Anim(my_anim_array, define_in_anim, 100(in ms),0);)
//For infinity animation write 0 in 'loops'
void Disp_Anim(const unsigned char **frames, int count, int delay_ms, int loops);

#endif
