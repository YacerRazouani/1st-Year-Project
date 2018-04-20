#define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h>


class lumiereDel
{
public:
    lumiereDel(char p);
    ~lumiereDel();
    void activateRed();
    void activateGreen();
    void activateAmbre();
    void activateNeutral();
private:
    char port_;
};