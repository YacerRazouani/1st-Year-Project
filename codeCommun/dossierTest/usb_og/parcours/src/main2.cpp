#include "lumiereDel.h"
#include "can.h"
#include "roues.h"
#include <avr/io.h> 
#include <avr/interrupt.h>
#include "capteurDistance.h"
#include <son.h>
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

enum state {Default = 1, MurGauchePasLeDroitDeChanger =2 , MurGaucheDroitDeChanger =3, MurDroitPasLeDroitDeChanger=4, MurDroitDroitDeChanger=5};

volatile uint8_t state = Default;

void initialisationMinuterie ( void ) {
    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'etre interrompu alors que
    // le microcontroleur n'est pas pret...
    cli ();
    DDRA = 0x00;

    // sei permet de recevoir a nouveau des interruptions.
    sei ();
}

void partirMinuterie ( uint16_t duree ) {
    TCNT2 = 0;
    
    OCR2A = duree;
    TCCR2A |= _BV(WGM21);
    TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);  
    TIMSK2 |= _BV(OCIE2A); 
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
        //state = MurGauchePasLeDroitDeChanger;
    }
    
    if (valeurCMGauche < 60)
    {
        videGauche = false;
        //state = MurDroitPasLeDroitDeChanger;
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

void ajusterMurGauche()
{
//     cli();
    if(valeurCMGauche >= 13 && valeurCMGauche < 17)
    {
        del.activateGreen();
        moteur.ajustementPWM(100, 100);
    }
    else if(valeurCMGauche < 13)     //trop proche du mur
    {
        del.activateRed();
        moteur.ajustementPWM(100, 80);
    }
    else if(valeurCMGauche >= 17 && valeurCMGauche < 35)     //trop loin du mur
    {
        del.activateRed();
        moteur.ajustementPWM(80,100);
    }
//     sei();
}

void ajusterMurDroit()
{
//     cli();
    if(valeurCMDroit >= 13 && valeurCMDroit < 17)
    {
        del.activateGreen();
        moteur.ajustementPWM(100, 100);
    }
    else if(valeurCMDroit < 13)      //trop proche du mur
    {
        del.activateRed();
        moteur.ajustementPWM(80,100);
    }
    else if(valeurCMDroit >= 17 && valeurCMDroit < 35)      //trop loin du mur
    {
        del.activateRed();
        moteur.ajustementPWM(100, 80);
    }
//     sei();
}

void verifierMurDroit()
{
    cli();
//     if (valeurCMDroit >= 35 && valeurCMDroit <= 60)
//     {
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
        for(uint16_t i = 0; i < 1000; i++)
        {
            _delay_ms(1);
        }   
        
        
        uint8_t distanceAParcourir = (valeurCMDroit - 30);
        uint8_t temps = round((distanceAParcourir * 1000) / 20);    // 15cm/s vitesse
        moteur.ajustementPWM(180, 160);  
        
        for(uint16_t i = 0; i < temps; i++)
        {
            _delay_ms(1);
        }
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
        for(uint16_t i = 0; i < 1000; i++)
        {
            _delay_ms(1);
        }
        state = MurDroitPasLeDroitDeChanger;
//     }
    
    
    
    sei();
}

void verifierMurGauche()
{
    cli();
//     if (valeurCMGauche >= 35 && valeurCMGauche <= 60)
//     {
        del.activateGreen();
        moteur.ajustementPWM(0,0);
        for(uint16_t i = 0; i < 1000; i++)
        {
            _delay_ms(1);
        }
        
        moteur.ajustementPWM(0, 180);
        for(uint16_t i = 0; i < 600; i++)
        {
            _delay_ms(1);
        }
        moteur.ajustementPWM(0,0);
        for(uint16_t i = 0; i < 1000; i++)
        {
            _delay_ms(1);
        }
        
        uint8_t distanceAParcourir = (valeurCMGauche - 30);
        //moteur.ajustementPWM(180, 160);
        uint16_t temps = round((distanceAParcourir * 1000)/ 20);    // 15cm/s vitesse
        moteur.ajustementPWM(180, 160);        
        
        for(uint16_t i = 0; i < temps; i++)
        {
            _delay_ms(1);
        }
        moteur.ajustementPWM(0,0);
        for(uint16_t i = 0; i < 1000; i++)
        {
            _delay_ms(1);
        }
        moteur.ajustementPWM(180, 0);
        for(uint16_t i = 0; i < 850; i++)
        {
            _delay_ms(1);
        }
        moteur.ajustementPWM(0,0);
        for(uint16_t i = 0; i < 1000; i++)
        {
            _delay_ms(1);
        }
        
        state = MurGauchePasLeDroitDeChanger;     
//     }

    sei();
}

int main()
{   

    DDRB = 0xff;    //en sortie
    DDRA = 0x00;    //en entree
    //DDRC = 0xff;    //en sortie
    DDRD = 0xff;    //en sortie
//    initialisationInterrupt();

    
    initialisationMinuterie();
    partirMinuterie(200);
    //moteur.ajustementPWM(120, 120);
    
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
                ajusterMurGauche();
                detecterPoteau();
                if (nouveauMurDroit())
                {
                    state = MurGaucheDroitDeChanger;
                }
                break;
                
            case MurGaucheDroitDeChanger:
                ajusterMurGauche();
                detecterPoteau();
                if(videDroit == false)
                 {
                    if (valeurCMDroit >= 35 && valeurCMDroit <= 60)
                        verifierMurDroit();
                    else
                        state = MurDroitPasLeDroitDeChanger;
                }
                break;
                
            case MurDroitPasLeDroitDeChanger:
                ajusterMurDroit();
                if (nouveauMurGauche())
                {
                    state = MurDroitDroitDeChanger;
                }
                break;
                
            case MurDroitDroitDeChanger:
                ajusterMurDroit();
                if (videGauche == false)
                 {
                    if (valeurCMGauche >= 35 && valeurCMGauche <= 60)
                        verifierMurGauche();
                    else
                        state = MurGauchePasLeDroitDeChanger;
                 }
                break;
        }//end of switch case
    }//end for
        
    return 0;
}
