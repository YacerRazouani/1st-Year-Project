/*
 * Nom: compteur 32 bits
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: Ceci est un exemple simple de programme 
 * Version: 1.1
 */


#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>

int main()
{
  DDRA = 0xff; // PORT A est en mode sortie
  DDRB = 0xff; // PORT B est en mode sortie
  DDRC = 0xff; // PORT C est en mode sortie
  DDRD = 0x00; // PORT D est en mode sortie
  //unsigned long compteur=0; // le compteur est initialise a 0.
                            // c'est un compteur de 32 bits
  


  for(;;)  // boucle sans fin
  {
   //compteur++;// incremente le compteur
                 // si le compteur est 0xffff ffff il revient a 0
   //PORTD = compteur;       // PORTD = 8 bits de large,
                            // il prend les bits de 0 a 7 du compteur
   //PORTC = compteur >> 8;  // PORTC = 8 bits de large,
                            // il prend les bits de 8 a 15 du compteur
                            // en faisant un decallage de 8 bits.
   //PORTB = compteur >> 16; // PORTB = 8 bits de large,
                            // il prend les bits de 16 a 23 du compteur
                            // en faisant un decallage de 16 bits.
   //PORTA = compteur >> 24; // PORTA = 8 bits de large,
                            // il prend les bits de 24 a 31 du compteur
                            // en faisant un decallage de 24 bits.		
		
		
		//uint8_t  couleurV = 1;   //POUR EXERCICE 1 DU TP2
	    //uint8_t couleurR = 2;
	    //uint8_t couleurNeutre = 0;
	    
	    //_delay_ms(500);
		//PORTB = couleurV;
		//_delay_ms(500);
		//PORTB = couleurR;
		//_delay_ms(500);
		
		//for(uint8_t i = 0; i < 68; i++){
		//	PORTB = couleurV;
		//	_delay_ms(10);
		//	PORTB = couleurR;
		//	_delay_ms(10);
		//}
		//PORTB = couleurNeutre;
		
		uint8_t couleurV = 1;			//POUR EXERCICE 2 DU TP2
		uint8_t couleurR = 2;
		uint8_t couleurNeutre = 0;
		if ( PIND & 0x04 ) {
			PORTB = couleurR;
			PORTB = couleurV;
		}
		else
			PORTB = couleurNeutre;
		
		
  }
  return 0; 
}


