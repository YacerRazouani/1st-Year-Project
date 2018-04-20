#include "lumiereDel.h"

lumiereDel::lumiereDel()
{
    port_ = 'B';
}
lumiereDel::lumiereDel(char port)
{
    if (port == 'a' || port == 'A')
        port_ = 'A';
    if (port == 'b' || port == 'B')
        port_ = 'B';
    if (port == 'c' || port == 'C')
        port_ = 'C';
    if (port == 'd' || port == 'D')
        port_ = 'D';
}

lumiereDel::~lumiereDel()
{
    //Destructeur
}

void lumiereDel::activateRed()
{
    if (port_ == 'A')
        PORTA = 0x02;
    if (port_ == 'B')
        PORTB = 0x02;
    if (port_ == 'C')
        PORTA = 0x02;
    if (port_ == 'D')
        PORTB = 0x02;
}

void lumiereDel::activateGreen()
{
    if (port_ == 'A')
        PORTA = 0x01;
    if (port_ == 'B')
        PORTB = 0x01;
    if (port_ == 'C')
        PORTA = 0x01;
    if (port_ == 'D')
        PORTB = 0x01;
}

void lumiereDel::activateAmbre()
{
    if (port_ == 'A')
        PORTA = 0x01;
        _delay_ms(10);
        PORTA = 0x02;
        _delay_ms(2);
    if (port_ == 'B')
        PORTB = 0x01;
        _delay_ms(10);
        PORTB = 0x02;
        _delay_ms(2);
    if (port_ == 'C')
        PORTC = 0x01;
        _delay_ms(10);
        PORTC = 0x02;
        _delay_ms(2);
    if (port_ == 'D')
        PORTD = 0x01;
        _delay_ms(10);
        PORTD = 0x02;
        _delay_ms(2);
}

void lumiereDel::activateNeutral()
{
    if (port_ == 'A')
        PORTA = 0x00;
    if (port_ == 'B')
        PORTB = 0x00;
    if (port_ == 'C')
        PORTA = 0x00;
    if (port_ == 'D')
        PORTB = 0x00;
    
}
