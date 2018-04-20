#include <avr/io.h> 
#define F_CPU 8000000UL
#include <util/delay.h>

/*
 *TP4-Probleme2
 * Mazigh Ouanes, 1721035
 * George Louis, 1880098
 * Equipe:72
 */

void PWM (int nDelais);
void Move(int f);

DDRC = 0x03; // PORT B est en mode sortie
DDRD = 0x00; // PORT D est en mode sortie

bool boutonAppuyer(){
    
    if ( PIND & 0x04 ){
        _delay_ms(10);
        if( PIND & 0x04)
            return true;
        }
    return false; 
}

enum states{
    OFF,
    ON

void delay(int t){
    for ( i = 0; i < t; i++){
        _delay_us(1);
    }
}


int main(){
    
    DDRA = 0x03;
    int f  =60;
    Move(f);
    Move(f);
    
    return 0;

}

void PWM(int nDelais){
    for (int i = 0; i< nDelais; i++)
        _delay_ms(1);
}

void Move(int f){
    
    double a = 0, b, c;
    if ( f== 60){
        b = 16.67;
        c = 600;
    }
    if (f == 400){
        b = 2.5;
        c = 4000;
    }
    uint8_t AVANT = 0x01;
    uint8_t DERIERE = 0x00;
    for ( int i = 0; i <= c; i++){
        PORTA = AVANT;
        PWM(a);
        PORTA = DERIERE;
        PWM(b);
        if (!(i % 120) && (f == 60)) {
            a += 4.17;
            b -= 4.17;
        }
        if (!(i % 800) && (f == 400)){
            a += 0.625;
            b -= 0.625;
        }
    }
}

