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

	TCCR0B |= (1 << CS02) ;

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
    
    static uint8_t valOCR0A[] = {142, 134, 126, 119, 112, 106, 100, 94, 89, 84, 79, 75, 71, 67, 63, 59, 56, 53, 50, 47, 44, 42, 39, 37, 35, 33, 31, 29, 28, 26, 25, 23, 22, 21, 19, 18, 17};
    
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

