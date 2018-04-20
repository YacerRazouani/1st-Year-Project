/*
 * Name: TP3 Problem #2
 * By: Ghaly, Amar && Razouani, Yacer
 * Description: 
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
	uint8_t couleurV = 2;
	uint8_t couleurNeutre = 0;
	//bool activateAmbre = false; 
	//bool activateGreen = false;
	//bool activateRed = false;
	//bool activateNeutral = false;
	bool firstPush = true;
	bool secondPush = false;
	bool thirdPush = false;
	bool originalState = true;
	for(;;){
		
		if (originalState){
			PORTB = couleurR;
			originalState = false;
		}
		
		if (firstPush){
			if ( PIND & 0x04 ){
				_delay_ms(10);
				if ( PIND & 0x04 ){
					//activateAmbre = true;
					PORTB = couleurV;
					_delay_ms(20);
					PORTB = couleurR;
					if ( !(PIND & 0x04) ){
						//activateAmbre = false;
						//activateGreen = true;
						PORTB = couleurV;
						firstPush = false;
						secondPush = true;
					}
				}
			}
		}
	
		if (secondPush){	
			if ( PIND & 0x04 ){
				_delay_ms(10);
				if ( PIND & 0x04 ){
					//activateGreen = false;
					//activateRed = true;
					PORTB = couleurR;
					_delay_ms(300);
					if ( !(PIND & 0x04) ){
						//activateRed = false;
						//activateNeutral = true;
						PORTB = couleurNeutre;
						secondPush = false;
						thirdPush = true;
					}
				}
			}
		}
		
		if (thirdPush){
			if ( PIND & 0x04 ){
				_delay_ms(10);
				if ( PIND & 0x04 ){
					PORTB = couleurV;
					_delay_ms(300);
					originalState = true;
					firstPush = true;
					secondPush = false;
					thirdPush = false;
				}
			}
		}	
							
		//if (activateAmbre){
		//	PORTB = couleurV;
		//	_delay_ms(20);
		//	PORTB = couleurR;
		//}							
		//if (activateGreen){
		//	PORTB = couleurV;	
		//}
		//if (activateRed){
		//	PORTB = couleurR;
		//}
		//if (activateNeutral){
		//	PORTB = couleurNeutre;
		//}
    }
}
		
