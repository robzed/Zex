// pictures.h
typedef struct {
UInt16 pwidth;
UInt16 pheight;
char *raw_data;
} LSRAW;


typedef struct {
UInt16 pwidth;
UInt16 pheight;
char *raw_data;	//rgba quads actually
} LSRAW_RGBA;

void print_pict(LSRAW *the_picture, UInt32 x, UInt32 y);
void print_crunched_pict(LSRAW *the_picture, UInt32 x, UInt32 y);
void print_crunched_pict_C(LSRAW *the_picture, UInt32 x, UInt32 y, UInt32 top, UInt32 bott);
void print_crunched_trans_pict(LSRAW *the_picture, UInt32 x, UInt32 y);
void print_crunched32_trans_pict(LSRAW *the_picture, UInt32 x, UInt32 y);
void print_crunched_translucent_pict(LSRAW *the_picture, UInt32 x, UInt32 y);
Handle GetZexPicture (unsigned long res_type, short resid);
void decrunch_pict(LSRAW *the_picture, LSRAW *where);
void print_pict_scaled(LSRAW *the_picture, UInt32 x, UInt32 y, UInt32 sf, UInt32 trans_on);
void print_pict_magnified(LSRAW *the_picture, UInt32 x, UInt32 y, int y_correction, int x_correction, float magnification);

void print_pict_quick(LSRAW *the_picture, int x, int y);

void erase_zex_rect(int w, int h);	//erase to black (0xff)

void erase_zex_rect_colour(int w, int h, unsigned char colour_8);	//erase to a given colour

void set_erase_colour(unsigned char colour_8);

void invert_y_pict(LSRAW *the_picture);


