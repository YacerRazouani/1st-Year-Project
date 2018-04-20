#include <avr/io.h> 
#define F_CPU 8000000UL
#include <util/delay.h>

/*TP3-Probleme2
 * Mazigh Ouanes, 1721035
 * George Louis, 1880098
 * Equipe:72
 * *********************Table d'etat*********************
 * *********Implementation Machine de Meanly*************
 *|Etat present: | Bouton: |Etat suivant: |Sortie(LED): |
 *-------------------------------------------------------
 *| Init         |   0     |Init          |Rouge        |   
 *| Init         |   1     |A             |Ambre        |   
 *| A            |   0     |A             |Vert         |
 *| A            |   1     |B             |Rouge        |
 *| B            |   0     |B             |Eteint       |
 *| B            |   1     |Init          |Vert         |
 *------------------------------------------------------- */

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
 
  uint8_t couleurV = 0b0001;
  uint8_t couleurR = 0b0010;
  uint8_t couleurN = 0b0000;
  
  //structure de donnees enum permettant d'enumerer l'ordre des etats du systeme
  enum states {
        Init,
        A,
        B,
  };

  //initialise l'etat a Init, l'etat initial
  states state = Init;

  for(;;)  // boucle sans fin
  {
      switch(state) {
          
          //Premiere etat
          case Init:
            
              PORTB = couleurR;
                
              while ( PIND & 0x04 ) {
                  PORTB = couleurR;
                  _delay_ms(5);
                  PORTB = couleurV;
                  _delay_ms(10);
                  
                  if ( !boutonAppuyer() ){
                    state = states(state + 1);
                    break;
                  }
              }
              break;
              
                        
          //Deuxieme etat
          case A:
              
              PORTB = couleurV;
              
              while ( PIND & 0x04 ) {
                  PORTB = couleurR;
                  
                  if (!boutonAppuyer()) {
                      state = states(state + 1);
                      break;
                  }
              }
              break;
          
          //Troisieme etat
          case B:
              
              PORTB = couleurN;
              while ( PIND & 0x04 ) {
                  PORTB = couleurV;
                  
                  if (!boutonAppuyer()) {
                      state = states(state - 2);   // il faut que sa revient a Init (rouge)
                      break;
                  }
              }
              break;     
          }

      
  }

  return 0;
}

