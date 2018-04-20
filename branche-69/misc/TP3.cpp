/*
 * Name: TP3 Problem #1
 * By: Ghaly, Amar && Razouani, Yacer
 * Description: Allume la DEL en rouge lorsque le bouton-poussoir est 
 *              appuyer cinq fois.
 */
#include <avr/io.h> 
#define F_CPU 8000000UL
#include <util/delay.h>


int main(){
	DDRA = 0xff; // PORT A est en mode sortie
	DDRB = 0xff; // PORT B est en mode sortie
	DDRC = 0xff; // PORT C est en mode sortie
	DDRD = 0x00; // PORT D est en mode entree
	
	
	uint8_t couleurR = 1;
	uint8_t couleurNeutre = 0;
	
	unsigned long compteur = 0;
	uint8_t oldValue = 0;
	uint8_t newValue = 0;
	
		
	for(;;){
		PORTB = couleurNeutre;
	
		if ( PIND & 0x04 ) {
			_delay_ms(25);
			if ( PIND & 0x04 ){
				newValue = 1;
			}
		}

		if ( (!(PIND & 0x04)) ) {
			_delay_ms(25);
			if ( (!(PIND & 0x04)) ) {
				oldValue = 0;
			}		
		}

		if ( newValue && (!oldValue) ){
			compteur = compteur + 1;
			_delay_ms(300);
			oldValue = 1;
			newValue = 0;
		}
		
		if ( compteur == 5 ) {
			PORTB = couleurR;
			_delay_ms(1000);
			compteur = 0;
		}
		if ( compteur != 5 ) {
			PORTB = couleurNeutre;
		}
	}
	return 0;
}
