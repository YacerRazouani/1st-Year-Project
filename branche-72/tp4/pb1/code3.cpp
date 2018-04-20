#include <avr/io.h> 
#define F_CPU 8000000UL
#include <util/delay.h>

/*
 *TP4-Probleme1
 * Mazigh Ouanes, 1721035
 * George Louis, 1880098
 * Equipe:72
 */

uint8_t couleurV = 0b0001;
uint8_t couleurR = 0b0010;
uint8_t couleurN = 0b0000;

bool boutonAppuyer(){
    
    if ( PIND & 0x04 ){
        _delay_ms(10);
        if( PIND & 0x04)
            return true;
        }
    return false; 
}

void delay(int t){
    
    for(int i = 0; i < t; i++){
        
        _delay_us(1);
    }
    
}

int main(){
    
    DDRD = 0x00;
    DDRB = 0xFF;
    
    for(;;){
        if ( PIND & 0x04 ){
            if ( !boutonAppuyer() ){
                int b = 9000;
                for ( int i = 0; i< b; i++){
                    PORTB = couleurN;
                    delay(i*i);
                    PORTB = couleurR;
                    delay(b-(i*i));
                    
                }
            }
        }

        
    
    }
}
