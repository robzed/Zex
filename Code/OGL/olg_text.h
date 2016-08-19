// olg_text.h
int init_ogl_text();
void OGL_draw_monaco_bold(const char* the_string);
void OGL_draw_helvetica(char* the_string);
void OGL_draw_helvetica_small(char* the_string);

void OGL_draw_helveticaP(unsigned char* the_string);	//passcal string version


void OGL_DrawStr(char *str,int the_len,int x, int y);
void ZDrawString(const unsigned char * the_string);
void ZDrawString_c(const char * the_string);
void ZMoveTo(int x, int y);
void ZLineTo(int x, int y);
void ZRGBForeColor (const RGBColor * RGB_Colour);

