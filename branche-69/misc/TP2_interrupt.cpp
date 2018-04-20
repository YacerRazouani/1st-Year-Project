#include <avr/io.h> 
#define F_CPU 8000000UL
#include <util/delay.h>

int main()
{
  DDRB = 0xff; // PORT B est en mode sortie
  DDRD = 0x00; // PORT D est en mode entree

  uint8_t couleurVert = 1;
  uint8_t couleurRouge = 2;
  uint8_t couleurNeutre = 0;
  
  for (;;){
    
        while (PIND & 0x04) {
        
        PORTB = couleurVert;
        _delay_ms(20);
        PORTB = couleurRouge;
        _delay_ms(10);
        }
    
    PORTB = couleurNeutre;
    
    
    
    
    
  }
  return 0;
}
