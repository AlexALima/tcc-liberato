#include <htc.h>
#include <stdio.h>
#include <lcd.h>

#include <xc.h>

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)




#ifndef _XTAL_FREQ
        #define _XTAL_FREQ 8000000
#endif

#define PIC_VDD RC1
#define BUZZER RC0
#define DISPLAY RB7
#define SS RC6

unsigned char DADO_RX=0xFF;
unsigned char posicao=0;
unsigned char cont=0;
unsigned char buffer=0xFF;
unsigned char cont_timer=0;
unsigned char ok=0xFF;
unsigned char portas_1=0;
unsigned char portas_2=0;
bit rec=0;
bit ON=0;
bit enter=0;
bit selec_portas=0;
bit oi=0;

void menu();

void spi_init(void){

	// Initialise I2C MSSP
	// Master 100KHz
	TRISC5=0;           	// set SCL and SDA pins as inputs
	TRISC3=0;

	SSPCON = 0b00110010;
	SSPCON2 = 0x00;

	SSPSTAT = 0b10000000; 	// Slew rate disabled

}

unsigned char spi_read(unsigned char dado){
    unsigned char rec=0xFF;
    SSPEN=1;
    SS=0;
    SSPBUF=dado;
    while(!BF);
    rec=SSPBUF;
    SS=1;
    SSPEN=0;
    return(rec);
}

void spi_write(unsigned char dado){
    SSPBUF=dado;
}

void acao(){
    PIC_VDD=1;
    buffer=0xFF;
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
            buffer=86;
            break;

        case 9:
            buffer=165;
            break;

        case 10:
            buffer=191;
            break;

        case 11:
            buffer=192;
            break;

        case 12:
            PIC_VDD=0;
            lcd_clear();
            printf("   At%c mais!",1);
            __delay_ms(400);
            __delay_ms(400);
            INTCON=0b10010000;
            cont_timer=0;
            TMR1=0;
            TMR1ON=0;
            ON=0;
            DISPLAY=0;
            break;

        case 13:
            buffer=222;
            break;

    }
    if(ON){
        ok=spi_read(0xAC);
        if(ok==0xAA){
            ok=spi_read(buffer);
            TMR2ON=1;
        }
        else {
            PIC_VDD=0;
            lcd_clear();
            printf("%c  SCID-2013   %c",0,0);
            __delay_us(100);
            printf("\nErro de gravacao");
            __delay_ms(200);
            __delay_ms(200);
            __delay_ms(200);
            __delay_ms(200);
            __delay_ms(200);
        }
        INTCON=0b11011000;
    }
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
            printf("     4027*");
            break;

        case 7:
            printf("     7474");
            break;

        case 8:
            printf("     7486");
            break;

        case 9:
            printf("     74165");
            break;

        case 10:
            printf("     74191");
            break;

        case 11:
            printf("     74192");
            break;

        case 12:
            lcd_clear();
            printf("%c  SCID-2013   %c\n",0,0);
            printf("    Desligar");
            break;

        case 13:
            printf(" Selec. portas");
            break;

        default:
            lcd_clear();
            printf("ERRO");
            printf("   ERRO");
            break;

    }
    
}

void bip(){

    BUZZER=1;
    __delay_ms(100);
    BUZZER=0;
    __delay_ms(50);
    BUZZER=1;
    __delay_ms(100);
    BUZZER=0;
}

void principal(){
    while(1){

        if (!ON) SLEEP();
        if(selec_portas){
            posicao=0;
            portas_1=0;
            portas_2=0;
            while(!oi){
                __delay_ms(200);
                lcd_clear();
                printf("%c  SCID-2013   %c",0,0);
                __delay_us(100);
                switch(posicao){
                    case 0: printf("\nPorta 1: AND");
                            break;
                    case 1: printf("\nPorta 1: NAND");
                            break;
                    case 2: printf("\nPorta 1: OR");
                            break;
                    case 3: printf("\nPorta 1: NOR");
                            break;
                    case 4: printf("\nPorta 1: XOR");
                            break;
                    case 5: printf("\nPorta 1: XNOR");
                            break;
                    case 6: printf("\nPorta 1: NOT");
                            break;
                    case 14:posicao=6;
                            break;
                    default:posicao=0;
                            break;
                }
            }
            oi=0;
            __delay_ms(100);
            portas_1|=posicao;
            INTCON=0b11011000;
            while(!oi){
                __delay_ms(200);
                lcd_clear();
                printf("%c  SCID-2013   %c",0,0);
                __delay_us(100);
                switch(posicao){
                    case 0: printf("\nPorta 2: AND");
                            break;
                    case 1: printf("\nPorta 2: NAND");
                            break;
                    case 2: printf("\nPorta 2: OR");
                            break;
                    case 3: printf("\nPorta 2: NOR");
                            break;
                    case 4: printf("\nPorta 2: XOR");
                            break;
                    case 5: printf("\nPorta 2: XNOR");
                            break;
                    case 6: printf("\nPorta 2: NOT");
                            break;
                    case 14:posicao=6;
                            break;
                    default:posicao=0;
                            break;
                }
            }
            oi=0;
            __delay_ms(100);
            portas_1|=(posicao<<4);
            INTCON=0b11011000;
            while(!oi){
                __delay_ms(200);
                lcd_clear();
                printf("%c  SCID-2013   %c",0,0);
                __delay_us(100);
                switch(posicao){
                    case 0: printf("\nPorta 3: AND");
                            break;
                    case 1: printf("\nPorta 3: NAND");
                            break;
                    case 2: printf("\nPorta 3: OR");
                            break;
                    case 3: printf("\nPorta 3: NOR");
                            break;
                    case 4: printf("\nPorta 3: XOR");
                            break;
                    case 5: printf("\nPorta 3: XNOR");
                            break;
                    case 6: printf("\nPorta 3: NOT");
                            break;
                    case 14:posicao=6;
                            break;
                    default:posicao=0;
                            break;
                }
            }
            oi=0;
            __delay_ms(100);
            portas_2|=posicao;
            INTCON=0b11011000;
            while(!oi){
                __delay_ms(200);
                lcd_clear();
                printf("%c  SCID-2013   %c",0,0);
                __delay_us(100);
                switch(posicao){
                    case 0: printf("\nPorta 4: AND");
                            break;
                    case 1: printf("\nPorta 4: NAND");
                            break;
                    case 2: printf("\nPorta 4: OR");
                            break;
                    case 3: printf("\nPorta 4: NOR");
                            break;
                    case 4: printf("\nPorta 4: XOR");
                            break;
                    case 5: printf("\nPorta 4: XNOR");
                            break;
                    case 6: printf("\nPorta 4: NOT");
                            break;
                    case 14:posicao=6;
                            break;
                    default:posicao=0;
                            break;
                }
            }
            oi=0;
            __delay_ms(100);
            portas_2|=(posicao<<4);
            PIC_VDD=1;
            ok=spi_read(0xBD);
            if(ok==0xAA){
                ok=spi_read(portas_1);
                __delay_ms(50);
            }
            ok=spi_read(0xBE);
            if(ok==0xAA){
                ok=spi_read(portas_2);
                __delay_ms(50);
            }
            ok=spi_read(0xCC);
            if(ok==portas_2){
                cont=0;
                PIC_VDD=0;
                bip();
            }
            selec_portas=0;
            INTCON=0b11011000;
            posicao=0;
            menu();
        }

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

void interrupt xandi(){
    if(INTF && INTE){
        INTF=0;
        __delay_ms(10);
        TMR1=0;
        cont_timer=0;
        if(ON==0){
            INTCON=0;
            DISPLAY=1;
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
            if(selec_portas){
               oi=1;
               return;
            }
            else acao();
            if(!ON) return;
            menu();
        }
    }

    if(RBIF && RBIE){
        PORTB=PORTB;
        RBIF=0;
        __delay_ms(80);
        TMR1=0;
        cont_timer=0;
        if(ON==0) return;

        if(!RB1){
            if(++posicao==14) posicao=0;
        }

        if(!RB2){
            if (--posicao==0xFF) posicao=13;
        }
        if(!selec_portas) menu();
    }

    if(TMR2IF && TMR2IE){
        TMR2IF=0;
        if(++cont==5){
            INTCON=0;
            ok=0xFF;
            ok=spi_read(0xCC);
            if(ok==buffer){
                cont=0;
                TMR2ON=0;
                PIC_VDD=0;
                if(buffer==222) selec_portas=1;
                bip();
        }
        else{
            cont=0;
            TMR2ON=0;
            PIC_VDD=0;
            lcd_clear();
            printf("%c  SCID-2013   %c",0,0);
            __delay_us(100);
            printf("\nErro de leitura");
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
            PIC_VDD=0;
            lcd_clear();
            printf("   At%c mais!",1);
            __delay_ms(400);
            __delay_ms(400);
            RBIE=0;
            ON=0;
            DISPLAY=0;//cortar transistor
        }
    }
    
}

void main()
{
    OSCCON=0b01110000; //8MHz
    ANSEL=0;
    ANSELH=0;
    TRISA=0;
    TRISB=0b01111111;
    TRISC=0b10111100;
    SS=1;
    DISPLAY=0;
    PIC_VDD=0;
    BUZZER=0;
    OPTION_REG=0;
    lcd_init();
    spi_init();
    INTCON=0;
    IOCB=0b00000110;
    T2CON=0b01111010;
    PR2=0xFF;
    TMR2IE=1;
    T1CON=0b00110000;
    TMR1IE=1;
    DISPLAY=0;
    ON=0;
    INTCON=0b11011000;
    while(1) principal();
    
            
        
    
}