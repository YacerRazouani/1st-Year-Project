#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include <son.h>


son::son()
{
    // Constructeur
};

son::~son()
{
    // Destructeur
};


/***********************************************************************
 * Fonction pour generer le son en utilisant du pwm
 **********************************************************************/
 void son::ajustementPWMSon(uint8_t note) 
 {

	// toggle on compare match and mode CTC for the timer
	TCCR0A |= (1 << WGM01) | (1 << COM0A0) ;
	
	OCR0A = note;

	// division d'horloge par 1024 - implique une frequence de PWM fixe

	TCCR0B |= (1 << CS02) | (1 << CS00);

}

/***********************************************************************
 * Initialisation du portD en sortie pour le son 
 **********************************************************************/
void son::initialisationSon() 
{

	DDRB = 0xff; // PORT B est en mode sortie
	
	// Mis a 0 de la pin 7 (0 volt)
	
	//PORTD &= ~(1 << 6);

}

/***********************************************************************
 * fonction qui joue une sonorite en utlisant l'indice de la note passee
 *  en parametre
 **********************************************************************/
void son::jouerSonorite(uint8_t note)
{   
    initialisationSon();
    
    static uint8_t valOCR0A[] = {37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    
	if((note >= 45) && (note <= 81))
		ajustementPWMSon(valOCR0A[note-45]);
		
}

/***********************************************************************
 * Fonction pour arreter toute sonorite
 **********************************************************************/
void son::arreterSon()
{
	TCCR0A = (0 << COM0A0);
}

