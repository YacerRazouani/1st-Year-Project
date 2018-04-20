#include "memoire_24.h"
#include "lumiereDel.h"
#include <avr/io.h> 
#define F_CPU 8000000UL

int main() {
    /*
     * Ecriture dans la memoire des deux routines
     */
	Memoire24CXXX memory;	
    uint8_t taille;
	uint8_t temporary;
	uint16_t adresse = 0x00;
    lumiereDel del;
    DDRB = 0xff;

    for(;;){
        memory.lecture(adresse++,&temporary);
        memory.lecture(adresse++,&temporary);
        if(temporary == 0)
        {
            del.activateRed();
            _delay_ms(1000);
        }
        if(temporary == 1)
        {
            del.activateGreen();
            _delay_ms(1000);
        }
    }
	return 0;
}
