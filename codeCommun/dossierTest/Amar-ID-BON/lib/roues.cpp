#include "roues.h"

void roues::ajustementPWM (uint8_t ratioRoueGauche, uint8_t ratioRoueDroite) {

    // mise à un des sorties OC1A et OC1B sur comparaison

    // réussie en mode PWM 8 bits, phase correcte

    // et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6

    // page 177 de la description technique du ATmega324PA)

    OCR1A = ratioRoueGauche ;

    OCR1B = ratioRoueDroite;


    // division d'horloge par 8 - implique une frequence de PWM fixe

    TCCR1A |= (1 << WGM10 ) | (1 << COM1A1) | (1 << COM1B1); 

    TCCR1B |= (1 << CS11 ) ;

    TCCR1C = 0;
}

void roues::ajustementPWMGauche (uint8_t ratioRoueGauche) {

    // mise à un des sorties OC1A et OC1B sur comparaison

    // réussie en mode PWM 8 bits, phase correcte

    // et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6

    // page 177 de la description technique du ATmega324PA)

    OCR1A = ratioRoueGauche ;


    // division d'horloge par 8 - implique une frequence de PWM fixe

    TCCR1A |= (1 << WGM10 ) | (1 << COM1A1) | (1 << COM1B1); 

    TCCR1B |= (1 << CS11 ) ;

    TCCR1C = 0;
}

void roues::ajustementPWMDroite (uint8_t ratioRoueDroite) {

    // mise à un des sorties OC1A et OC1B sur comparaison

    // réussie en mode PWM 8 bits, phase correcte

    // et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6

    // page 177 de la description technique du ATmega324PA)



    OCR1B = ratioRoueDroite;


    // division d'horloge par 8 - implique une frequence de PWM fixe

    TCCR1A |= (1 << WGM10 ) | (1 << COM1A1) | (1 << COM1B1); 

    TCCR1B |= (1 << CS11 ) ;

    TCCR1C = 0;
}

void roues::activateReverseGauche(uint8_t ratio)
{
    //PORTD &= (1 << 6); 
    PORTD |= ~(1 << 6);
    ajustementPWMGauche(ratio);
}

void roues::activateReverseDroite( uint8_t ratio)
{
    //PORTD &= (1 << 7);
    PORTD |= ~(1 << 7);
    ajustementPWMDroite(ratio);
}

void roues::activateForwardGauche(uint8_t ratio)
{
    //PORTD |= ~(1 << 6); 
    PORTD &= (1 << 6);
    ajustementPWMGauche(ratio);
        
}

void roues::activateForwardDroite(uint8_t ratio)
{
    //PORTD |= ~(1 << 7);
    PORTD &= (1 << 7);
    ajustementPWMDroite(ratio);
}

void roues::activateNeutral()
{
    ajustementPWMGauche(0);
    ajustementPWMDroite(0);
}
