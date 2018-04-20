

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

void activateRed() {
    uint8_t colorRed = 0x01;
    PORTB = colorRed;
}

void activateGreen() {
     uint8_t colorGreen = 0x02;
    PORTB = colorGreen;
}

void activateNeutral() {
    uint8_t colorNeutral = 0;
    PORTB  = colorNeutral;
}

int main(){
	DDRA = 0xff; // PORT A est en mode sortie
	DDRB = 0xff; // PORT B est en mode sortie
	DDRC = 0xff; // PORT C est en mode sortie
	DDRD = 0x00; // PORT D est en mode entree
	
    activateNeutral();
    for(;;) {
        if(userPush() && !(userRelease())){
            activateRed();
            while (true){
                if(!(userPush()) && userRelease()){
                    activateGreen();
                    break;
                }
            }
        }
        if(userPush() && !(userRelease())){
            activateNeutral();
        }
    }
    return 0;
}

