//////////////////////////////////////////////////////////////////////////////
//
// Programme permettant de generer les vecteurs de
// test pour la mise au point du pont en H.
// 
// Les sorties 1, 2 et 3 du port C sont constantes a 5V, les 
// sorties 0, 4, 5, 6 et 7 sont constantes a 0V.
//
// Laurent Tremblay et Jerome Collin
// Juillet 2008
//  
//////////////////////////////////////////////////////////////////////////////

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h> 
void activateRed(){
	PORTC = 0x02;
}

void activateNeutral(){
	PORTC = 0x00;
}
	
int main()
{
	DDRC = 0xff; // PORT C est en mode sortie
	//PORTC = 0x0E ; // binaire 1110
    int valeur = 20;

	for(;;){

        for(double i = 0; i < valeur; i++){
            activateRed();
            _delay_ms(2.5);
        }
        
        valeur = valeur - (0.01);
        activateNeutral();
        _delay_ms(10);

        
	}
	return 0; 
}
