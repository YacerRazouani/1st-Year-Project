#include "bouton.h"

bool bouton::userPush()
{
    bool push = false;
    if (PIND & 0x04)
    {
        _delay_ms(20);
        if (PIND & 0x04)
        {
            push = true;
        }
    }
    return push;
}

bool bouton::userRelease()
{
    bool release = false;
    if (!(PIND & 0x04))
    {
        _delay_ms(20);
        if (!(PIND & 0x04))
        {
            release = true;
        }
    }
    return release;
    
}
