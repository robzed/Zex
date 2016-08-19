#include "options.h"		// should always be first include
#include "SDL.h"
#include "LS_Standard.h"

#error "This file is not in the Zex2005 build"

#include	<Resources.h>
#include <Palettes.h>
#include <QDOffscreen.h>
#include <mactypes.h>

#include	"Error_handler.h"
#include	"splat_control.h"
#include	"splat_control_int.h"

LS_CGrafPtr the_cover_window;
LS_CGrafPtr the_game_window;
GWorldPtr offport;
LS_CGrafPtr displayport;
GDHandle displaydevice;
PixMapHandle src_pixmap_h;
int game_window_top,game_window_left;

short initial_depth;
//old drawsprocket code - not used in zex from version d5 onwards


int init_screen()
{
//SB 9/12/98
extern	CTabHandle mZexCLUT;
extern Ptr screenaddr;
extern int screenwidth;
int i;
EventRecord	theEvent;

	mZexCLUT = LS_GetCTable(128);	//our clut
	LS_DetachResource((Handle) mZexCLUT);
	switch_to_256();	//saves current depth
	create_blackout_window();
	create_game_window();
    create_virtual_screen();	//sets up screen address and screenwidth

    for (i=0;i<100;i++)
           {
			if (WaitNextEvent(everyEvent, &theEvent, 1, nil))
			{
			switch (theEvent.what)
				{
				case mouseDown:
				break;

				case keyDown:
				break;

				case autoKey:
				break;

				case updateEvt:
				BeginUpdate((WindowRef) theEvent.message);
				EndUpdate((WindowRef) theEvent.message);
				break;

				case diskEvt:
                break;

				case osEvt:
				break;

				case kHighLevelEvent:
				break;
				}	//end of swittch

			}	//end of if event

           }	//end of if i
  return 0;
}



void create_virtual_screen()
{
extern	CTabHandle mZexCLUT;
extern GWorldPtr offport;
Rect offrect = {0,0,480,640};
QDErr the_err;
extern LS_CGrafPtr displayport;
extern GDHandle displaydevice;
extern PixMapHandle src_pixmap_h;

//the_err=NewGWorld(&offport,8,&offrect,mZexCLUT,0,0);
the_err=NewGWorld(&offport,8,&offrect,0,0,0);

 if (the_err!=0) report_error ("Not enough memory to start up.");

GetGWorld(&displayport,&displaydevice);
src_pixmap_h=GetGWorldPixMap(offport);
start_modifying_picture();	//lock it and leave it that way...

}

void start_modifying_picture()
{
extern LS_CGrafPtr displayport;
extern GDHandle displaydevice;
extern PixMapHandle src_pixmap_h;
extern GWorldPtr offport;
extern Ptr screenaddr;
extern int screenwidth;
Boolean Err;


SetGWorld(offport,0);	//so we can use QD
SetPort((GrafPtr) offport);
Err=LockPixels(src_pixmap_h);
if (Err==0) report_error ("Fatal: Start_modify: LockPixels.");
screenaddr=GetPixBaseAddr(src_pixmap_h);
screenwidth=0x3fff & ((*(offport)->portPixMap))->rowBytes;
}



void switch_to_256()
{
extern short initial_depth;
PixMapHandle pixmap_h;
PixMap *pixmap;
GDHandle main_device_h;
GDevice *main_device;
short mode_id;

//get current ddepth so we can restore
main_device_h=GetMainDevice();
main_device=*main_device_h;
pixmap_h=(*main_device).gdPMap;
pixmap=*pixmap_h;
initial_depth=(*pixmap).pixelSize;

//switch to 8 bit
if (initial_depth!=8)
  {
   mode_id=HasDepth(main_device_h,8,0,1);
    if (mode_id==0) report_error("Zex requires a monitor capable of 256 colours");
    SetDepth (main_device_h,8,0,1);
  }
}

void create_blackout_window()
{
//RP 12/97
//SB 12/98
extern LS_CGrafPtr the_cover_window;
extern const RGBColor	rgbBlack;
Rect monitor_rect;
Str255 window_title ="";
GDHandle main_device_h;
GDevice *main_device;

main_device_h=GetMainDevice();
main_device=*main_device_h;
monitor_rect=(*main_device).gdRect;

the_cover_window=(LS_CGrafPtr) NewCWindow(0,&monitor_rect,window_title,0,2,(WindowPtr) -1,0,0);
SetPort ((GrafPtr) the_cover_window);
ShowWindow((GrafPtr)the_cover_window);
RGBBackColor(&rgbBlack);
EraseRect (&monitor_rect);
}

void create_game_window()
{
extern LS_CGrafPtr the_game_window;
extern LS_CGrafPtr displayport;
extern GDHandle displaydevice;
extern int game_window_top,game_window_left;

Rect dest_rect = {0,0,480,640};
Rect monitor_rect;
Str255 window_title ="";
UInt16 left,right,bottom,top,width,h,mwidth,mh,wleft,wtop;
GDHandle main_device_h;
GDevice *main_device;

the_game_window=(LS_CGrafPtr) NewCWindow(0,&dest_rect,window_title,0,2,(WindowPtr) -1,0,0);
SetPort ((GrafPtr) the_game_window);
displayport=(LS_CGrafPtr) the_game_window;

//Center the window
main_device_h=GetMainDevice();
main_device=*main_device_h;


monitor_rect=(*main_device).gdRect;
//calc window width
left=dest_rect.left;
right=dest_rect.right;
width=right-left;

//calc window width
left=monitor_rect.left;
right=monitor_rect.right;
mwidth=right-left;
//subtract win width from mwidth
width=mwidth-width;
wleft=width>>1;	//left
wleft&=0xfff8;	//octal align
//calc window h
top=dest_rect.top;
bottom=dest_rect.bottom;
h=bottom-top;

//calc monitor h
top=monitor_rect.top;
bottom=monitor_rect.bottom;
mh=bottom-top;
//subtract win width from mwidth
h=mh-h;
wtop=h>>1;	//top

MoveWindow((GrafPtr) the_game_window,wleft,wtop,true);
game_window_top=wtop;
game_window_left=wleft;
SetPort((GrafPtr) the_game_window);
ShowWindow((GrafPtr) the_game_window);
MoveTo (20,100);
DrawString ("Hello");
RGBBackColor(&rgbBlack);
EraseRect (&dest_rect);


}


//ееееееееееееееееееееееееееееееее
void stop_modifying_picture()
{
extern PixMapHandle src_pixmap_h;

UnlockPixels(src_pixmap_h);

}

LS_CGrafPtr debugport=0;

//ееееееееееееееееееееееееееееееее
void Show_it(int line_count)	//how many lines to splat starting at line 0
{
extern LS_CGrafPtr displayport;
extern GDHandle displaydevice;
extern PixMapHandle src_pixmap_h;
extern GWorldPtr offport;
extern Ptr screenaddr;
extern int screenwidth;
extern int game_window_top,game_window_left;

extern const RGBColor	rgbBlack;
extern const RGBColor	rgbWhite;
extern int game_window_top,game_window_left;

PixMapHandle dest_pmap;
char *src,*dest,*src_now,*dest_now;
Float64 *src_float,*dest_float;

int line,pix,dest_width;


//stop_modifying_picture();
//custom blit

if (debugport!=0)
{
 if (debugport!=displayport) Debugger();

}
 debugport=displayport;
dest_width=0x3fff & ((*(displayport)->portPixMap))->rowBytes;
dest_pmap=displayport->portPixMap;
dest=(**dest_pmap).baseAddr; //base address of vram, now offset for y
dest+=game_window_top*dest_width;
dest+=game_window_left;	//correct vram address of start of game window

src=screenaddr;

//dest_now=dest;
//src_now=src;
src_float=(Float64 *) src;
dest_float=(Float64 *) dest;


//the below code has been tweaked so optimiser outputs
//mtctr 640/8
//lfdu
//stfdu
//bdnz
//cmp 480
//add
//add
//ble
//for the inner loop
//so don't touch.
//on my 8500 this is twice as fast as copybits
for (line=0;line<line_count;line++)
    {
     for (pix=0;pix<640/8;pix++)
         *dest_float++=*src_float++;

     src = src+screenwidth;	//point to next src line;
     dest=dest+dest_width;

     src_float=(Float64 *) src;
     dest_float=(Float64 *) dest;

    }	//do next line


//copybits
//SetGWorld(displayport,displaydevice);
//RGBBackColor(&rgbWhite);
//RGBForeColor(&rgbBlack);
//
//CopyBits((BitMapPtr) *offport->portPixMap,(BitMapPtr) *displayport->portPixMap,&offport->portRect,&displayport->portRect,srcCopy,0);
//start_modifying_picture();

}



//int init_screen() {
//OSStatus the_error;
//extern	CTabHandle mZexCLUT;
//
//	  the_error=DSpStartup();
//
//	  if (the_error!=0)
//	  {
//	  	  report_error("This game requires DrawSprocket.");
//	  	  return	-1;
//	  }
//
//	  GraphicsSelectContext();
//
//return 0;
//}
//
//
//void GraphicsSelectContext()
//{
//extern	CTabHandle mZexCLUT;
//OSStatus	  	  theError;
//DisplayIDType	displayID;
//
//	  //е	Initialize our context description
//	  GraphicsInitAttributes(&mDisplayAttributes);
//
//	  //е	Tell DrawSprocket the exact needs we have for our game
//	  mDisplayAttributes.displayWidth		  	  = 640;
//	  mDisplayAttributes.displayHeight	  	  = 480;
//	  mDisplayAttributes.colorNeeds		  	  = kDSpColorNeeds_Require;
//	  mDisplayAttributes.backBufferDepthMask	= kDSpDepthMask_8;
//	  mDisplayAttributes.displayDepthMask		  = kDSpDepthMask_8;
//	  mDisplayAttributes.backBufferBestDepth	= 8;
//	  mDisplayAttributes.displayBestDepth		  = 8;
//	  mDisplayAttributes.pageCount	  	  	  = 2;	  //want triple buffering
//	  mDisplayAttributes.colorTable		  	  = mZexCLUT;
//
//	  //	Find the best context for these attributes
//	  //	This call will check to see if multiple monitors can handle the request
//	  //	If there is more than one valid device it will put up the selection interface
//	  theError = DSpUserSelectContext(&mDisplayAttributes, 0L, nil, &mDisplayContext);
//
//	  if (theError)
//	  	  report_error("A suitable display context could not be found.");
//
//
//	  mDisplayAttributes.contextOptions |= kDSpContextOption_PageFlip; //if possible give us hw page flipping
////	mDisplayAttributes.contextOptions |= 1U<<2; //if possible give us triple bufering
//
//	  //	Set the blanking color to black
//	  DSpSetBlankingColor(&rgbBlack);
//
//	  //	Find the DisplayID for this draw context and then find the GDevice
//	  //	that goes along with it.  We need this because if we need to perform
//	  //	QuickDraw operations in a context that isn't on the main monitor then
//	  //	we need to have the proper GDevice set or QuickDraw dies.
//	  theError =  DSpContext_GetDisplayID(mDisplayContext, &displayID);
//	  if (theError)
//	  	  report_error("DSpContext_GetDisplayID() had an error.");
//
//	  theError = DMGetGDeviceByDisplayID(displayID, &mZexGDH, false);
//	  if (theError)
//	  	  report_error("DMGetGDeviceByDisplayID() had an error.");
//
//	  //	Reserve the draw context so that we get the attributes we want
//	  theError = DSpContext_Reserve(mDisplayContext, &mDisplayAttributes);
//	  if (theError)
//	  	  report_error("Could not reserve display context.");
//
//  	// Fade the screen to black and make our context active
//	  DSpContext_FadeGammaOut(NULL, NULL);
//	  GraphicsActive();
//	  DSpContext_FadeGammaIn(NULL, NULL);
//
//}
//
//void GraphicsInitAttributes(DSpContextAttributes *inAttributes)
//{
//	  if (! inAttributes)
//	  	  report_error("Nil context passed to GraphicsInitAttributes().");
//
//   inAttributes->frequency=0;
//   inAttributes->displayWidth=0;
//   inAttributes->displayHeight=0;
//   inAttributes->reserved1=0;
//   inAttributes->reserved2=0;
//   inAttributes->colorNeeds=0;
//   inAttributes->colorTable=NULL;
//   inAttributes->contextOptions=0;
//   inAttributes->backBufferDepthMask=0;
//   inAttributes->displayDepthMask=0;
//   inAttributes->backBufferBestDepth=0;
//   inAttributes->pageCount=0;
//   inAttributes->gameMustConfirmSwitch=false;
//   inAttributes->reserved3[0]=0;
//   inAttributes->reserved3[1]=0;
//   inAttributes->reserved3[2]=0;
//   inAttributes->reserved3[3]=0;
//
//
////	BlockZero(inAttributes, sizeof (DSpContextAttributes));
//}
//
//void GraphicsActive(void)
//{
//OSStatus	  theErr;
//
//	  LS_HideCursor();
//
//	  theErr = DSpContext_SetState(mDisplayContext, kDSpContextState_Active);
//	  if (theErr)
//	  	  DebugStr("SetState active failed in GraphicsActive().");
//}

//void
//Show_it(void)
//{
//extern	DSpContextReference		    mDisplayContext;
//
//	  DSpContext_SwapBuffers(mDisplayContext, NULL, 0);
//}

//returns the address of the buffer to write into as a LS_CGrafPtr
//new routine, old dsp version below
LS_CGrafPtr Get_back_buffer(void) {
extern	LS_CGrafPtr	backBuff;
extern GWorldPtr offport;

return offport;
}


//LS_CGrafPtr Get_back_buffer(void) {
//extern	LS_CGrafPtr	  backBuff;
//extern	DSpContextReference		  mDisplayContext;
//OSStatus err;
//
//	  //е	Get a reference to the back buffer so we can draw into it
//	  err=DSpContext_GetBackBuffer(mDisplayContext, kDSpBufferKind_Normal, &backBuff);
//	  if (err) Debugger();
//return backBuff;
//}

void screen_quit() 	//Call before quitting game
{
extern GWorldPtr offport;
DisposeGWorld(offport);
}
//extern DSpContextReference		  mDisplayContext;
//Boolean OutBusyFlag=1;
////there's no mention of his anywhere, but drawsprocket can take a long time
////doing a splat, and if we try to quit whilst it is doing a splat
////our mac will hang whilst waiting for the splat to finish!!!!!!!!!
////(Good Old Apple!)
//	  while (OutBusyFlag!=0)
//	  {
//	   DSpContext_IsBusy(mDisplayContext,&OutBusyFlag);
//	  }
//
//	  DSpContext_FadeGammaOut(NULL, NULL);
//	  DSpContext_SetState(mDisplayContext, kDSpContextState_Inactive);
//	  LS_ShowCursor();
//	  DSpContext_FadeGammaIn(NULL, NULL);
//
//	  DSpContext_Release(mDisplayContext);
////	DSpShutdown();
//
//}
//

//show current buffer, switch and erase next buffer ready for drawing
void show_and_get_next_screen()
{
extern Ptr screenaddr;
extern int screenwidth;
extern GDHandle displaydevice;
extern LS_CGrafPtr the_drawing_buffer;
extern const RGBColor	rgbBlack;
GrafPtr		savePort;
GDHandle	saveGDH;

     //Show_it(WIND_BOTTOM);	//switch pages
	 Show_it(WIND_HEIGHT);

	 the_drawing_buffer=Get_back_buffer();
	 SetPort((GrafPtr)the_drawing_buffer);
	RGBBackColor(&rgbBlack);
//	SetGDevice(displaydevice);
	 EraseRect(&the_drawing_buffer->portRect);

     screenaddr=((*(the_drawing_buffer)->portPixMap))->baseAddr;
     screenwidth=0x3fff & ((*(the_drawing_buffer)->portPixMap))->rowBytes;

}

