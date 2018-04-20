#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "lumiereDel.h"

lumiereDel del;
void initialisationMinuterie ( void ) {
    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'etre interrompu alors que
    // le microcontroleur n'est pas pret...
    cli ();

    // configurer et choisir les ports pour les entrees
    // et les sorties. DDRx... Initialisez bien vos variables
    DDRA = 0x03; // Output A
    DDRD = 0x00;

    // cette procédure ajuste le registre EIMSK
    // de ATmega324PA pour permettre les interruptions externes
    EIMSK |= _BV(INT0);

    // il faut sensibiliser les interruptions externes aux
    // changement de niveau du bouton-poussoir
    // en ajustant le registre EICRA
    EICRA |= _BV(ISC01) | _BV(ISC00);

    // sei permet de recevoir a nouveau des interruptions.
    sei ();
}


ISR(TIMER2_COMPA_vect)
{
        del.activateGreen();
        _delay_ms(500);
        del.activateNeutral();
        _delay_ms(500);
}

void partirMinuterie ( uint16_t duree ) {
    //timer = 0;
    // mode CTC du timer 1 avec horloge divisee par 1024
    // interruption après la duree specifiee
    TCNT2 = 0;
    
    OCR2A = duree;
    TCCR2A |= _BV(WGM21);
    TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);  
    TIMSK2 |= _BV(OCIE2A); 
}

int main() 
{
    DDRB = 0xff; 
    initialisationMinuterie();
    del.activateRed();
    _delay_ms(2000);
    partirMinuterie (2000);
    for(;;){
    }
    return 0;
}
