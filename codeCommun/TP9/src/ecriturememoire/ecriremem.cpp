#include "memoire_24.h"


void initialisationUART ( void ) {

// 2400 bauds. Nous vous donnons la valeur des deux

// premier registres pour vous éviter des complications

UBRR0H = 0;

UBRR0L = 0xCF;

// permettre la reception et la transmission par le UART0

UCSR0A = (1 << UDRE0) ;  /* USART Data Register Empty. Set when the UDR0 register is
                            empty and new data can be transmitted. */

UCSR0B = (1 << RXEN0) | (1 << TXEN0) ; /* Receiver Enable AND Transmitter enable */

// Format des trames: 8 bits, 1 stop bits, none parity

UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) ; /* 8-bit (011) we didnt activate UCSZ20 */

}


unsigned char lireUART () {
    
    while( !(UCSR0A & (1 << RXC0))){                  /* Activate RXC0 to signal that data is             
                                                       available and has not been read yet */
    }
    
    return UDR0;
}

int main() {

	initialisationUART();
	Memoire24CXXX memory;	
	uint8_t temporary;
	uint16_t adresse = 0x00;
	for(;;){
		temporary = lireUART();
		memory.ecriture(adresse++,temporary);		
	}
	return 0;
}
