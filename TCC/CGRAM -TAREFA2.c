#ifndef _XTAL_FREQ
 // Unless specified elsewhere, 4MHz system frequency is assumed
 #define _XTAL_FREQ 20000000
#endif

#include <htc.h>
#include "lcd.h"
#include <stdio.h>

#define RS RD4 //Caso RS esteja em nivel lógico baixo"zero" os dados enviados ao LCD seram tratados com comandos, caso contrário será como dado;
#define EN RD5 //Coleta aquilo que esta sendo enviado a ele apenas quando houver uma transição de nivél alto para baixo (1 --> 0);
#define DATA PORTD
#define	LCD_STROBE()	((EN = 1),(EN=0))

void envia_byte_dado(int lcd_rs,int dado);
void envia_nibble_lcd(int dado);
void criando_cgram_carater();
void limpa_lcd();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void limpa_lcd()
{
envia_byte_dado(0,0b00000001);
__delay_ms(2);//Aguarda 2us para estabilizar o LCD;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void envia_nibble_lcd(int dado)
{
DATA=dado;
EN=1;
__delay_ms(1);//Recomendado para estabilizar o LCD;
EN=0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void envia_byte_dado(int lcd_rs,int dado)
{
RS=lcd_rs; //Se ZERO:Comando Se UM:Dado;
__delay_us(100); //Para estabilizar o LCD;
EN=0;//Desativa o Enable;
envia_nibble_lcd(dado>>4);
envia_nibble_lcd(dado & 0x0f);//Isso limpa os 4 bits mais significativos, assim como garante a veracidade dos 4bits menos significativos e serem enviados;

__delay_us(50); //Aguarda 40us para estabilziar o LCD;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void criando_cgram_carater()
{
/*********************************
 *        CRIANDO A CGRAM        *
 *********************************/
envia_byte_dado(0,0b01000000); //Zero informando que é comando, e informando o endereço inicial da CGRAM;
LCD_STROBE();
/*********************************
 *     CRIANDO OS CARACTERES     *
 *********************************/
// º = ENDEREÇO 0
envia_byte_dado(1,0b00001010);
LCD_STROBE();
envia_byte_dado(1,0b00011111);
LCD_STROBE();
envia_byte_dado(1,0b00001110);
LCD_STROBE();
envia_byte_dado(1,0b00011111);
LCD_STROBE();
envia_byte_dado(1,0b00001110);
LCD_STROBE();
envia_byte_dado(1,0b00011111);
LCD_STROBE();
envia_byte_dado(1,0b00001110);
LCD_STROBE();
envia_byte_dado(1,0b00000000);
LCD_STROBE();


//010234

limpa_lcd();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int cgram_init ()
{
criando_cgram_carater();
}



