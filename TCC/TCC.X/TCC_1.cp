#line 1 "C:/Users/Alexandre/Documents/Colégio/micros/TCC/TCC.X/TCC_1.c"
#line 1 "c:/users/public/documents/mikroelektronika/mikroc pro for pic/include/stdio.h"
#line 1 "c:/users/alexandre/documents/colégio/micros/tcc/tcc.x/lcd.h"
#line 8 "c:/users/alexandre/documents/colégio/micros/tcc/tcc.x/lcd.h"
extern void lcd_write(unsigned char);



extern void lcd_clear(void);



extern void lcd_puts(const char * s);



extern void lcd_goto(unsigned char pos);



extern void lcd_init(void);

extern void lcd_putch(char);
#line 16 "C:/Users/Alexandre/Documents/Colégio/micros/TCC/TCC.X/TCC_1.c"
unsigned char DADO_RX=0xFF;
unsigned char posicao=0;
unsigned char cont=0;
unsigned char buffer=0xFF;
unsigned char cont_timer=0;
unsigned char ok=0xFF;
bit rec=0;
bit ON=0;
bit enter=0;

void menu();


void acao(){
  RC1 =1;
 __delay_ms(100);
 switch(posicao){
 case 0:
 buffer=0;
 break;

 case 1:
 buffer=2;
 break;

 case 2:
 buffer=4;
 break;

 case 3:
 buffer=8;
 break;

 case 4:
 buffer=32;
 break;

 case 5:
 buffer=47;
 break;

 case 6:
 buffer=73;
 break;

 case 7:
 buffer=74;
 break;

 case 8:
 buffer=161;
 break;

 case 9:
 buffer=165;
 break;

 case 10:
 buffer=191;
 break;

 case 11:
 buffer=193;
 break;

 case 12:
  RC1 =0;
 lcd_clear();
 printf("   At%c mais!",1);
 __delay_ms(400);
 __delay_ms(400);
 RBIE=0;
 cont_timer=0;
 TMR1=0;
 TMR1ON=0;
 ON=0;
  RC2 =0;
 break;

 }
 if(ON){
 ok=i2c_le(0x0C);
 if(ok==0xAA){
 i2c_escreve(buffer);
 TMR2ON=1;
 }
 else{
 cont=0;
 TMR2ON=0;
  RC1 =0;
 lcd_clear();
 printf("%c  SCID-2013   %c",0,0);
 __delay_us(100);
 printf("\nErro de gravacao");
 __delay_ms(200);
 __delay_ms(200);
 __delay_ms(200);
 __delay_ms(200);
 __delay_ms(200);
 menu();
 INTCON=0b11011000;
 return;
 }
 }
 INTCON=0b11011000;
}

void menu(){

 lcd_clear();
 printf("%c  SCID-2013   %c\n",0,0);
 __delay_us(100);
 switch(posicao){
 case 0:
 printf("     7400");
 break;

 case 1:
 printf("     7402");
 break;

 case 2:
 printf("     7404");
 break;

 case 3:
 printf("     7408");
 break;

 case 4:
 printf("     7432");
 break;

 case 5:
 printf("     7447");
 break;

 case 6:
 printf("     7473");
 break;

 case 7:
 printf("     7474");
 break;

 case 8:
 printf("     74161");
 break;

 case 9:
 printf("     74165");
 break;

 case 10:
 printf("     74191");
 break;

 case 11:
 printf("     74193");
 break;

 case 12:
 lcd_clear();
 printf("%c  SCID-2013   %c\n",0,0);
 printf("    Desligar");
 break;
 default:
 lcd_clear();
 printf("ERRO");
 printf("   ERRO");
 break;

 }

}

void principal(){
 while(1){

 if (!ON) SLEEP();

 }

}

void inicializacao(){
 lcd_clear();
 printf("%cSubstituidor de\nCKT's integrados", 0);
 __delay_ms(500);
 __delay_ms(500);
 __delay_ms(500);
 lcd_clear();
 printf(" Alexandre Lima\n TC 2013 - 4411");
 __delay_ms(500);
 __delay_ms(500);
 __delay_ms(500);
 lcd_clear();
 INTF=0;
}

void bip(){

  RC0 =1;
 __delay_ms(200);
  RC0 =0;
 __delay_ms(200);
  RC0 =1;
 __delay_ms(200);
  RC0 =0;

}

void interrupt xandi(){
 if(INTF && INTE){
 INTF=0;
 __delay_ms(10);
 TMR1=0;
 cont_timer=0;
 if(ON==0){
 INTCON=0;
  RC2 =1;
 ON=1;
 posicao=0;
 inicializacao();
 menu();
 T1CON=0b00110001;
 TMR1IE=1;
 INTCON=0b11011000;
 INTF=0;
 return;
 }

 if(ON){
 INTCON=0;
 acao();
 }
 }

 if(RBIF && RBIE){
 PORTB=PORTB;
 RBIF=0;
 __delay_ms(10);
 TMR1=0;
 cont_timer=0;
 if(ON==0) return;

 if(!RB1){
 if(++posicao==13) posicao=0;
 }

 if(!RB2){
 if (--posicao==0xFF) posicao=12;
 }
 menu();
 }

 if(TMR2IF && TMR2IE){
 TMR2IF=0;
 if(++cont==1){
 INTCON=0;
 ok=0xFF;
 ok=i2c_le(0x2F);
 if(ok==buffer){
 cont=0;
 TMR2ON=0;
  RC1 =0;
 bip();
 }
 else{
 cont=0;
 TMR2ON=0;
  RC1 =0;
 lcd_clear();
 printf("%c  SCID-2013   %c",0,0);
 __delay_us(100);
 printf("\nErrode gravacao2");
 __delay_ms(200);
 __delay_ms(200);
 __delay_ms(200);
 __delay_ms(200);
 __delay_ms(200);
 menu();
 }

 INTCON=0b11011000;
 }
 }

 if(TMR1IF && TMR1IE){
 TMR1IF=0;
 if(++cont_timer>=70){
 cont_timer=0;
 TMR1ON=0;
  RC1 =0;
 lcd_clear();
 printf("   At%c mais!",1);
 __delay_ms(400);
 __delay_ms(400);
 RBIE=0;
 ON=0;
  RC2 =0;
 }
 }

}

void main()
{
 OSCCON=0b01110000;
 ANSEL=0;
 ANSELH=0;
 TRISA=0;
 TRISB=0xFF;
 TRISC=0b10111000;
  RC2 =1;
  RC1 =0;
  RC0 =0;
 OPTION_REG=0;
 lcd_init();
 i2c_Init();
 INTCON=0;
 IOCB=0b00000110;
 T2CON=0b01111010;
 PR2=0xFF;
 TMR2IE=1;
 T1CON=0b00110000;
 TMR1IE=1;
  RC2 =0;
 ON=0;
 INTCON=0b11011000;
 while(1) principal();




}
