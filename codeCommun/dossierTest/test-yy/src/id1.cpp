#include "lumiereDel.h"
void initialisationUART ( void ) {

// 2400 bauds. Nous vous donnons la valeur des deux

// premiers registres pour vous éviter des complications

UBRR0H = 0;

UBRR0L = 0xCF;

// permettre la reception et la transmission par le UART0

UCSR0A = (1 << UDRE0) ;  /* USART Data Register Empty. Set when the UDR0 register is
                            empty and new data can be transmitted. */

UCSR0B = (1 << RXEN0) | (1 << TXEN0) ; /* Receiver Enable AND Transmitter enable */

// Format des trames: 8 bits, 1 stop bits, none parity

UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) ; /* 8-bit (011) we didnt activate UCSZ20 */

}

void transmissionUART ( uint8_t donnee ) {

    while ( !( UCSR0A & (1 << UDRE0))) {            /* Cannot put "donnee" 
                                                     into UDR0 if UDRE bit is not set */
    }
    
    UDR0 = donnee;
}

unsigned char lireUART () {
    
    while( !(UCSR0A & (1<<RXC0))){                  /* Activate RXC0 to signal that data is             
                                                    available and has not been read yet */
    }
    
    return UDR0;
}

enum MessageDuLogiciel
{
    VitesseRoueG         = 0xf8,
    VitesseRoueD         = 0xf9,
    CouleurDel           = 0xfa, 
    InfoId               = 0xfb
};

enum MessageDuRobot
{
    NomRobot         = 0xf0,
    NumeroEquipe     = 0xf1,
    NumeroSection    = 0xf2, 
    Session          = 0xf3,
    CouleurBaseRobot = 0xf4,
    EtatInterrupt    = 0xf5,
    DistanceCapG     = 0xf6,
    DistanceCapD     = 0xf7
};

int main()
{
    
    //DDRB = 0xff;
    //lumiereDel lumiereDel;
    //lumiereDel.activateRed();
    //_delay_ms(3000);
    
    char I = 0x49;
    char i = 0x69;
    char j = 0x6A;
    char six = 0x36;
    char neuf = 0x39;
    char sept = 0x37;
    char deux = 0x32;
    char zero = 0x30;
    char trois = 0x33;
    char un = 0x31;
    char huit = 0x38;
    char tiret = 0x2D;
    
    initialisationUART();
    
    uint8_t instruction;
    
    instruction = lireUART();
    
    //si lireUART == 0xfb
    if(instruction == InfoId){
        
        //transmettre au logiciel le nom du robot
        transmissionUART(NomRobot);
        transmissionUART(I);
        transmissionUART(j);
        transmissionUART(i);
        
        //transmettre au logiciel le NumeroEquipe
        transmissionUART(NumeroEquipe);
        transmissionUART(six);
        transmissionUART(neuf);
        transmissionUART(sept);
        transmissionUART(deux);
        
        transmissionUART(Session);
        transmissionUART(un);
        transmissionUART(huit);
        transmissionUART(tiret);
        transmissionUART(un);
    }
    
    
    return 0;
}
