#define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h>

class roues
{
public:
    void ajustementPWM (uint8_t ratioRoueGauche, uint8_t ratioRoueDroite);
    void activateReverse(char r, uint8_t p);
    void activateForward(char r, uint8_t p);
    void activateBothForward(uint8_t p);
    void activateNeutral();
    
};
