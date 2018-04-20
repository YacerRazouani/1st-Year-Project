/*
 Fichier: pb2_versionFinale.cpp
 Auteur: Amar Ghaly et Yacer Razouani
 Date: 23 janvier 2018
 Mise a jour : 24 janvier 2018 à 18h47
 Description: 
 Ce programme a pour but de résoudre le problème suivant :
     Quand la carte mère démarre, la DEL libre doit s'allumer en rouge. Si le bouton-poussoir noir est
     pesé, la DEL affiche la couleur ambre. Quand le bouton-poussoir est relâché, la DEL devient verte. Si
     le bouton est de nouveau pesé, la DEL prend la couleur rouge encore. Quand il est relâché, la DEL
     s'éteint. Si le bouton est de nouveau pesé, la DEL affiche la couleur verte. Quand il est relâché, la
     DEL tourne au rouge ce qui fait que la carte mère est de retour à son état initial et tout peut
     recommencer. [Source : Énoncé TP3 INF1995]
 
 Mode des Ports : (IN)- PORTD, (OUT) - PORTB<1..2> (bit 1 et 2)
 
 
                        Table 1 : Tableau des états
 
                Etat present    A   Etat suivant    Sortie Z
 
                    INIT        0       INIT            1 (ROUGE)
 
                    INIT        1        E1             1 (AMBRE)
 
                    E1          1        E1             1 (AMBRE)
 
                    E1          0        E2             1 (VERT)
 
                    E2          0        E2             1 (VERT)
 
                    E2          1        E3             1 (ROUGE)
 
                    E3          1        E3             1 (ROUGE)
 
                    E3          0        E4             1 (AUCUNE COULEUR)
 
                    E4          0        E4             0 (AUCUNE COULEUR)
 
                    E4          1        E5             0 (VERT)
 
                    E5          1        E5             1 (VERT)
 
                    E5          0       INIT            1 (ROUGE)


*/

#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>




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
        _delay_ms(10);
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
        _delay_ms(10);
    if( !(PIND & 0x04) ) {
            release = true;
        }
    }
    return release;
}




int main(){
	DDRB = 0xff; // PORT B est en mode sortie
	DDRD = 0x00; // PORT D est en mode entree

	uint8_t couleurR = 2;
	uint8_t couleurV = 1;
	uint8_t couleurNeutre = 0;
    
    unsigned long state = 0;
    for(;;){
        switch (state){
            // Etat INIT, couleur Rouge de la DEL au demarrage de la carte-mère
            case 0: { 
                PORTB = couleurR;
                if ( userPush()){
                    state++;
                }
            } 
            break;
                
            // Etat E1, couleur Ambre de la DEL lorsque le bouton-poussoir est appuye
            case 1: { 
                PORTB = couleurV;
                _delay_ms(10);
                PORTB = couleurR;
                if ( !(userPush()) && userRelease() ){
                    state++;
                }
            } 
            break;
            
            // Etat E2, couleur Verte de la DEL lorsque le bouton-poussoir est relache
            case 2: {
                PORTB = couleurV;
                if (userPush()){
                    state++;
                }
            } 
            break;
                
            // Etat E3, couleur Rouge de la DEL lorsque le bouton-poussoir est appuye
            case 3: {
                if ( userPush() && !(userRelease()) ){
                    PORTB = couleurR;
                    state++;
                }
                break;
            }
            
            // Etat E4, aucune couleur de la DEL lorsque le bouton-poussoir est relache
            case 4: {
                if ( !(userPush()) && userRelease() ){
                    PORTB = couleurNeutre;
                    state++;
                }
                break;
            }
            // Etat E5, aucune couleur de la DEL lorsque le bouton-poussoir est relache
            case 5: {
                if ( userPush() && !(userRelease()) ){
                    PORTB = couleurV;
                    state++;
                }
                break;
            }
            
            //Case permettant le retour à l'etat INIT
            case 6: {
                if ( !(userPush()) && userRelease() ) {
                    state = 0;
                }
                break;
            }
            
        }//end switch
    }//end for
    return 0;
}// end main
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
    
