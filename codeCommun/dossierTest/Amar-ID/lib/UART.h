
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h> 


class UART
{
public:
    void initialisationUART ( void );
    void transmissionUART ( uint8_t donnee );
    unsigned char lireUART ();
};
