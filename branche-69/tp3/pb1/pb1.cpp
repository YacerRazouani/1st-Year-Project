#include <avr/io.h> 
#define F_CPU 8000000UL
#include <util/delay.h>
/*
 Fichier: pb1.cpp
 Auteur: Amar Ghaly et Yacer Razouani
 Date: 23 janvier 2018
 Mise a jour : 24 janvier 2018 à 18h47
 Description:
 Ce programme a pour but de résoudre le problème suivant :
    On veut ici simplement que la DEL soit éteinte au départ. On doit appuyer et relâcher 5 fois le
    bouton-poussoir avant que la DEL tourne au rouge pendant exactement 1 seconde. Par la suite, on
    revient au départ pour pouvoir recommencer.
 
 
 Mode des Ports : (IN)- PORTD, (OUT)- PORTB<1..2> (bit 1 et 2)
 
 
             Table 1 : Tableau des états
             
             Etat present    A   Etat suivant    Sortie Z
             
             INIT        0       INIT            0 (AUCUNE COULEUR)
             
             INIT        1        E1             0 (AUCUNE COULEUR)
             
             E1          0        E1             0 (AUCUNE COULEUR)
             
             E1          1        E2             0 (AUCUNE COULEUR)
             
             E2          0        E2             0 (AUCUNE COULEUR)
             
             E2          1        E3             0 (AUCUNE COULEUR)
             
             E3          0        E3             0 (AUCUNE COULEUR)
             
             E3          1        E4             0 (AUCUNE COULEUR)
             
             E4          0        INIT           1 (ROUGE)
             
             E4          1        INIT           0 (AUCUNE COULEUR)

 
 */


/*
 Fonction:   userPush()
 Description:
 Fonction qui prévoit le mécanisme antirebond pour éviter que le microcontroleur interprète plusieurs
 transitions sur l'interrupteur comme autant de changements de signal lorsque l'usager appuie sur le
 bouton-poussoir.
 ParamËtres: Aucun
 Retour: la valeur de la variable (bool) push
 */

bool userPush() {
    bool push = false;
	if ( PIND & 0x04 ) {
        _delay_ms(20);
        if ( PIND & 0x04 ){
            push = true;
        }
    }
	return push;
}
/*
 Fonction:   userRelease()
 Description:
 Fonction qui prévoit le mécanisme antirebond pour éviter que le microcontroleur interprète plusieurs
 transitions sur l'interrupteur comme autant de changements de signal lorsque l'usager relache le
 bouton-poussoir.
 ParamËtres: Aucun
 Retour: la valeur de la variable (bool) release
 */
bool userRelease() {
    bool release = false;
    if ( !(PIND & 0x04) ) {
        _delay_ms(20);
        if( !(PIND & 0x04) ) {
            release = true;
        }
    }
    return release;
}



int main()
{
    DDRB = 0xff; // PORT B est en mode sortie
    DDRD = 0x00; // PORT D est en mode entree
    
    uint8_t colorRed = 2;
    uint8_t colorNeutral = 0;
    unsigned long state = 0;
    bool was_pressed = 0;
    
    for(;;){
      
        // AUCUNE COULEUR de la DEL au demarrage de la carte-mère
        PORTB = colorNeutral;
        if ( userPush() && !was_pressed ){
            state++;
            was_pressed = 1;
        } else if (userRelease()) {
            was_pressed = 0;
        }

        
        switch (state) {
            // Etat INIT, AUCUNE COULEUR de la DEL lorsque l'usager presse le bouton pour la 1er fois
            case 1 : 
            // Etat E1, AUCUNE COULEUR de la DEL lorsque l'usager presse le bouton pour la 2e fois
            case 2 :
            // Etat E2, AUCUNE COULEUR de la DEL lorsque l'usager presse le bouton pour la 3e fois
            case 3 :
            // Etat E3, AUCUNE COULEUR de la DEL lorsque l'usager presse le bouton pour la 4e fois
            case 4 :{
                if ( userPush() && !was_pressed ){
                    state++;
                    was_pressed = 1;
                } 
                else if (userRelease()) {
                    was_pressed = 0;
                }
                break;
            }
            // Etat E4,COULEUR ROUGE de la DEL lorsque l'usager relache le bouton
            case 5 : {
                if ( !(userPush()) && userRelease() ){
                    PORTB = colorRed;
                    _delay_ms(1000);
                    state = 0;
                }
                break;
            }
        }//end switch
        

  }//end for
  
  
  return 0;
  
  
}//end main
