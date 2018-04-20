#define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h>

class roues
{
public:
    void ajustementPWM (uint8_t ratioRoueGauche, uint8_t ratioRoueDroite);
    void ajustementPWMGauche(uint8_t ratioRoueGauche);
    void ajustementPWMDroite (uint8_t ratioRoueDroite);
    void activateReverseGauche(uint8_t ratio);
    void activateReverseDroite(uint8_t ratio);
    void activateForwardGauche(uint8_t ratio);
    void activateForwardDroite(uint8_t ratio);
    void activateNeutral();
    
};
