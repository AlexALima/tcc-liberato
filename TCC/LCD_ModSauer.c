/*
 *	LCD interface example
 *	Uses routines from delay.c
 *	This code will interface to a standard LCD controller
 *	like the Hitachi HD44780. It uses it in 4 bit mode, with
 *	the hardware connected as follows (the standard 14 pin 
 *	LCD connector is used):
 *	
 *	PORTC bits 0-3 are connected to the LCD data bits 4-7 (high nibble)
 *	PORTA bit 3 is connected to the LCD RS input (register select)
 *	PORTA bit 1 is connected to the LCD EN bit (enable)
 *	 
 *	To use these routines, set up the port I/O then
 *	call lcd_init(), then other routines as required.
 *	
 */

#ifndef _XTAL_FREQ
 // Unless specified elsewhere, 4MHz system frequency is assumed
 #define _XTAL_FREQ 8000000
#endif

#include	<htc.h>
#include	"lcd.h"
#include	<stdio.h>

#define	LCD_RS RA4
//#define	LCD_RW RA2
#define LCD_EN RA5

#define LCD_DATA	PORTA

#define	LCD_STROBE()	((LCD_EN = 1),(LCD_EN=0))

/* write a byte to the LCD in 4 bit mode */

void
lcd_write(unsigned char c)
{
	__delay_us(40);
	LCD_DATA &= 0xF0;
	LCD_DATA |= ( ( c >> 4 ) & 0x0F );
	LCD_STROBE();
	LCD_DATA &=0xF0;
	LCD_DATA |= ( c & 0x0F );
	LCD_STROBE();
        __delay_us(40);
}

/*
 * 	Clear and home the LCD
 */

void
lcd_clear(void)
{
	LCD_RS = 0;
	__delay_us(40);
	lcd_write(0x1);
	__delay_ms(2);
}

/* write a string of chars to the LCD */

void
lcd_puts(const char * s)
{
	LCD_RS = 1;	// write characters
	while(*s)
		lcd_write(*s++);
}

/* write one character to the LCD */

void lcd_putc(char c)
{
	 switch (c) {
     case '\f'   : 
     			//LCD_RS = 0;	// write characters
					lcd_clear();	// Clear screen
          break;
     case '\n'   : 	
     			lcd_goto(0x40);        
     			break;
     case '\r'   : 	
     			LCD_RS = 0;	// write characters
					lcd_write(0x80);	  
					break;
     default     : 
     			LCD_RS = 1;	// write characters
					lcd_write( c );     
					break;
		}
}


/*
 * Go to the specified position
 */

void
lcd_goto(unsigned char pos)
{
	LCD_RS = 0;
	lcd_write(0x80+pos);
}
	
/* initialise the LCD - put into 4 bit mode */
void
lcd_init()
{
	char init_value;


	init_value = 0x3;

	LCD_RS = 0;
	LCD_EN = 0;
	//LCD_RW = 0;
	
	__delay_ms(15);	// wait 15mSec after power applied,
	LCD_DATA	 = init_value;
	LCD_STROBE();
	__delay_ms(5);
	LCD_STROBE();
	__delay_us(200);
	LCD_STROBE();
	__delay_us(200);
	LCD_DATA = 2;	// Four bit mode
	LCD_STROBE();

	lcd_write(0x28); // Set interface length
	__delay_us(50);
	lcd_write(0x0C); // Display On, Cursor On, Cursor OFF
	__delay_us(50);
	lcd_clear();	// Clear screen
	__delay_us(50);
	lcd_write(0x6); // Set entry Mode
	__delay_us(50);

        /*LOGO TC*/
        LCD_RS=0;
        lcd_write(0b01000000);
        LCD_RS=1;
        lcd_write(0b00001010);
        lcd_write(0b00011111);
        lcd_write(0b00001110);
        lcd_write(0b00011111);
        lcd_write(0b00001110);
        lcd_write(0b00011111);
        lcd_write(0b00001110);
        lcd_write(0b00000000);

        lcd_write(0b00000010);
        lcd_write(0b00000100);
        lcd_write(0b00001110);
        lcd_write(0b00010001);
        lcd_write(0b00011111);
        lcd_write(0b00010000);
        lcd_write(0b00001110);

        LCD_RS=0;
        lcd_clear();


}

void putch(char c) {
    lcd_putc(c);
}