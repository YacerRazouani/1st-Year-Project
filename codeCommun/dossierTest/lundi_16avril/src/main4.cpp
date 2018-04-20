#include "lumiereDel.h"
#include "can.h"
#include "roues.h"
#include <avr/io.h> 
#include <avr/interrupt.h>
#include "capteurDistance.h"
#include "son.h"
#define F_CPU 8000000UL

volatile uint16_t valeurGauche = 0;
volatile uint16_t valeurDroit = 0;
volatile uint16_t valeurCMGauche= 0;
volatile uint16_t valeurCMDroit =0;
volatile bool videDroit = false;
volatile bool videGauche = false;
volatile bool etaitVideDroit = false;
volatile bool etaitVideGauche = false;
volatile bool debut = true;
volatile uint16_t longueur= 0;
volatile uint16_t moyenneCMGauche = 0;
volatile uint16_t moyenneCMDroit = 0; 
lumiereDel del;
capteurDistance capteur;
roues moteur;
son son;

enum state {Default, MurGauchePasLeDroitDeChanger, MurGaucheDroitDeChanger, MurDroitPasLeDroitDeChanger, MurDroitDroitDeChanger};

volatile uint8_t state = Default;


void jouerSon()
{
    son.jouerSonorite(70);
    _delay_ms(200);
    son.arreterSon();
    _delay_ms(100);
    son.jouerSonorite(75);
    _delay_ms(200);
    son.arreterSon();
    _delay_ms(100);
    son.jouerSonorite(80);
    _delay_ms(200);
    son.arreterSon();
}

void detecterPoteau()
{
    if (videDroit == false)
    {
        _delay_ms(1000);
        if (videDroit == true)
        {
            jouerSon();
        }
    }
    if (videGauche == false)
    {
        _delay_ms(1000);
        if (videGauche == true)
        {
            jouerSon();
        }
    }
}
bool nouveauMurDroit()
{
    if(valeurCMDroit <= 60 && etaitVideDroit == true && videDroit == false)
    {
        etaitVideDroit = false;
        return true;
    }
    else 
        return false;
}

bool nouveauMurGauche()
{
    if(valeurCMGauche <= 60 && etaitVideGauche == true && videGauche == false)
    {
        etaitVideGauche = false;
        return true;
    }
    else 
        return false;
}

ISR(TIMER2_COMPA_vect) {
    capteur.lireDetecteur();
    
    /*  TRANSFORMER LES VOLTS EN CENTIMETRES */
    valeurGauche = capteur.obtenirCoteGauche();
    valeurCMGauche = round(pow(4187.8/valeurGauche, 1.1060));
    
    valeurDroit = capteur.obtenirCoteDroit();
    valeurCMDroit = round(pow(4187.8/valeurDroit, 1.1060));
    
    
    if (valeurCMDroit < 60)
    {
        videDroit = false;
    }
    
    if (valeurCMGauche < 60)
    {
        videGauche = false;
    }
    
    if(valeurCMDroit >= 60)
    {
        videDroit = true;
        etaitVideDroit = true;
    }
    
    if(valeurCMGauche >= 60)
    {
        videGauche = true;
        etaitVideGauche = true;
    }
    
}


void ajusterMurGauche()
{
    cli();
    if(valeurCMGauche >= 13 && valeurCMGauche < 17)
    {
        del.activateGreen();
        moteur.ajustementPWM(120, 120);
    }
    else if(valeurCMGauche < 13)     //trop proche du mur
    {
        del.activateRed();
        moteur.ajustementPWM(120, 100);	// 100,80
    }
    else if(valeurCMGauche >= 17 && valeurCMGauche < 25)     //trop loin du mur
    {
        del.activateRed();
        moteur.ajustementPWM(100,120);	// 80,100
    }
    sei();
}

void ajusterMurDroit()
{
     cli();
    if(valeurCMDroit >= 13 && valeurCMDroit < 17)
    {
        del.activateGreen();
        moteur.ajustementPWM(120, 120);
    }
    else if(valeurCMDroit < 13)      //trop proche du mur
    {
        del.activateRed();
        moteur.ajustementPWM(100,130);	// 80,110
    }
    else if(valeurCMDroit >= 17 && valeurCMDroit < 25)      //trop loin du mur
    {
        del.activateRed();
        moteur.ajustementPWM(110, 100); // 90,80
    }
     sei();
}

void verifierMurDroit()
{
    cli();
    del.activateGreen();
    moteur.ajustementPWM(0,0);
    for(uint16_t i = 0; i < 1000; i++)
    {
        _delay_ms(1);
    }
        
    moteur.ajustementPWM(180, 0);
    for(uint16_t i = 0; i < 600; i++)
    {
        _delay_ms(1);
    }
    moteur.ajustementPWM(0,0);
    for(uint16_t i = 0; i < 900; i++)
    {
        _delay_ms(1);
    }   

    uint16_t distanceAParcourir = (valeurCMDroit -15);
    uint16_t temps = round((distanceAParcourir * 1500) / 20);    // 15cm/s vitesse
    moteur.ajustementPWM(180, 180);  
        
    for(uint16_t i = 0; i < temps; i++)
    {
        _delay_ms(1);
    }
    moteur.ajustementPWM(0,0);
    for(uint16_t i = 0; i < 1000; i++)
    {
            _delay_ms(1);
    }
    moteur.ajustementPWM(0, 150);
    for(uint16_t i = 0; i < 850; i++)
    {
        _delay_ms(1);
    }
    moteur.ajustementPWM(0,0);
    for(uint16_t i = 0; i < 700; i++)
    {
        _delay_ms(1);
    }
    state = MurDroitPasLeDroitDeChanger;
    sei();
}

void verifierMurGauche()
{
        cli();
        del.activateGreen();
        moteur.ajustementPWM(0,0);
        for(uint16_t i = 0; i < 1000; i++)
        {
            _delay_ms(1);
        }
        
        moteur.ajustementPWM(0, 180);
        for(uint16_t i = 0; i < 850; i++)
        {
            _delay_ms(1);
        }
        moteur.ajustementPWM(0,0);
        for(uint16_t i = 0; i < 900; i++)
        {
            _delay_ms(1);
        }
        
        uint16_t distanceAParcourir = (valeurCMGauche - 10);
        uint16_t temps = round((distanceAParcourir * 1000)/ 20);    // 15cm/s vitesse
        moteur.ajustementPWM(180, 180);       
        
        for(uint16_t i = 0; i < temps; i++)
        {
            _delay_ms(1);
        }
        moteur.ajustementPWM(0,0);
        for(uint16_t i = 0; i < 1000; i++)
        {
            _delay_ms(1);
        }
        moteur.ajustementPWM(150, 0);
        for(uint16_t i = 0; i < 600; i++)
        {
            _delay_ms(1);
        }
        moteur.ajustementPWM(0,0);
        for(uint16_t i = 0; i < 700; i++)
        {
            _delay_ms(1);
        }
        
        state = MurGauchePasLeDroitDeChanger;     
        sei();
}

int main()
{   

    DDRB = 0xff;    //en sortie
    DDRA = 0x00;    //en entree
    DDRD = 0xff;    //en sortie

    
    capteur.initialisationMinuterie();
    capteur.partirMinuterie(200);
    
    for(;;)
    {
        switch(state)
        {
            case Default:
                if(valeurCMGauche < valeurCMDroit)
                    state = MurGaucheDroitDeChanger;
                else if (valeurCMDroit < valeurCMGauche)
                    state = MurDroitDroitDeChanger;
                break;
            case MurGauchePasLeDroitDeChanger:
                detecterPoteau();
                ajusterMurGauche();
                _delay_ms(500);
                if (nouveauMurDroit())
                {
                    state = MurGaucheDroitDeChanger;
                }
                else if (videDroit == true && videGauche == true)
                {
                   	
                   	 moteur.ajustementPWM(100,160);
		    	 _delay_ms(1000);
			ajusterMurDroit();
                }
                break;
                
            case MurGaucheDroitDeChanger:
                detecterPoteau();
                ajusterMurGauche();
                if(videDroit == false)
                 {
                    if (valeurCMDroit >= 25 && valeurCMDroit <= 60)
                        verifierMurDroit();
                    else
                        state = MurDroitPasLeDroitDeChanger;
                }
                else if (videDroit == true && videGauche == true)
                {
		    
                   	 moteur.ajustementPWM(100,160);
		    	 _delay_ms(1000);
			ajusterMurDroit();
                }
                break;
                
            case MurDroitPasLeDroitDeChanger:
                detecterPoteau();
                ajusterMurDroit();
                if (nouveauMurGauche())
                {
                    state = MurDroitDroitDeChanger;
                }
                else if (videDroit == true && videGauche == true)
                {
            
                    moteur.ajustementPWM(160,100);
                    _delay_ms(1000);
                    ajusterMurGauche();
                }
                break;
                
            case MurDroitDroitDeChanger:
                detecterPoteau();
                ajusterMurDroit();
                _delay_ms(500);
                if (videGauche == false)
                {
                    if (valeurCMGauche >= 25 && valeurCMGauche <= 60)
                        verifierMurGauche();
                    else
                        state = MurGauchePasLeDroitDeChanger;
                }
                else if (videDroit == true && videGauche == true)
                {
      
                    	moteur.ajustementPWM(160,100);
		    	_delay_ms(1000);
			ajusterMurGauche();
                }
                break;
        }//end of switch case
    }//end for
        
    return 0;
}

