#include <avr/io.h> 
#define F_CPU 8000000UL
#include <util/delay.h>

class outin
{
public:
    outin();
    ~outin();
    void setOut(char D);
    void setIn(char D);
    
};
