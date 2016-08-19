// *****************************************************************MODULE HEADER*****
// * FILENAME		: fcircle.h
// * MODULE TITLE	: Circle drawing external definitions file
// * PROJECT		: Zex
// * DATE STARTED	: 11th April 1999, Sun 10:00pm
// * ORIGINAL AUTHOR: Rob Probin
// *
// * COPYRIGHT (c) 1999 Lightsoft
// *		   (c) 1999 Robert Probin and Stuart Ball
// *		   http://www.lightsoft.co.uk/
// *
// * Lightsoft is a trading name of Robert Probin and Stuart Ball.
// *
// ***********************************************************************************
// * HISTORY:
// * Date Initial		  Descrip.
// *
// *
// *
// ***********************************************************************************


void init_circle(void);
void fcircle(int x, int y, int r, unsigned char colour, int mode);

#define MODE_SOLID 0
#define MODE_TRANSPARENT 1

void create_outer_flare(int x, int y, int size, int brightness);

