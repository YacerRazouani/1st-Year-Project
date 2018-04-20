#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h> 
#include <avr/interrupt.h>: Interrupts
#include "lumiereDel.h"

lumiereDel del;
void initialisationInterrupt ( void ) {
    cli ();
    
    DDRD &= ~(1 << PD2);
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC01);

    sei ();
}

ISR(INT0_vect) {

    _delay_ms(30);
    del.activateRed();
    _delay_ms(1500);
    del.activateNeutral();
    // Voir la note plus bas pour comprendre cette instruction et son rôle

    EIFR |= (1 << INTF0) ;

}


int main() 
{
    DDRD = 0x00;
    DDRB = 0xff;
    initialisationInterrupt();
    del.activateGreen();
    
    for(;;)
    {
    }
    
    return 0;
}
