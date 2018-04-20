
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h> 

void activateReverse(){                  //reculer la roue par en arriere.
	PORTC = 0x03;
}

void activateForward(){                   //avancer la roue par en avant.
	PORTC = 0x02;
}

void activateNeutral(){
    PORTC = 0x00;
}
	
int main()
{
	DDRC = 0xff; // PORT C est en mode sortie
	//PORTC = 0x0E ; // binaire 1110
    int periode1 = 16667;       //periode de 60Hz ou 1/60s.   
    int periode2 = 2500;        //periode de 400Hz ou 1/400s.
    bool firstPeriod = true;
    bool secondPeriod = false;
    int valeurA = 0;
    int valeurB = periode1 - valeurA;

	for(;;){
        if (firstPeriod){
            for(int i = 0; i < valeurA; i++){
                activateForward();
                _delay_us(120);
            }
            for(int i = 0; i < valeurB; i++){
                activateNeutral();
                _delay_us(120);  
            }
            
            valeurA += periode1/4;
            
            if(valeurA > (periode1 + 100)){
                valeurA = 0;
                firstPeriod = false; 
                secondPeriod = true;
                valeurB = periode2 - valeurA;
            }
            valeurB = periode1 - valeurA;
           
        }
        
        if (secondPeriod){
            for(int i = 0; i < valeurA; i++){
                activateForward();
                _delay_us(800);
            }
            for(int i = 0; i < valeurB; i++){
                activateNeutral();
                _delay_us(800);  
            }
            
            valeurA += periode2/4;
            
            if(valeurA > (periode2 + 100)){
                valeurA = 0;
            }
            valeurB = periode2 - valeurA;
           
        }
        
        
        
	}
	return 0;
}
