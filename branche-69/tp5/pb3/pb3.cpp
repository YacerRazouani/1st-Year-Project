#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h> 
#include <avr/interrupt.h>: Interrupts


void ajustementPWM ( 'modifier ici' ) {

// mise à un des sorties OC1A et OC1B sur comparaison

// réussie en mode PWM 8 bits, phase correcte

// et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6

// page 177 de la description technique du ATmega324PA)

OCR1A = 'modifier ici' ;

OCR1B = 'modifier ici' ;


// division d'horloge par 8 - implique une frequence de PWM fixe

TCCR1A = 'modifier ici' ;

TCCR1B = 'modifier ici' ;

TCCR1C = 0;

}
