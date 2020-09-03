#line 1 "C:/Users/Alexandre/Documents/Colégio/micros/TCC/LCD_ModSauer.c"
#line 1 "c:/users/alexandre/documents/colégio/micros/tcc/lcd.h"
#line 8 "c:/users/alexandre/documents/colégio/micros/tcc/lcd.h"
extern void lcd_write(unsigned char);



extern void lcd_clear(void);



extern void lcd_puts(const char * s);



extern void lcd_goto(unsigned char pos);



extern void lcd_init(void);

extern void lcd_putch(char);
#line 1 "c:/users/public/documents/mikroelektronika/mikroc pro for pic/include/stdio.h"
#line 37 "C:/Users/Alexandre/Documents/Colégio/micros/TCC/LCD_ModSauer.c"
void
lcd_write(unsigned char c)
{
 __delay_us(40);
  PORTA  &= 0xF0;
  PORTA  |= ( ( c >> 4 ) & 0x0F );
  (( RA5  = 1),( RA5 =0)) ;
  PORTA  &=0xF0;
  PORTA  |= ( c & 0x0F );
  (( RA5  = 1),( RA5 =0)) ;
 __delay_us(40);
}
#line 54 "C:/Users/Alexandre/Documents/Colégio/micros/TCC/LCD_ModSauer.c"
void
lcd_clear(void)
{
  RA4  = 0;
 __delay_us(40);
 lcd_write(0x1);
 __delay_ms(2);
}



void
lcd_puts(const char * s)
{
  RA4  = 1;
 while(*s)
 lcd_write(*s++);
}



void lcd_putc(char c)
{
 switch (c) {
 case '\f' :

 lcd_clear();
 break;
 case '\n' :
 lcd_goto(0x40);
 break;
 case '\r' :
  RA4  = 0;
 lcd_write(0x80);
 break;
 default :
  RA4  = 1;
 lcd_write( c );
 break;
 }
}
#line 101 "C:/Users/Alexandre/Documents/Colégio/micros/TCC/LCD_ModSauer.c"
void
lcd_goto(unsigned char pos)
{
  RA4  = 0;
 lcd_write(0x80+pos);
}


void
lcd_init()
{
 char init_value;


 init_value = 0x3;

  RA4  = 0;
  RA5  = 0;


 __delay_ms(15);
  PORTA  = init_value;
  (( RA5  = 1),( RA5 =0)) ;
 __delay_ms(5);
  (( RA5  = 1),( RA5 =0)) ;
 __delay_us(200);
  (( RA5  = 1),( RA5 =0)) ;
 __delay_us(200);
  PORTA  = 2;
  (( RA5  = 1),( RA5 =0)) ;

 lcd_write(0x28);
 __delay_us(50);
 lcd_write(0x0C);
 __delay_us(50);
 lcd_clear();
 __delay_us(50);
 lcd_write(0x6);
 __delay_us(50);


  RA4 =0;
 lcd_write(0b01000000);
  RA4 =1;
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

  RA4 =0;
 lcd_clear();


}

void putch(char c) {
 lcd_putc(c);
}
