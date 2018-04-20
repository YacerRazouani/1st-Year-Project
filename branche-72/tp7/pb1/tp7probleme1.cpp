/*
 * Fichier : tp7probleme1.cpp
 * Auteur : Georges Louis & Mazigh Ouanes
 * Date : 1 mars 2018
 */
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Declaration de la variable compteur
volatile uint8_t compteur =0;

/********************************************************************************
 Incrementer le compteur a chaque fois que le timer fait une interruption
 ********************************************************************************/
ISR ( TIMER1_COMPA_vect) {
	compteur++;
}

/*******************************************************************************
 * Fonction pour lancer le timer 1
 ********************************************************************************/
void partirMinuterie( uint16_t duree ){
	compteur= 0;
	
	//mode CTC du timer 1 avec horloge divisee par 1024 interruption apres la duree specifiee
	TCNT1 = 0 ; //compteur = 0
	OCR1A = duree; 
	TCCR1A = 0 ; 
	TCCR1B |=  (1 << WGM12)  |  (1 << CS10)  |  (1 << CS12); // CTC  - divise par 1024
	TCCR1C = 0; 
	TIMSK1 =  (1 <<OCIE1A); 
}

/*******************************************************************************
 * Fonction d'interruption externe (bouton poussoir)
 ********************************************************************************/
ISR ( INT0_vect) {
	
	// anti-rebond
	 if (! (PIND & 0x04) )
	 {
		_delay_ms(10);
   		if (! (PIND & 0x04) )
		 	partirMinuterie(781);	// bouton appuyé --> on lance le compteur.
	 }
	 EIFR |= (1 << INTF0) ;	
}

/*******************************************************************************
 * Fonction pour clignother en vert pendant 0.5 seconde
 ********************************************************************************/
void clignoterV( uint16_t duree){
	
	for (uint8_t i = 0; i < duree / 100 ; i++ ){
	PORTB = 0x01; //couleur rouge
	_delay_ms(50);
	PORTB = 0x00; //eteint
	_delay_ms(50);	
	}
}


/*******************************************************************************
 * Fonction pour clignoter en rouge 2 fois par seconde 
   nombreDeFois fois.
 ********************************************************************************/
void clignoterR (uint8_t nombreDeFois){
	
	for ( uint8_t i = 0; i < nombreDeFois; i++){
		
		for (uint8_t j = 0 ; j < 2 ; j++){
			PORTB = 0x02;
			_delay_ms(250);
			PORTB = 0x00;
			_delay_ms(250);
		}
		
	}
}

/*******************************************************************************
 * Fontion pour activer les interruptions et initialiser les ports.
 ********************************************************************************/
void initialisation ( void ) {
	// cli est une routine qui bloque toutes les interruptions.
	// Il serait bien mauvais d'etre interrompu alors que
	// le microcontroleur n'est pas pret...
	cli ();
	
	// configurer le port B en sortie et le port D en entrée
	DDRB = 0xFF;
    DDRD = 0x00; 
    
	// cette procédure ajuste le registre EIMSK
	// de ATmega324PA pour permettre les interruptions externes
	EIMSK |= _BV(INT0);

	// il faut sensibiliser les interruptions externes aux
	// changements de niveau du bouton-poussoir
	// en ajustant le registre EICRA
	EICRA |= (1 << ISC00);
	
	// sei permet de recevoir a nouveau des interruptions.
	sei ();
}

/*******************************************************************************
 * Fonction pour arrêter  la minuterie et tout mettre a 0.
 ********************************************************************************/
void arreterMinuterie() {
	//on remet tout a 0
     OCR1A = 0;
     TCCR1A = 0;
     TCCR1B = 0;
     TIMSK1 = 0;
}

/*******************************************************************************
 * Fonction boucle qui sera utiliser plus tard dans le main
 ********************************************************************************/
void boucle(){
	initialisation();
	do {
			 // attendre pour que le compteur atteint la valeur 120 ou le bouton soit relaché
	} while (( compteur < 120 && !(PIND & _BV(PORTD2))) || (compteur == 0) );
	arreterMinuterie();
	
	// Une interruption s'est produite. 
	//bloquer toute autre interruptions
	cli ();
	clignoterV(500);	// clignoter en vert pendant 1/2 seconde
	PORTB = 0x00;
	_delay_ms(2000);
    
	clignoterR(compteur/2);	// clignter en couleur rouge compteur/2 fois.
    
	PORTB = 0x01;
	_delay_ms(2000);
	PORTB = 0x00;
}

/*******************************************************************************
 *main fonction
 ********************************************************************************/	
int main()
{	
	while(1){
		boucle();
	}
    return 0;
}


