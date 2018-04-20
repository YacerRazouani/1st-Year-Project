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

enum RobotID
{
    name        = 0x49,
    name1       = 0x4A
};



int main()

{   

{
    DDRB = 0xff;
    
<<<<<<< HEAD
    char i = 105;
    char j = 106;
=======

>>>>>>> be9b93de45ae687c8a55ae3bbf30cb2fe3d01066
    lumiereDel lumiereDel;
    lumiereDel.activateRed();
    _delay_ms(3000);
    initialisationUART();

    uint8_t instruction = lireUART();

    uint8_t instruction;
    uint8_t message = 0xf0;
    instruction = lireUART();
    
    uint8_t sendName = 0xf0;
    if(instruction == 0xfb){
<<<<<<< HEAD
        transmissionUART(message);
        transmissionUART(i);
        transmissionUART(j);
        transmissionUART(i);
        }
    
=======

        initialisationUART();
        transmissionUART('i');
        lumiereDel.activateGreen();
        _delay_ms(3000);
    }
>>>>>>> be9b93de45ae687c8a55ae3bbf30cb2fe3d01066
    
    return 0;
}
