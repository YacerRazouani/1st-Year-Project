#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h> 
#include <avr/interrupt.h>: Interrupts
	
	
volatile uint8_t state = 0; 	
volatile uint8_t minuterieExpiree;
volatile uint8_t boutonPoussoir;

void activateRed(){
    PORTB = 2;
}

void activateGreen(){
    PORTB = 1;
}

bool userPush() {
    bool push = false;	
	if ( PIND & 0x04 ) {
        _delay_ms(10);
        if ( PIND & 0x04 ){
            push = true;
        }
    }
	return push;
}

bool userRelease() {
    bool release = false;
    if ( !(PIND & 0x04) ) {
        _delay_ms(10);
    if( !(PIND & 0x04) ) {
            release = true;
        }
    }
    return release;
}


void initialisation ( void ) {

    // cli est une routine qui bloque toutes les interruptions.

    // Il serait bien mauvais d'être interrompu alors que

    // le microcontroleur n'est pas prêt...

    cli ();


    // configurer et choisir les ports pour les entrées

    // et les sorties. DDRx... Initialisez bien vos variables

    DDRB = 0xff; // PORT B est en mode sortie
    DDRD = 0x00; // PORT D est en mode entree

    // cette procédure ajuste le registre EIMSK

    // de l’ATmega324PA pour permettre les interruptions externes
    
    EIMSK |= (1 << INT0) ;


    // il faut sensibiliser les interruptions externes aux

    // changements de niveau du bouton-poussoir

    // en ajustant le registre EICRA

    EICRA |= (1 << ISC01) ;


    // sei permet de recevoir à nouveau des interruptions.

    sei ();

}

// ISR(INT0_vect) {
// 
//     _delay_ms(30);
// 
//     // se souvenir ici si le bouton est pressé ou relâché
// 
//     int buttonPressed = 1;
// 
//     // changements d'états tels que ceux de la
// 
//     // semaine précédente
//     if (buttonPressed == 1){
//         state++;
//         buttonPressed = 0;
//     }
//     // Voir la note plus bas pour comprendre cette instruction et son rôle
// 
//     EIFR |= (1 << INTF0) ;
// 
// }	

ISR (TCNT1) {

minuterieExpiree = 1;

}


ISR (INT0_vect) {

boutonPoussoir = 1;

// anti-rebond
userPush();

}
	
	
int main()
{
    initialisation();
    
    for(;;){
       activateRed();
       _delay_ms(1000);
       
    }//end for

	return 0;
}
