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

void roues::activateReverse(char r, uint8_t p)
{
    if (r == 'g' || r == 'G'){
        ajustementPWM(p,0);
        PORTD |= (1 << 6);  //left
    }
    if (r == 'd' || r == 'D'){
        ajustementPWM(0, p);
        PORTD |= (1 << 7);  //right
    }
}

void roues::activateForward(char r, uint8_t p)
{
    PORTD &= ~(1 << 6); //left  
    PORTD &= ~(1 << 7); //right
    if (r == 'g' || r == 'G')
        ajustementPWM(p, 0);
        
    if (r == 'd'|| r == 'D')
        ajustementPWM(0, p);
}

void roues::activateNeutral()
{
    ajustementPWM(0, 0);    
}
/*
void roues::activateBothForward(uint8_t p)
{

    // mise à un des sorties OC1A et OC1B sur comparaison

    // réussie en mode PWM 8 bits, phase correcte

    // et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6

    // page 177 de la description technique du ATmega324PA)

    OCR1A = p ;

    OCR1B = p;


    // division d'horloge par 8 - implique une frequence de PWM fixe

    TCCR1A |= (1 << WGM10 ) | (1 << COM1A1) | (1 << COM1B1); 

    TCCR1B |= (1 << CS11 ) ;

    TCCR1C = 0;

}*/
