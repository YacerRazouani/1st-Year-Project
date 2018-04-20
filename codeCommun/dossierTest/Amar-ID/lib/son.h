

#include <avr/io.h> 
#define F_CPU 8000000UL
#include <util/delay.h>

class son{
public:

son();
~son();
void ajustementPWMSon (uint8_t note);
void initialisationSon();
void jouerSonorite(uint8_t note);
void arreterSon();

};
