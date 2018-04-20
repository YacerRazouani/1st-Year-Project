#include "outin.h"


outin::outin()
{
    DDRA = 0xff;
    DDRB = 0xff;
    DDRC = 0xff;
    DDRD = 0xff;
}

outin::~outin()
{
    //Destructeur 
}

void outin::setOut(char D)
{
    if (D == 'A')
        DDRA = 0xff;
    if (D == 'B')
        DDRB = 0xff;
    if (D == 'C')
        DDRC = 0xff;
    if (D == 'D')
        DDRD = 0xff;
}

void outin::setIn(char D)
{
    if (D == 'A')
        DDRA = 0x00;
    if (D == 'B')
        DDRB = 0x00;
    if (D == 'C')
        DDRC = 0x00;
    if (D == 'D')
        DDRD = 0x00;
    
}
