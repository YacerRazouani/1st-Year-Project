#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h> 
#include <avr/interrupt.h>: Interrupts
#include "memoire_24.cpp"

void activateNeutral(){
    PORTA = 0;
}

void activateGreen(){
    PORTA = 1;
}

void activateRed(){
    PORTA = 2;
}

int main()
{   
    DDRA = 0xff;
    activateNeutral();    
    uint16_t adresse = 0x00;
    uint8_t donnee[] = "*E*C*O*L*E* *P*O*L*Y*T*E*C*H*N*I*Q*U*E*\n";
    Memoire24CXXX mem = Memoire24CXXX();
    mem.init();
    mem.ecriture(adresse, donnee, 40);
    _delay_ms(10);
    
    uint8_t lu[] = {};
    mem.lecture(adresse, lu, 40);
    
    int compareStatus = 0;
    
    for(uint8_t i = 0; i < 40; i++){
    
        if (lu[i] != donnee[i]){
            compareStatus = 1; 
        }
        else {
            compareStatus = 2;
        }
    }
    
    if (compareStatus == 1){
        for(;;)
            activateRed();
    }
    if (compareStatus == 2){
        for(;;)
            activateGreen();
    }
    
    return 0;
}
