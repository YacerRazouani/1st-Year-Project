#include "capteurDistance.h"
#include "son.h"
#include <avr/io.h> 
#include <avr/interrupt.h>
#define F_CPU 8000000UL
void capteurDistance::lireDetecteur()
{
    uint16_t moyenneGauche = 0;
    uint16_t moyenneDroit = 0;
    for (uint8_t i = 0; i < 20; i++)
    {
        coteGauche_ = detecteur.lecture(0);						//On lit la valeur du detecteur.
        coteDroit_ = detecteur.lecture(1);
        moyenneGauche += coteGauche_;
        moyenneDroit += coteDroit_;
    }
    moyenneDroit /= 20;
    moyenneGauche /= 20;
    
    coteGauche_ = moyenneGauche;
    coteDroit_ = moyenneDroit;
}

void capteurDistance::initialisationMinuterie ( void ) {
    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'etre interrompu alors que
    // le microcontroleur n'est pas pret...
    cli ();
    DDRA = 0x00;
    
    // sei permet de recevoir a nouveau des interruptions.
    sei ();
}


uint16_t capteurDistance::obtenirCoteGauche()
{
    return coteGauche_;
}


uint16_t capteurDistance::obtenirCoteDroit()
{
    return coteDroit_;
}

void capteurDistance::partirMinuterie ( uint16_t duree ) {
    TCNT2 = 0;
    
    OCR2A = duree;
    TCCR2A |= _BV(WGM21);
    TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);
    TIMSK2 |= _BV(OCIE2A);
}



