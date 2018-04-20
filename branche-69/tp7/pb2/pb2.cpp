/*
 Fichier: pb2.cpp
 Auteur: Yacer Razouani et Amar Ghaly
 Date: 20 fevrier 2018
 Mise a jour : 28 fevrier 2018 à 10h14
 Description: 
 Ce programme a pour but de résoudre le problème suivant :
    Si la lumière est basse (en cachant la photorésistance), la DEL prendra la couleur verte.
    Si la lumière est à un bon niveau (lumière ambiante), la DEL tournera à l'ambré.
    Si la lumière est trop forte (photorésistance sous une lampe de poche), la DEL devient rouge. 
    [Source : Énoncé TP7 INF1995]
 
 Mode des Ports : (IN)- PORTA, (OUT) - PORTB<1..2> (bit 1 et 2)
*/
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h> 
#include "can.h"

/*
 Fonction:   activateGreen()
 Description:
 Fonction qui allume la lumiere DEL en verte.
 ParamËtres: Aucun
 Retour: Aucun
 */
void activateGreen() {
    PORTB = 1;
}

/*
 Fonction:   activateRed()
 Description:
 Fonction qui allume la lumiere DEL en rouge.
 ParamËtres: Aucun
 Retour: Aucun
 */
void activateRed() {
    PORTB = 2;
}

/*
 Fonction:   activateAmber()
 Description:
 Fonction qui allume la lumiere DEL en ambre.
 ParamËtres: Aucun
 Retour: Aucun
 */
void activateAmber() {
    PORTB = 1;
    _delay_ms(10);
    PORTB = 2;
    _delay_ms(5);
}

/*
 Fonction:   activateNeutral()
 Description:
 Fonction qui eteint la lumiere DEL.
 ParamËtres: Aucun
 Retour: Aucun
 */
void activateNeutral() {
    PORTB = 0;
}

int main(){
    DDRB = 0xff;    												//Le port B est en sortie.
    DDRA = 0x00;													//Le port A est en entree.
    const int VALEUR_BASSE = 500;				
    const int VALEUR_AMBIANTE = 950;
    const int VALEUR_MAX = 1024;
    can detecteur = can() ;
    for(;;){
        
        uint16_t valeur = detecteur.lecture(0);						//On lit la valeur du detecteur.
        
        if (0 <=valeur && valeur <=VALEUR_BASSE){					//Si la valeur est entre 0 et VALEUR_BASSE,
            activateGreen();										//la lumiere DEL s'allume verte.
        }
        
        if (VALEUR_BASSE <=valeur && valeur <=VALEUR_AMBIANTE) {	//Si la valeur est entre VALEUR_BASSE et VALEUR_AMBIANTE,
            activateAmber();										//la lumiere DEL s'allume ambre.
        }
        
        if (VALEUR_AMBIANTE <=valeur && valeur <=VALEUR_MAX){		//Si la valeur est entre VALEUR_AMBIANTE et VALEUR_MAX,
            activateRed();											//la lumiere DEL s'allume rouge.
        }

    }//end for
    return 0;   
}//end main
