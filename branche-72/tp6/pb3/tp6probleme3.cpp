/*
 * tp6 pb3
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <memoire_24.h>

uint8_t phraseAEcrire[] = "*E*C*O*L*E* *P*O*L*Y*T*E*C*H*N*I*Q*U*E*";
uint8_t phraseLue[] = "";
uint8_t taille_tableau = sizeof(phraseAEcrire);

Memoire24CXXX maMemoire;


void initialisationUART () {

// 2400 bauds. Nous vous donnons la valeur des deux
// premier registres pour vous éviter des complications
	UBRR0H = 0;
	UBRR0L = 0xCF;
// permettre la reception et la transmission par le UART0
	//UCSR0A = 'modifier ici' ;
	UCSR0B |= (1 << RXEN0)| (1 << TXEN0) ;
// Format des trames: 8 bits, 1 stop bits, none parity
	UCSR0C |= (1 <<UCSZ01) | (1 <<UCSZ00);
}

void transmissionUART ( uint8_t donnee ) {
	
	while (!( UCSR0A & (1<<UDRE0)));                
    UDR0 = donnee; 
}


int main()
{
	initialisationUART ( );
	
	maMemoire.ecriture(0x00,phraseAEcrire,taille_tableau);
	_delay_ms(5);
	
	
	uint16_t adresse =0x00;
	uint8_t* car;
	
	while (*car != 0xFF){
		maMemoire.lecture(adresse,car);
		transmissionUART(*car);
		adresse+=1;
	
	}
	
}

