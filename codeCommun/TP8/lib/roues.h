#include <avr/io.h> 
#define F_CPU 8000000UL

class roues
{
public:
    void activateReverse();
    void activateForward();
    void activateNeutral();
};