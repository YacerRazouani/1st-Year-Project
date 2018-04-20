#include <avr/io.h> 
#define F_CPU 8000000UL
#include <util/delay.h>

/*
 *TP3-Probleme1
 * Mazigh Ouanes, 1721035
 * George Louis, 1880098
 * Equipe:72
 * 
 *********************Table d'etat*******************
 ***********Implementation Machine de Moore**********
 *---------------------------------------------------
 *|Etat present | Bouton |Etat suivant |Sortie(LED):|        
 *| Init        |   0    | Init        |0           |   
 *| Init        |   1    |A            |0           |   
 *|   A         |   0    |A            |0           |  
 *|   A         |   1    |B            |0           | 
 *|   B         |   0    |B            |0           | 
 *|   B         |   1    |C            |0           |          
 *|   C         |   0    |C            |0           | 
 *|   C         |   1    |D            |0           |                                  
 *|   D         |   0    |D            |0           |       
 *|   D         |   1    |Rouge        |0           |       
 *|  Rouge      |   X    |Init         |10          |  
 *-------------------------------------------------- */


/* *************************************************************
 * Fonction: boutonAppuyer()
 * Type: Boolean
 * Parametre: n/a
 * Retour: false
 * Role: Assure un antirebond lors de l'appuie de l'interrupteur
 *Verifie si l'interrupteur est toujours appuye 
 ************************************************************* */

bool boutonAppuyer(){
    
    if ( PIND & 0x04 ){
        _delay_ms(10);
        if( PIND & 0x04)
            return true;
        }
    return false; 
}

int main()
{
  DDRB = 0xff; // PORT B est en mode sortie
  DDRD = 0x00; // PORT D est en mode sortie
  uint8_t couleurR = 0b0010; //couleur Rouge
  uint8_t couleurN = 0b0000; //led eteinte
  
  //structure de donnees enum permettant d'enumerer l'ordre des etats du systeme
  enum states {
        Init,
        A,
        B,
        C,
        D,
        Rouge
 };
 //initialise l'etat a Init, l'etat initial
 states state = Init;
 //led en mode eteint
 PORTB = couleurN;

  for(;;)  // boucle sans fin
  {
        
        switch(state) {
            case Init:
            case A:
            case B:
            case C:
            case D:    
                
                
                if ( boutonAppuyer() ) {
                    
                      while ( boutonAppuyer() ) {
                      }
                      
                state = states(state + 1);
                }
                
                break;
                
            case Rouge:
                //Affichage de la couleur rouge pendant une seconde
                PORTB = couleurR;
                _delay_ms(1000);
                PORTB = couleurN;
                //retour etat initial
                state = Init;
                break;
        }        
    }
    
    
   return 0; 
}

