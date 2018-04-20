

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h> 

void activateRed(){
	PORTC = 0x02;
}

void activateGreen(){
	PORTC = 0x01;
}

void activateNeutral(){
	PORTC = 0x00;
}
	
int main()
{
	DDRC = 0xff; // PORT C est en mode sortie
	//PORTC = 0x0E ; // binaire 1110
	bool firstPush = true;
    bool secondPush = false;
    int periode = 1000;    
    int valeurA = 1000;
    int valeurB = periode - valeurA;
    int valeurC = 1000;
    int valeurD = periode - valeurC;
    activateNeutral();
	for(;;){

        if(firstPush){
            for(int i = 0; i < valeurA; i++){
                activateRed();
                _delay_us(10);
            }
            for(int i = 0; i < valeurB; i++){
                activateNeutral();
                _delay_us(10);  
            }
            
            valeurA = valeurA - 3;
            valeurB = periode - valeurA;
            
            if(valeurB > 1000){
                firstPush = false;
                secondPush = true;
            }
        }
        
        if(secondPush){
            for(int i = 0; i < valeurC; i++){
                activateGreen();
                _delay_us(10);
            }
            for(int i = 0; i < valeurD; i++){
                activateNeutral();
                _delay_us(10);  
            }
            
            valeurC = valeurC - 3;
            valeurD = periode - valeurC;
        }
        
	}
	return 0;
}
