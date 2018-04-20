#include "memoire_24.h"
#include "lumiereDel.h"

int main() {
    /*
     * Ecriture dans la memoire 
     */
	Memoire24CXXX memory;	
	uint8_t routine = 0x00;
// 	uint16_t adresse = 0x00;
    memory.lecture(0x00, (uint8_t*)&routine);
    if (routine == 0x00)
        memory.ecriture(0x00, 0x01);
    else
        memory.ecriture(0x00, 0x00);
    
    
    if (routine == 0x00){
        del.activateRed();
        _delay_ms(1000);
    }
    if (routine == 0x01){
        del.activateGreen();
        _delay_ms(1000);
    }
    
	return 0;
}
