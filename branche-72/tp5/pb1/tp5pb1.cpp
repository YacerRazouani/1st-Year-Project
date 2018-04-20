/* Tableau des états 

    |Etats Present |Bouton|Etats Suivant | LED         |
	|init          |   0  |init          |Rouge        |
	|init          |   1  |e1            |ambre        |
	|e1            |   0  |e2            |verte        |
	|e2            |   1  |e3            |rouge        |
	|e3            |   0  |e4            |eteint       |
	|e4            |   1  |e5            |verte        |
	|e5            |   0  |init          |rouge        |
	
	*/
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Definiton des constantes
const uint8_t PIN_BUTTON = 0x04;
const uint8_t SORTIE = 0xFF;
const uint8_t ENTREE = 0x00;
const uint8_t ROUGE = 0x02;
const uint8_t VERT = 0x01;
const uint8_t ETEINT = 0x00;
const uint8_t DELAI_1 = 8;
const uint8_t DELAI_2 = 20;

// Definitions des etats
enum state {init = 0,e1,e2,e3,e4,e5};

//Declarations des variables
volatile state etat=init;
volatile bool boutonPresse = false;

    
    
 


/*******************************************************************************
*
*******************************************************************************/
void setup()
{
	DDRB = SORTIE; // Port B en mode sortie
	DDRD = ENTREE; // Port D en mode entrée
	PORTB = ROUGE; // LED allume rouge
	
}

/*******************************************************************************
*
*******************************************************************************/


 ISR (INT0_vect) {
	// laisser un delai avant de confirmer la reponse du
	// bouton-poussoir: environ 30 ms (anti-rebond)
	_delay_ms ( 30 );

	// Se souvenir ici si le bouton est presse ou relache


	// changements d'etats tels que ceux de la
	// semaine precedente
	switch (etat) {
		
		case init : 
					if (PIND & PIN_BUTTON) 
						etat = e1;
					break;	    
		case e1 : if (!(PIND & PIN_BUTTON)) 
					   etat= e2;
					break;		
		case e2 : if (PIND & PIN_BUTTON) 
					 etat= e3;
					break;
		case e3 : if (!(PIND & PIN_BUTTON)) 
						etat= e4;
				  break;
		case e4 : if (PIND & PIN_BUTTON) 
					  etat= e5;
				  break;
				
		case e5 : if (!(PIND & PIN_BUTTON)) 
					 etat= init;
				  break;
		}

	// Voir la note plus bas pour comprendre cette instruction et son role
	EIFR |= (1 << INTF0) ;
}

void initialisation ( void ) {
	// cli est une routine qui bloque toutes les interruptions.
	// Il serait bien mauvais d'etre interrompu alors que
	// le microcontroleur n'est pas pret...

	cli ();

	// configurer et choisir les ports pour les entrees
	// et les sorties. DDRx... Initialisez bien vos variables

	setup();

	// cette procédure ajuste le registre EIMSK
	// de ATmega324PA pour permettre les interruptions externes

	EIMSK |= _BV(INT0);

	// il faut sensibiliser les interruptions externes aux
	// changements de niveau du bouton-poussoir
	// en ajustant le registre EICRA

	EICRA |= 1 << ISC00;

	// sei permet de recevoir a nouveau des interruptions.
	sei ();

}


/*******************************************************************************
* 
*******************************************************************************/

/*******************************************************************************
*  
*******************************************************************************/
void loop()
{
	switch (etat) {
	
	case init : 
				PORTB = VERT;
				_delay_ms(DELAI_1);
				PORTB=ROUGE;
				_delay_ms(DELAI_2);
				break;	    
	case e1 :	PORTB = VERT;
				break;		
	case e2 : 	PORTB = ROUGE;
				break;
	case e3 : 	PORTB = ETEINT;
				break;
	case e4 : 	PORTB = VERT;
			    break;    
	case e5 : 	PORTB=ROUGE;
				break;
	}
}

int main()
{
	initialisation ();
    for(;;) // boucle infinie
    {
			loop();
	}
        
    return 0;
}

