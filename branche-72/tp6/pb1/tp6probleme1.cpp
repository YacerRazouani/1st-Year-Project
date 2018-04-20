/*
 * Nom: compteur 32 bits
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: Ceci est un exemple simple de programme 
 * Version: 1.1
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <memoire_24.h>

uint8_t phraseAEcrire[] = "*E*C*O*L*E* *P*O*L*Y*T*E*C*H*N*I*Q*U*E*";
uint8_t phraseLue[] = "";
uint8_t taille_tableau = sizeof(phraseAEcrire);


//Definiton des constantes
const uint8_t PIN_BUTTON = 0x04;
const uint8_t SORTIE = 0xFF;
const uint8_t ENTREE = 0x00;
const uint8_t ROUGE = 0x02;
const uint8_t VERT = 0x01;
const uint8_t ETEINT = 0x00;
const uint8_t DELAI_1 = 8;
const uint8_t DELAI_2 = 12;
Memoire24CXXX maMemoire;
uint8_t b;



    
    
 


/*******************************************************************************
*
*******************************************************************************/
void setup()
{
	DDRB = SORTIE; // Port B en mode sortie
	DDRD = ENTREE; // Port D en mode entrée
	PORTB = ETEINT; // LED allume rouge
	
}
void ecrireEtLire(){
	
	maMemoire.ecriture( 0x00,phraseAEcrire,taille_tableau);
	_delay_ms(5);
	maMemoire.lecture( 0x00,phraseLue,taille_tableau);
}
void verifier(){
	
	for (uint8_t i = 0; i < taille_tableau-1; i++){
		
		if (phraseAEcrire[i]!=phraseLue[i]){
			PORTB = ROUGE;
			_delay_ms(1000);
		}
			
		else 
			PORTB = VERT;
			_delay_ms(500);
			PORTB =ETEINT;
			_delay_ms(100);
					
	}
}


int main()
{
	setup();
	ecrireEtLire(); 
	verifier();   
    return 0;
}

