#include <avr/io.h> 
#define F_CPU 8000000UL
#include <util/delay.h>

class bouton
{
public:
    bool userPush();
    bool userRelease();
    
};
