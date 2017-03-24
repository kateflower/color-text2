#include <stdlib.h>
#include <6502.h>
#include <lynx.h>
#include <tgi.h>
// enable Mikeys interrupt response
//#define CLI asm("\tcli")

// These header files are related to the processor family, the Lynx and the TGI libraries.

//  this array will contain a string “tgi” that indicates that the loading of the driver was successful.
extern char lynxtgi[]; 


// lynx-160-102-16.tgi file. The filename is derived from the 160×102 resolution and 16 color palette

void initialize()
{
	// driver for the Tiny Graphics Interface (TGI)
	// allows for 2D graphics primitives like drawing and text 
  	// Lynx file is called lynx-160-102-16.tgi and is part of the Lynx specific CC65 files
	// function call to tgi_install will install the Lynx specific TGI driver
	// Lynx does not have the ability to load a driver dynamically it needs to be linked statically when the game is built.   
	tgi_install(&lynxtgi);

	// performs the initialization of the loaded drivers of which the TGI graphics driver is just one
	// There are two additional drivers (joystick and comlynx) that you can load as well. 
  	tgi_init();

	// represents the CLI assembly instruction for the 65SC02 processor
	// allows you to clear the Interrupt Disable flag of the processor,   
  	CLI();
	
  	while (tgi_busy())  {  };
 
  	tgi_setpalette(tgi_getdefpalette());
  	tgi_setcolor(COLOR_WHITE);
  	tgi_setbgcolor(COLOR_BLACK);
  	tgi_clear();
}

//
// Render the screen
//
// loop from 0 through 10 that reads error messages for the current index of the loop.
// It also sets the background color to 1 through 11 with the tgi_setbgcolor function 
// and then outputs the messages on a new line.
void show_screen()
{	
	const char* msg;
	char i;
	
	tgi_clear();
	tgi_setcolor(COLOR_WHITE);
		
	for (i = 0; i < 11; i++)
	{
		msg = tgi_geterrormsg(i);
		// The tgi_setbgcolor is Lynx specific and not available in the core TGI functino set.
		tgi_setbgcolor(i + 1);
		// Since the fonts are 8 bits high and the line-height is set to 9 
		// (the multiplication) there is a single pixel high gap between the pieces of text.
		tgi_outtextxy(0, i * 9, msg);
	} 
 
	// display is updated
	// tgi_updatedisplay function is a Lynx-only  
  	tgi_updatedisplay();
}

// The entry point is the main function
void main(void)  {	
  	initialize();
	  
	// An infinite loop where we remain forever  
  	while (1)
  	{
    	if (!tgi_busy())
    	{
      		show_screen();
    	}
  	};
}