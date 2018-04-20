#include <avr/io.h> 
#define F_CPU 8000000UL
#include <util/delay.h>

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
    DDRA = 0xff; // PORT A est en mode sortie
    DDRB = 0xff; // PORT B est en mode sortie
    DDRC = 0xff; // PORT C est en mode sortie
    DDRD = 0x00; // PORT D est en mode sortie
    
    //uint8_t colorGreen = 0x01;
    uint8_t colorRed = 2;
    uint8_t colorNeutral = 0;
    unsigned long compteur = 0;
    bool was_pressed = 0;
    
    for(;;){
      
        PORTB = colorNeutral;
        if ( userPush() && !was_pressed ){
            compteur++;
            was_pressed = 1;
        } else if (userRelease()) {
            was_pressed = 0;
        }

        if (compteur == 5 ) {
            PORTB = colorRed;
            _delay_ms(1000);
            compteur = 0;
        }
  }
  
  
  return 0;
  
  
}
