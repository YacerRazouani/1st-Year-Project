/*
 * Fichier : tp7probleme2.cpp
 * Auteur : Georges Louis & Mazigh Ouanes
 * Date : 1 mars 2018
 */
#include "can.cpp"
#include <avr/io.h> 
#define F_CPU 8000000UL // 1 MHz
#include <util/delay.h>

int main()
{ 
  
  DDRA = 0x00; //entree 
  DDRB = 0xff; //sortie
  DDRC = 0x00; //entree
  DDRD = 0x00; //entree
  
  enum etats {rouge, vert, ambre};
  can convertir;
  const  uint16_t puissanceMaximale = 255 ;			//puissance à 100%
  const  uint16_t puissanceMoyenne = 205 ;		//puissance à 80%
  const  uint16_t puissanceMinimale = 77 ;			//puissance à 30%
  
  etats etat;
  
  for (;;)
  {
  uint16_t puissance = convertir.lecture(0) ;
  puissance = puissance >> 0x2;
  
  if (puissance >= 0 && puissance < puissanceMinimale )
    etat = vert;
    
  if (puissance >= 77 && puissance < puissanceMoyenne )
    etat = ambre;
  
  if (puissance >= puissanceMoyenne && puissance < puissanceMaximale)
     etat = rouge;
   
  switch (etat)
    {
  
     case rouge:					//puissance > 80%
			  PORTB = 0x02;		//allume rouge
			  break;
			  
  
    case vert:				//puissance < 30%
				PORTB = 0x01;	//allume vert
				break;
    
    case ambre:					//puissance < 80%
				for (uint16_t i = 0; i<20 ; i++)		//boucle pour la couleur ambrée
				{
					PORTB = 0x02;
					_delay_ms(10);
					PORTB = 0x01;
					_delay_ms(10);
				 }
			  break;
	
    default:
			  break;
    }
  }
    
  return 0;
}
