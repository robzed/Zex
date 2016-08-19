// Name:   		TxtSplat.h
// Date Started:	25/11/98
// Author(s):		ID/RP
//
// Copyright(c) Lightsoft Software   		
//


//The base address you want the charcters draw to and the screen width
void InitText();  

//The string (80 max but increade it if you want more), X/Y co-ords on screen			
void SplatText(const char splat_str[80], int x_pos, int y_pos, unsigned char fg_colour, unsigned char bg_colour);
void SplatText_Small(const char splat_str[16], int x_pos, int y_pos, unsigned char fg_colour, unsigned char bg_colour);

void SplatText_Thrust(char splat_str[30], int x_pos, int y_pos);
//As above but with the colour you want transparent
void SplatText_Transparent(const char splat_str[80], int x_pos, int y_pos, int colour);
void Large_Draw_to_Double_Buffer_Transparent(char *char_ptr, int x_pos, int y_pos); // Colour is the colour that needs masking
void SplatTextLarge_Transparent(const char splat_str[80], int x_pos, int y_pos);
void SplatTextLarge_Treplace(char splat_str[80], int x_pos, int y_pos, int colour, int replace);
void Large_Draw_to_Double_Buffer_Treplace(char *char_ptr, int x_pos, int y_pos, int colour, int replace);
void SplatTextLarge_weird(char splat_str[80], int x_pos, int y_pos);
void Large_Draw_to_Double_Buffer_weird(char *char_ptr, int x_pos, int y_pos);
void SplatTextLarge_weirdB(char splat_str[80], int x_pos, int y_pos);
void Large_Draw_to_Double_Buffer_weirdB(char *char_ptr, int x_pos, int y_pos);
void SplatText_Transparent_Fade(const char splat_str[80], int x_pos, int y_pos, unsigned char colour);
void Draw_to_Double_Buffer_TransparentFade(char *char_ptr, int x_pos, int y_pos, unsigned char colour); // Colour is the colour that needs masking
void SplatText_Fade(char splat_str[80], int x_pos, int y_pos, unsigned char fg_colour, unsigned char  bg_colour);
void Draw_to_Double_Buffer_Fade(char *char_ptr, int x_pos, int y_pos , unsigned char fg_colour,unsigned char bg_colour);
void Draw_to_Double_Buffer_Small_Transparent(char *char_ptr, int x_pos, int y_pos , unsigned char fg_colour);
 void SplatText_Small_Transparent(const char splat_str[16], int x_pos, int y_pos, unsigned char fg_colour);
 
void SplatTextLarge_TransparentC(char splat_str[80], int x_pos, int y_pos, int colour);

void Large_Draw_to_Double_Buffer_TransparentC(char *char_ptr, int x_pos, int y_pos,int colour); // Colour is the colour that needs masking


int PixelWidthof_SplatTextLarge_Transparent(const char splat_str[80]);


