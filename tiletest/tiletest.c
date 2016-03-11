/* include the image data */
#include "test.h"

/* include the map data */
#include "testmap.h" 

//button identifiers
#define BUTTON_RIGHT 16
#define BUTTON_LEFT  32
#define BUTTON_UP    64
#define BUTTON_DOWN  128
#define BUTTONS (*(volatile unsigned int*)0x04000130)

//vertical refresh register
#define REG_DISPSTAT   *(volatile unsigned short*)0x4000004


//wait for vertical refresh
void WaitVBlank(void)
{
    while((REG_DISPSTAT & 1));
}



/* the mode 0 flag */
#define MODE_0 0x0

/* the width and height of the screen */
#define WIDTH 240
#define HEIGHT 160

/* the flag to enable various background layers */
#define BG0_ENABLE 0x100
#define BG1_ENABLE 0x200
#define BG2_ENABLE 0x400
#define BG3_ENABLE 0x800

//scrolling registers for background 0
#define REG_BG0HOFS *(volatile unsigned short*)0x4000010
#define REG_BG0VOFS *(volatile unsigned short*)0x4000012


/* the graphics display control register */
#define REG_DISPCNT ((volatile unsigned long*) 0x4000000)

/* the screen memory pointer */
#define SCREEN ((volatile unsigned short*) 0x6000000)

/* the location of background 0 */
#define REG_BG0_PTR *(volatile unsigned short*)0x4000008

/* TODO what's this about */
#define SCREEN_SHIFT 8
#define WRAPAROUND 0x1
#define CharBaseBlock(n) (((n)*0x4000)+0x6000000)
#define ScreenBaseBlock(n) (((n)*0x800)+0x6000000)

/* sets up 256 color mode for tile maps */
#define BG_COLOR256 0x80

/* identify palette memory */
#define BG_PALETTE_MEMORY ((unsigned short*)0x5000000)

/* different sizes for the tile maps */
#define TEXTBG_SIZE_256x256 0x0
#define TEXTBG_SIZE_256x512 0x8000
#define TEXTBG_SIZE_512x256 0x4000
#define TEXTBG_SIZE_512x512 0xC000

/* scrolling registers for background 0 */
#define REG_BG0_XSCROLL *(volatile unsigned short*)0x4000010
#define REG_BG0_YSCROLL *(volatile unsigned short*)0x4000012

/* the registers which trigeer a DMA operation to start */
#define REG_DMA_SOURCE *(volatile unsigned int*)0x40000D4
#define REG_DMA_DESTINATION *(volatile unsigned int*)0x40000D8
#define REG_DMA_COUNT *(volatile unsigned int*)0x40000DC

/* flag to enable direct memory access */
#define DMA_ENABLE 0x80000000

/* start it right away */
#define DMA_TIMING_IMMEDIATE 0x00000000

/* transfer 16 or 32 bit memory? */
#define DMA_16 0x00000000
#define DMA_32 0x04000000

/* combined flags which turn on DMA right away for 16 or 32 bits */
#define DMA_32_NOW (DMA_ENABLE | DMA_TIMING_IMMEDIATE | DMA_32)
#define DMA_16_NOW (DMA_ENABLE | DMA_TIMING_IMMEDIATE | DMA_16)

/* function to use the GBA's hardware memory copy */
void dma_memcpy(void* source, void* dest, unsigned count, unsigned mode) {
    /* ensure that one of the valid modes are passed */
    if (mode != DMA_16_NOW && mode != DMA_32_NOW) {
        return;
    }

    /* trigger the DMA hardware copy to run - the CPU is
     * automatically suspended until the transfer is done */
    REG_DMA_SOURCE = (unsigned int) source;
    REG_DMA_DESTINATION = (unsigned int) dest;
    REG_DMA_COUNT = count | mode;
}

/* the interrupt functions are this type */
typedef void (*intrp)( );

/* this function is used for interrupts which we ignore */
void interrupt_ignore( ) {
}

/* this table specifies which interrupts we handle which way */
const intrp IntrTable[13] = {
    interrupt_ignore,   /* V Blank interrupt */
    interrupt_ignore,   /* H Blank interrupt */
    interrupt_ignore,   /* V Counter interrupt */
    interrupt_ignore,   /* Timer 0 interrupt */
    interrupt_ignore,   /* Timer 1 interrupt */
    interrupt_ignore,   /* Timer 2 interrupt */
    interrupt_ignore,   /* Timer 3 interrupt */
    interrupt_ignore,   /* Serial communication interrupt */
    interrupt_ignore,   /* DMA 0 interrupt */
    interrupt_ignore,   /* DMA 1 interrupt */
    interrupt_ignore,   /* DMA 2 interrupt */
    interrupt_ignore,   /* DMA 3 interrupt */
    interrupt_ignore,   /* Key interrupt */
};


/* the main function */
int main( ) {
    int x = 0, y = 0;

    /* we set the mode to mode 0 with background 0 turned on*/
    *REG_DISPCNT = MODE_0 | BG0_ENABLE;

    /* set up background 0 */
    REG_BG0_PTR = BG_COLOR256 | TEXTBG_SIZE_256x256 |
        (31 << SCREEN_SHIFT) | WRAPAROUND;

    /* load the palette into background palette memory */
    dma_memcpy((void*) test_palette, (void*) BG_PALETTE_MEMORY,
            256, DMA_32_NOW);

    /* load the tile image into tile memory */
    dma_memcpy((void*) test_data, (void*) CharBaseBlock(0),
            (test_width * test_height), DMA_16_NOW);

    /* copy the tile map itself into memory */
    unsigned short* bg0map =(unsigned short*)ScreenBaseBlock(31);
    dma_memcpy((void*)testmap, (void*)bg0map, 1024, DMA_32_NOW);

    /* we now loop forever displaying the image */
    while (1) {
		//D-pad moves background
		if(!(BUTTONS & BUTTON_LEFT)) x--;
		if(!(BUTTONS & BUTTON_RIGHT)) x++;
		if(!(BUTTONS & BUTTON_UP)) y--;
		if(!(BUTTONS & BUTTON_DOWN)) y++;
        //
        //wait for vertical refresh
		WaitVBlank();

		//use hardware background scrolling
		REG_BG0VOFS = y ;
		REG_BG0HOFS = x ;

		
        int n;
		for(n = 0; n < 4000; n++);
    }
}


