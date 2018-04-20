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
volatile char mur;
volatile bool debut = true;
volatile uint16_t longueur;
lumiereDel del;
capteurDistance capteur;
roues moteur;
son son;

void initialisationInterrupt ( void ) {
    cli ();
    
    DDRD &= ~(1 << PD2);
    EIMSK |= (1 << INT0);
    EICRA &= ~(1 << ISC00);
    EICRA |= (1 << ISC01);

    sei ();
}

ISR(INT0_vect) {

    _delay_ms(30);
    if(mur == 'g')
    {
        moteur.ajustementPWM(255,0);
        _delay_ms(1200);
    }
    if(mur == 'd')
    {
        moteur.ajustementPWM(0,255);
        _delay_ms(1200);
    }
    moteur.ajustementPWM(200,200);
    _delay_ms(50);
    // Voir la note plus bas pour comprendre cette instruction et son rôle

    EIFR |= (1 << INTF0) ;

}
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

ISR(TIMER2_COMPA_vect) {
    capteur.lireDetecteur();
    //_delay_ms(30);
    capteur.lireDetecteur();
    
    if(valeurGauche < 430.8)
        valeurCMGauche = 0;
    if(valeurDroit < 430.8)
        valeurCMDroit = 0;
    
    valeurGauche = capteur.obtenirCoteGauche();
    valeurCMGauche = round(pow(4187.8/valeurGauche, 1.1060));

    valeurDroit = capteur.obtenirCoteDroit();
    valeurCMDroit = round(pow(4187.8/valeurDroit, 1.1060));
    
    if (valeurCMDroit > 60)
    {
        videDroit = true;
        etaitVideDroit = true;
    }
    if (valeurCMGauche > 60)
    {
        videGauche = true;
        etaitVideGauche = true;
    }
    if (valeurCMDroit <= 60)
    {
        videDroit = false;
    }
    if (valeurCMGauche <= 60)
    {
        videGauche = false;
    }
    
    /*
     * Pour savoir si le robot commence avec un mur a gauche ou bien un mur a droite.
     */
    
    
}

bool nouveauMurDroit()
{
    if(valeurCMDroit <= 60 && etaitVideDroit == true)
    {
        return true;
        etaitVideDroit = false;
    }
    else 
        return false;
}

bool nouveauMurGauche()
{
    if(valeurCMGauche <= 60 && etaitVideGauche ==  true)
    {
        return true;
        etaitVideGauche = false;
    }
    else 
        return false;
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

void ApprocherDuMur(){
    if (mur = 'g' && nouveauMurDroit()){
        del.activateRed();
        moteur.ajustementPWM(180, 0);
        _delay_ms(500);
        moteur.ajustementPWM(140, 140);
        _delay_ms(500);
         moteur.ajustementPWM(0,180);
        _delay_ms(500);
        mur = 'd';
        //_delay_ms(1000);
    }
    if (mur = 'd'&& nouveauMurGauche()){
        del.activateRed();
        moteur.ajustementPWM(0, 180);
        _delay_ms(500);
        moteur.ajustementPWM(140, 140);
        _delay_ms(1000);
         moteur.ajustementPWM(180,0);
         _delay_ms(500);
        mur = 'g';
        //_delay_ms(1000);
    }
}

int main()
{   

    DDRB = 0xff;    //en sortie
    DDRA = 0x00;    //en entree
    //DDRC = 0xff;    //en sortie
    DDRD = 0xff;    //en sortie
    initialisationInterrupt();
//     jouerSon();
//     del.activateGreen();
//     _delay_ms(200);
//     del.activateRed();
//     _delay_ms(200);
    
    initialisationMinuterie();
    partirMinuterie(200);
    moteur.ajustementPWM(120, 120);
    
    if (debut)
    {
        if (valeurCMGauche < valeurCMDroit)
        {
            mur = 'g';
        }
        if (valeurCMDroit < valeurCMGauche)
        {
            mur = 'd';
        }
        debut = false;
    }
    
    for(;;)
    {
        while (mur == 'g')
        {
            detecterPoteau();
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
            if(valeurCMGauche > 17)     //trop loin du mur
            {
                del.activateRed();
                moteur.ajustementPWM(90,120);
            }
           /* if (valeurCMGauche > 30)
            {
                _delay_ms(800);
                if(valeurCMGauche > 30)
                {
                    del.activateRed();
                    moteur.ajustementPWM(0, 255);
                    _delay_ms(400);
                    
                    uint8_t distanceAParcourir = (valeurCMGauche - 15);
                    moteur.ajustementPWM(180, 180);
                    uint8_t temps = distanceAParcourir / 15;    // 15cm/s vitesse
                    uint8_t i;
                    for(i = 0; i < temps; i++)
                    {
                        _delay_ms(1);
                    }
                    moteur.ajustementPWM(0, 255);
                    _delay_ms(400);
                }
            }*/
            if(nouveauMurDroit())
            {
                ApprocherDuMur();
                
                mur = 'd';
                
            }
        }//end while mur gauche
        
        while (mur == 'd')
        {
            detecterPoteau();
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
            if(valeurCMDroit > 17)      //trop loin du mur
            {
                del.activateRed();
                moteur.ajustementPWM(120, 90);
            }
            /*if (valeurCMDroit > 30)
            {
                _delay_ms(800);
                if(valeurCMDroit > 30)
                {
                    del.activateRed();
                    moteur.ajustementPWM(0, 255);
                    _delay_ms(400);
                    
                    uint8_t distanceAParcourir = (valeurCMDroit - 15);
                    moteur.ajustementPWM(180, 180);
                    uint8_t temps = distanceAParcourir / 15;    // 15cm/s vitesse
                    uint8_t i;
                    for(i = 0; i < temps; i++)
                    {
                        _delay_ms(1);
                    }
                    moteur.ajustementPWM(0, 255);
                    _delay_ms(400);
                }
            }*/
            if(nouveauMurGauche())
            {
                ApprocherDuMur();
                
                mur = 'g';
                
            }
        }//end while mur droit
        
        
    }//end for
        
    return 0;
}
