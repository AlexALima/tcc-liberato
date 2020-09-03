#include <htc.h>
#include <stdio.h>

#pragma config FOSC = INTRCIO// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

#ifndef _XTAL_FREQ
   #define _XTAL_FREQ 8000000
#endif

#define PINOS_18 RA0

unsigned char DADO_RX=0xFF;
unsigned char PORTA1=0xFF;
unsigned char PORTA2=0xFF;
unsigned char PORTA3=0xFF;
unsigned char PORTA4=0xFF;
bit up1;
bit up2;

void spi_init(void){

	// Initialise I2C MSSP
	// Master 100KHz
	TRISC7=0;           	// set SCL and SDA pins as inputs
	TRISB6=1;

	SSPCON = 0b00110100;

	SSPSTAT = 0b00000000; 	// Slew rate disabled
        SSPIE=1;
}

void spi_write(unsigned char dado){
    SSPBUF=dado;
}

void tabela(unsigned char dado){
    switch (dado){
        case 0: PORTA=0b11111011;
                PORTB=0b10001111;
                PORTC=0b11111001;
                break;
        case 1: PORTA=0b11111111;
                PORTB=0b11101111;
                PORTC=0b11111011;
                break;
        case 2: PORTA=0b11111111;
                PORTB=0b10011111;
                PORTC=0b11111000;
                break;
        case 3: PORTA=0b11111111;
                PORTB=0b11001111;
                PORTC=0b11111000;
                break;
        case 4: PORTA=0b11111011;
                PORTB=0b11101111;
                PORTC=0b11111010;
                break;
        case 5: PORTA=0b11111011;
                PORTB=0b11001111;
                PORTC=0b11111100;
                break;
        case 6: PORTA=0b11111011;
                PORTB=0b10001111;
                PORTC=0b11111110;
                break;
        case 7: PORTA=0b11111111;
                PORTB=0b11101111;
                PORTC=0b11111001;
                break;
        case 8: PORTA=0b11111011;
                PORTB=0b10001111;
                PORTC=0b01111000;
                break;
        case 9: PORTA=0b11111011;
                PORTB=0b11101111;
                PORTC=0b11111000;
                break;
        case 10:PORTA=0b11111111;
                PORTB=0b10011111;
                PORTC=0b11111110;
                break;
        case 11:PORTA=0b11111111;
                PORTB=0b11001111;
                PORTC=0b11111110;
                break;
        case 12:PORTA=0b11111011;
                PORTB=0b11111111;
                PORTC=0b11111010;
                break;
        case 13:PORTA=0b11111011;
                PORTB=0b11011111;
                PORTC=0b11111100;
                break;
        case 14:PORTA=0b11111011;
                PORTB=0b10011111;
                PORTC=0b11111110;
                break;
        case 15:PORTA=0b11111111;
                PORTB=0b11111111;
                PORTC=0b11111111;
                break;
        default: break;
    }
}



void interrupt xandi(void){

    if (SSPIF && SSPIE) {//testa se é recepção serial
        SSPIF=0;
        DADO_RX=SSPBUF;
        if(DADO_RX==0xAA) DADO_RX=0xFF;
        BF=0;
    }

    if (EEIF && EEIE) {//testa se é recepção serial
        EEIF=0;
    }
}

void gravar_eeprom(unsigned char adr){
    GIE=0;
    WREN=1;
    EEADR=adr;
    EEDAT=DADO_RX;
    EECON2=0x55;
    EECON2=0xAA;
    WR=1;
    GIE=1;
    WREN=0;
}

void modo_programacao(void){
    PINOS_18=1;
    spi_init();
    EEIE=1;
    spi_write(0xAA);
    while(DADO_RX==0xFF);
    if(DADO_RX==0xAC){
        while(DADO_RX==0xAC);
        SSPIE=0;
        gravar_eeprom(0x01);
        SLEEP();
        RD=1;
        spi_write(EEDAT);
        SSPIE=1;
        while(DADO_RX!=0xCC);
        DADO_RX=0xFF;
        return;
    }
    if(DADO_RX==0xBD){
        while(DADO_RX==0xBD);
        SSPIE=0;
        gravar_eeprom(0x21);
        SLEEP();
        RD=1;
        spi_write(0xAA);
        DADO_RX=0xFF;
        return;
    }
    if(DADO_RX==0xBE){
        while(DADO_RX==0xBE);
        SSPIE=0;
        gravar_eeprom(0x22);
        SLEEP();
        RD=1;
        spi_write(EEDAT);
        SSPIE=1;
        while(DADO_RX!=0xCC);
        DADO_RX=0xFF;
        return;
    }
}

void AX7400(void){
    PINOS_18=0;
    TRISA=0b11111110;
    TRISB=0b10111111;
    TRISC=0b01101011;
    while(1){
        RC4=!(RA5&&RC5);
        RC7=!(RC3&&RC6);
        RB6=!(RB5&&RB4);
        RC2=!(RC1&&RC0);
    }
}

void AX7402(void){
    PINOS_18=0;
    TRISA=0b11011110;
    TRISB=0b11101111;
    TRISC=0b11110110;
    while(1){
        RA5=!(RC5||RC4);
        RC3=!(RC7||RC6);
        RB4=!(RB5||RB6);
        RC0=!(RC1||RC2);
    }
}

void AX7404(void){
    PINOS_18=0;
    TRISA=0b11111110;
    TRISB=0b10101111;
    TRISC=0b01010101;
    while(1){
        RC5=!RA5;
        RC3=!RC4;
        RC7=!RC6;
        RB6=!RB5;
        RB4=!RC2;
        RC1=!RC0;
    }
}

void AX7408(void){
    PINOS_18=0;
    TRISA=0b11111110;
    TRISB=0b10111111;
    TRISC=0b01101011;
    while(1){
        RC4=(RA5&&RC5);
        RC7=(RC3&&RC6);
        RB6=(RB5&&RB4);
        RC2=(RC1&&RC0);
    }
}

void AX7432(void){
    PINOS_18=0;
    TRISA=0b11111110;
    TRISB=0b10111111;
    TRISC=0b01101011;
    while(1){
        RC4=(RA5||RC5);
        RC7=(RC3||RC6);
        RB6=(RB5||RB4);
        RC2=(RC1||RC0);
    }
}

void AX7447(void){  //
    PINOS_18=1;
    TRISA=0b11111010;
    TRISB=0b10001111;
    TRISC=0b11111000;
    unsigned char num=0;
    while(1){
        if(RC5 && RC4 && RC3){
            num=((RC6<<3)|(RA5<<2)|(RA4<<1)|(RC7));
            tabela(num);
        }
        else{
            if(!RC5 && RC4) tabela(8);
            if(!RC3 && RC5 && RC4) tabela(15);
            if(!RC4) tabela(15);
        }
    }
}

void AX7473(void){
    PINOS_18=1;
    TRISA=0b11111110;
    TRISB=0b10011111;
    TRISC=0b00111111;
    while(1){
        if(!RC4) up1=0;
        if(RA5 && RC3){
            if(RC4 && !up1){
                up1=1;
                RC6=RC5;
                RC7=!RC6;
            }
        }
        else{
            if(!RA5 && !RC3){
                RC6=1;
                RC7=1;
            }
            if(!RA5 && RC3){
               RC6=0;
               RC7=1;
           }
           if(RA5 && !RC3){
               RC6=1;
               RC7=0;
           }
        }
        //2
        if(!RC2) up2=0;
        if(RC0 && RB4){
            if(RC2 && !up2){
                up2=1;
                RB5=RC1;
                RB6=!RB5;
            }
        }
        else{
            if(!RC0 && !RB4){
                RB5=1;
                RB6=1;
            }
            if(!RC0 && RB4){
               RB5=0;
               RB6=1;
           }
           if(RC0 && !RB4){
               RB5=1;
               RB6=0;
           }
        }
    }
}

void AX7474(void){
    PINOS_18=0;
    TRISA=0b11111110;
    TRISB=0b10011111;
    TRISC=0b00111111;
    while(1){
        if(!RC4) up1=0;
        if(RA5 && RC3){
            if(RC4 && !up1){
                up1=1;
                RC6=RC5;
                RC7=!RC6;
            }
        }
        else{
            if(!RA5 && !RC3){
                RC6=1;
                RC7=1;
            }
            if(!RA5 && RC3){
               RC6=0;
               RC7=1;
           }
           if(RA5 && !RC3){
               RC6=1;
               RC7=0;
           }
        }
        //2
        if(!RC2) up2=0;
        if(RC0 && RB4){
            if(RC2 && !up2){
                up2=1;
                RB5=RC1;
                RB6=!RB5;
            }
        }
        else{
            if(!RC0 && !RB4){
                RB5=1;
                RB6=1;
            }
            if(!RC0 && RB4){
               RB5=0;
               RB6=1;
           }
           if(RC0 && !RB4){
               RB5=1;
               RB6=0;
           }
        }
    }
}


void AX7486(void){
    PINOS_18=0;
    TRISA=0b11111110;
    TRISB=0b10111111;
    TRISC=0b01101011;
    while(1){
        RC4=(RA5^RC5);
        RC7=(RC3^RC6);
        RB6=(RB5^RB4);
        RC2=(RC1^RC0);
    }
}

void AX74165(void){
    PINOS_18=1;
    TRISA=0b11111110;
    TRISB=0b10111111;
    TRISC=0b01111111;
    unsigned char data=0;
    if(RB5) data|=0b10000000;
    else data&=0b01111111;
    RB6=0;
    RC7=1;
    while(1){
        if(!RA5) up1=0;
        if(RA2) up1=1;
        if(!RA4){
            up1=1;
            data=0;
            if(RC6) data|=0b00000001;
            if(RC3) data|=0b00000010;
            if(RC4) data|=0b00000100;
            if(RC5) data|=0b00001000;
            if(RC0) data|=0b00010000;
            if(RC1) data|=0b00100000;
            if(RC2) data|=0b01000000;
            if(RB4) data|=0b10000000;
            RB6=data;
            RC7=!RB6;
            data>>=1;
            if(RB5) data|=0b10000000;
            else data&=0b01111111;
        }
        if(RA5 && !up1){
             up1=1;
             if(RB5) data|=0b10000000;
             else data&=0b01111111;
             RB6=data;
             RC7=!RB6;
             data>>=1;
        }
    }
}


void AX74191(void){
    PINOS_18=1;
    TRISA=0b11011110;
    TRISB=0b11111111;
    TRISC=0b00011001;
    unsigned char num=0;
    PORTA=0;
    PORTB=0;
    PORTC=0;
    while(1){
        if(!RC0 && !RC3 &&(num==15)) RC1=0; //RC up
        else if(!RC0 && RC3 &&(num==0)) RC1=0; //RC down
        else RC1=1;

        if(num==15 && !RC3) RC2=1;
        else if(num==0 && RC3) RC2=1;
        else RC2=0;

        if(!RC0) up1=0;

        if(!RC4 && RB4){  //enabled e load
            if(RC0 && !up1){  //clock up
                up1=1;
                if(RC3){  //se down
                    if (--num==0xFF) num=15;
                }
                else{
                    if (++num==16) num=0;
                }
                RC5=(num);
                RA5=(num>>1);
                RC6=(num>>2);
                RC7=(num>>3);
            }
        }
        else{
            if(!RB4){
                up1=1;
                num=0;
                if(RA2) num|=0b00000001;
                if(RA4) num|=0b00000010;
                if(RB5) num|=0b00000100;
                if(RB6) num|=0b00001000;
                RC5=(num);
                RA5=(num>>1);
                RC6=(num>>2);
                RC7=(num>>3);
            }
        }

    }
}

void AX74192(void){
    PINOS_18=1;
    TRISA=0b11011110;
    TRISB=0b11111111;
    TRISC=0b00011001;
    unsigned char num=0;
    PORTA=0;
    PORTB=0;
    PORTC=0;
    while(1){
        if(!RC4 && (num==0)) RC1=0; //BO
        else RC1=1;

        if(!RC3 && (num==9)) RC2=0; //CO
        else RC2=1;

        if(!RC3) up1=0;
        if(!RC4) up2=0;

        if(RB4 && !RC0){  //enabled e load
            if(RC4 && RC3 && !up2){  //clock down
                up2=1;
                if (--num==0xFF) num=9;
                RC5=(num);
                RA5=(num>>1);
                RC6=(num>>2);
                RC7=(num>>3);
            }

            if(RC4 && RC3 && !up1){  //clock up
                up1=1;
                if (++num==10) num=0;
                RC5=(num);
                RA5=(num>>1);
                RC6=(num>>2);
                RC7=(num>>3);
            }
        }
        else{
            if(!RB4){
                up1=1;
                up2=1;
                num=0;
                if(RA2) num|=0b00000001;
                if(RA4) num|=0b00000010;
                if(RB5) num|=0b00000100;
                if(RB6) num|=0b00001000;
                RC5=(num);
                RA5=(num>>1);
                RC6=(num>>2);
                RC7=(num>>3);
            }
        }

    }
}

void select_portas(void){
    PINOS_18=0;
    TRISA=0b11111110;
    TRISB=0b10111111;
    TRISC=0b01101011;
    EEADR=0x21;
    RD=1;
    NOP();
    PORTA1=(EEDAT&0x0F);
    PORTA2=((EEDAT>>4)&0x0F);
    EEADR=0x22;
    RD=1;
    NOP();
    PORTA3=(EEDAT&0x0F);
    PORTA4=((EEDAT>>4)&0x0F);
    while(1){
        switch(PORTA1){
            case 0: RC4=(RC5&RA5); break;
            case 1: RC4=!(RC5&RA5); break;
            case 2: RC4=(RC5|RA5); break;
            case 3: RC4=!(RC5|RA5); break;
            case 4: RC4=(RC5^RA5); break;
            case 5: RC4=!(RC5^RA5); break;
            case 6: RC4=(!RC5); break;
        }
        switch(PORTA2){
            case 0: RC7=(RC3&RC6); break;
            case 1: RC7=!(RC3&RC6); break;
            case 2: RC7=(RC3|RC6); break;
            case 3: RC7=!(RC3|RC6); break;
            case 4: RC7=(RC3^RC6); break;
            case 5: RC7=!(RC3^RC6); break;
            case 6: RC7=(!RC3); break;
        }
        switch(PORTA3){
            case 0: RB6=(RB4&RB7); break;
            case 1: RB6=!(RB4&RB7); break;
            case 2: RB6=(RB4|RB7); break;
            case 3: RB6=!(RB4|RB7); break;
            case 4: RB6=(RB4^RB7); break;
            case 5: RB6=!(RB4^RB7); break;
            case 6: RB6=(!RB7); break;
        }
        switch(PORTA4){
            case 0: RC2=(RC0&RC1); break;
            case 1: RC2=!(RC0&RC1); break;
            case 2: RC2=(RC0|RC1); break;
            case 3: RC2=!(RC0|RC1); break;
            case 4: RC2=(RC0^RC1); break;
            case 5: RC2=(RC0^RC1); break;
            case 6: RC2=(!RC0); break;
        }
    }
}


void main(){
    OSCCON=0b01110000; // 8MHz
    ANSEL=0;
    ANSELH=0;
    TRISA=0b11111110;
    PINOS_18=0;
    TRISB=0xFF;
    TRISC=0xFF;
    OPTION_REG=0b10000000;
    INTCON=0b11000000; // GIE=1 e PIE=1
    EECON1=0;
    while(RA4&&RA5&&RC4&&RC3&&RC5&&RA1&&RA2&&RC0&&RC1&&RC2) modo_programacao();// Está no programador
    EEADR=0x01;
    RD=1;
    NOP();
    switch(EEDAT){
        case 0: AX7400(); break;

        case 2: AX7402(); break;

        case 4: AX7404(); break;

        case 8: AX7408(); break;

        case 32: AX7432(); break;

        case 47: AX7447(); break;

        case 73: AX7473(); break;

        case 74: AX7474(); break;

        case 86: AX7486(); break;

        case 165: AX74165(); break;

        case 191: AX74191(); break;

        case 192: AX74192(); break;

        case 222: select_portas(); break;

        default:
            GIE=0;
            SLEEP();
            break;
    }
}
