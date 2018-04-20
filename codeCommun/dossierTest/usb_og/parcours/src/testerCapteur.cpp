#include "lumiereDel.h"
#include "can.h"
#include "roues.h"
#include <avr/io.h> 
#include <avr/interrupt.h>
#include "capteurDistance.h"
#include <son.h>
#define F_CPU 8000000UL

volatile uint16_t valeurGauche;
volatile uint16_t valeurDroit;
volatile uint16_t valeurCMGauche;
volatile uint16_t valeurCMDroit;
volatile bool videDroit = false;
volatile bool videGauche = false;
volatile bool etaitVideDroit = false;
volatile bool etaitVideGauche = false;
volatile bool debut = true;
volatile uint16_t longueur;
lumiereDel del;
capteurDistance capteur;
roues moteur;
son son;

enum state {Default = 1, MurGauchePasLeDroitDeChanger = 2, MurGaucheDroitDeChanger = 3, MurDroitPasLeDroitDeChanger = 4, MurDroitDroitDeChanger = 5};

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

void nouveauMurDroit()
{
    if(valeurCMDroit <= 60 && etaitVideDroit == true)
    {
        etaitVideDroit = false;
    }
}

void nouveauMurGauche()
{
    if(valeurCMGauche <= 60 && etaitVideGauche == true)
    {
        etaitVideGauche = false;
    }
}

ISR(TIMER2_COMPA_vect) {
    capteur.lireDetecteur();
    
    /* PLUS PETIT QUE 12 CM ON DIT QUE C'EST EGALE A 0 CM
    if(valeurGauche < 430.8)
        valeurCMGauche = 0;
    if(valeurDroit < 430.8)
        valeurCMDroit = 0;
    */
    /*  TRANSFORMER LES VOLTS EN CENTIMETRES */
    valeurGauche = capteur.obtenirCoteGauche();
    valeurCMGauche = round(pow(4187.8/valeurGauche, 1.1060));
    valeurDroit = capteur.obtenirCoteDroit();
    valeurCMDroit = round(pow(4187.8/valeurDroit, 1.1060));
    
    
    if (valeurCMDroit <= 60)
    {
        videDroit = false;
        //state = MurGauchePasLeDroitDeChanger;
    }
    if (valeurCMGauche <= 60)
    {
        videGauche = false;
        //state = MurDroitPasLeDroitDeChanger;
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
        _delay_ms(500);
        if (videDroit == true)
        {
            jouerSon();
        }
    }
    if (videGauche == false)
    {
        _delay_ms(500);
        if (videGauche == true)
        {
            jouerSon();
        }
    }
}

void ajusterMurGauche()
{
    if(valeurCMGauche >= 13 && valeurCMGauche <= 17)
    {
        del.activateGreen();
        moteur.ajustementPWM(120, 120);
    }
    if(valeurCMGauche < 13)     //trop proche du mur
    {
        del.activateRed();
        moteur.ajustementPWM(120, 90);
    }
    if(valeurCMGauche > 17 && valeurCMGauche < 24)     //trop loin du mur
    {
        del.activateRed();
        moteur.ajustementPWM(105,120);
    }
}

void ajusterMurDroit()
{
    if(valeurCMDroit >= 13 && valeurCMDroit <= 17)
    {
        del.activateGreen();
        moteur.ajustementPWM(120, 120);
    }
    if(valeurCMDroit < 13)      //trop proche du mur
    {
        del.activateRed();
        moteur.ajustementPWM(90,120);
    }
    if(valeurCMDroit > 17 && valeurCMDroit < 24)      //trop loin du mur
    {
        del.activateRed();
        moteur.ajustementPWM(120, 105);
    }
}

void verifierMurDroit()
{
    cli();
    if (valeurCMDroit >= 24 && valeurCMDroit < 60)
    {
        del.activateGreen();
        moteur.ajustementPWM(0,0);
        _delay_ms(500);
        moteur.ajustementPWM(180, 0);
        _delay_ms(500);
                    
        uint8_t distanceAParcourir = (valeurCMDroit - 18);
        moteur.ajustementPWM(180, 180);
        uint8_t temps = distanceAParcourir / 16;    // 15cm/s vitesse
        
        for(uint8_t i = 0; i < temps; i++)
        {
            _delay_ms(1);
        }
        moteur.ajustementPWM(0,0);
        _delay_ms(500);
        moteur.ajustementPWM(0, 180);
        _delay_ms(500);
        
        state = MurDroitPasLeDroitDeChanger;
    }
    sei();
}

void verifierMurGauche()
{
    cli();
    if (valeurCMGauche >= 24 && valeurCMGauche < 60)
    {
        del.activateGreen();
        moteur.ajustementPWM(0,0);
        _delay_ms(500);
        moteur.ajustementPWM(0, 180);
        _delay_ms(500);
                    
        uint8_t distanceAParcourir = (valeurCMGauche - 18);
        moteur.ajustementPWM(180, 180);
        uint8_t temps = distanceAParcourir / 16;    // 15cm/s vitesse
                
        for(uint8_t i = 0; i < temps; i++)
        {
            _delay_ms(30);
        }
        moteur.ajustementPWM(0,0);
        _delay_ms(500);
        moteur.ajustementPWM(180, 0);
        _delay_ms(500);
        
        state = MurGauchePasLeDroitDeChanger;
    }
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
    moteur.ajustementPWM(120, 120);
    
    for(;;)
    {
        
        switch(state)
        {
            case Default:
                if(valeurCMGauche <= valeurCMDroit)
                    state = MurGaucheDroitDeChanger;
                else 
                    state = MurDroitDroitDeChanger;
                break;
            case MurGauchePasLeDroitDeChanger:
                ajusterMurGauche();
                if (valeurCMDroit > 60)
                {
                    videDroit = true;
                    etaitVideDroit = true;
                    state = MurGaucheDroitDeChanger;
                }
                break;
                
            case MurGaucheDroitDeChanger:
                ajusterMurGauche();
                verifierMurDroit();
                if (valeurCMDroit < 24)
                    state = MurDroitPasLeDroitDeChanger;
                //state = MurGauchePasLeDroitDeChanger;
                break;
                
            case MurDroitPasLeDroitDeChanger:
                ajusterMurDroit();
                if (valeurCMGauche > 60)
                {
                    videGauche = true;
                    etaitVideGauche = true;
                    state = MurDroitDroitDeChanger;
                }
                break;
                
            case MurDroitDroitDeChanger:
                ajusterMurDroit();
                verifierMurGauche();
                if (valeurCMGauche < 24)
                    state = MurGauchePasLeDroitDeChanger;
                //state = MurDroitPasLeDroitDeChanger;
                break;
        }
        
        /*
         * 
        while (mur == 'g')
        {
            detecterPoteau();
            nouveauMurDroit();
            switch MurGauche
            {
                if(valeurCMGauche >= 13 && valeurCMGauche <= 17)
                {
                    del.activateGreen();
                    moteur.ajustementPWM(120, 120);
                }
                if(valeurCMGauche < 13)     //trop proche du mur
                {
                    del.activateRed();
                    moteur.ajustementPWM(120, 90);
                }
                if(valeurCMGauche > 17 && valeurCMGauche < 50)     //trop loin du mur
                {
                    del.activateRed();
                    moteur.ajustementPWM(105,120);
                }
            }

        }//end while mur gauche
        
        while (mur == 'd')
        {
            detecterPoteau();
            nouveauMurGauche();
            switch(MurDroit)
            {
                if(valeurCMDroit >= 13 && valeurCMDroit <= 17)
                {
                    del.activateGreen();
                    moteur.ajustementPWM(120, 120);
                }
                if(valeurCMDroit < 13)      //trop proche du mur
                {
                    del.activateRed();
                    moteur.ajustementPWM(90,120);
                }
                if(valeurCMDroit > 17 && valeurCMDroit < 50)      //trop loin du mur
                {
                    del.activateRed();
                    moteur.ajustementPWM(120, 105);
                }
            }
            

        }//end while mur droit
        *
        */
        
        
    }//end for
        
    return 0;
}
