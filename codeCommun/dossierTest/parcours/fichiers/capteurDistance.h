#include "can.h"
#include "lumiereDel.h"
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h> 
class capteurDistance
{
public:
    void lireDetecteur();
    uint16_t obtenirCoteGauche();
    uint16_t obtenirCoteDroit();
    
private:
    can detecteur = can();
    lumiereDel del;
    uint16_t coteGauche_ = 0;
    uint16_t coteDroit_ = 0;
};
