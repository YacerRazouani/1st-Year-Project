#include "roues.h"

void roues::activateReverse()
{
    PORTC = 0x03;
}

void roues::activateForward()
{
	PORTC = 0x02;
}

void roues::activateNeutral()
{
    PORTC = 0x00;
}