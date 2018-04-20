#include "lumiereDel.h"
#include "can.h"
#include "roues.h"
#include <avr/io.h> 
#include <avr/interrupt.h>
#include "capteurDistance.h"
#include "son.h"
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
        _delay_ms(1300);
    }
    if(mur == 'd')
    {
        moteur.ajustementPWM(0,255);
        _delay_ms(1300);
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

// bool verification(uint8_t donnee, char coter)
// {
//     if (coter == 'g')
//     {
//         _delay_ms(30);
//         if(donnee == capteur.obtenirCoteGauche())
//         {
//             return true;
//         }
//         else 
//             return false;
//     }
//     if (coter == 'd')
//     {
//         _delay_ms(30);
//         if(donnee == capteur.obtenirCoteDroit())
//         {
//             return true;
//         }
//         else 
//             return false;
//     }
// }

ISR(TIMER2_COMPA_vect) {
    
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
}

bool nouveauMurDroit()
{
    if(valeurCMDroit <= 60 && etaitVideDroit == true)
    {
       /* _delay_ms(1000);
        if(valeurCMDroit < 30 && etaitVideDroit == true){
            return true;
            etaitVideDroit = false;
        }*/
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
        /*_delay_ms(1000);
        if(valeurCMGauche < 30 && etaitVideGauche ==  true){
            return true;
            etaitVideGauche = false;
        }*/
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
        _delay_ms(800);
        if (videDroit == true)
        {
            jouerSon();
        }
    }
    if (videGauche == false)
    {
        _delay_ms(800);
        if (videGauche == true)
        {
            jouerSon();
        }
    }
}

int main()
{   

    DDRB = 0xff;    //en sortie
    DDRA = 0x00;    //en entree
    //DDRC = 0xff;    //en sortie
    DDRD = 0xff;    //en sortie
    //jouerSon();
    del.activateGreen();
    _delay_ms(200);
    del.activateRed();
    _delay_ms(200);
    
    initialisationInterrupt();
    initialisationMinuterie();
    partirMinuterie(200);
    moteur.ajustementPWM(120, 120);
    
    for(;;)
    {
        while (mur == 'g')
        {
            detecterPoteau();
            if(valeurCMGauche >= 13 && valeurCMGauche <= 17)
            {
                del.activateGreen();
                moteur.ajustementPWM(135, 135);
            }
            if(valeurCMGauche < 13)     //trop proche du mur
            {
                del.activateRed();
                moteur.ajustementPWM(135, 120);
            }
            if(valeurCMGauche > 17 && valeurCMGauche <= 25)     //trop loin du mur
            {
                del.activateRed();
                moteur.ajustementPWM(120,135);
            }
            
            if (valeurCMGauche > 25)
            {
                _delay_ms(800);
                if(valeurCMGauche > 25){
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
                    moteur.ajustementPWM(255, 0);
                    _delay_ms(400);
                }
            }
            if(nouveauMurDroit())
            {
                mur = 'd';
            }
            if (videDroit == true && videGauche == true){
            
                //180
                moteur.ajustementPWM(80,150);
                _delay_ms(1500);
                videGauche = false;
            }
        }//end while mur gauche
        
        while (mur == 'd')
        {
            detecterPoteau();
            if(valeurCMDroit >= 13 && valeurCMDroit <= 17)
            {
                del.activateGreen();
                moteur.ajustementPWM(135, 135);
            }
            if(valeurCMDroit < 13)      //trop proche du mur
            {
                del.activateRed();
                moteur.ajustementPWM(120,135);
            }
            if(valeurCMDroit > 17 && valeurCMDroit <=25 )      //trop loin du mur
            {
                del.activateRed();
                moteur.ajustementPWM(135, 120);
            }
            if (valeurCMDroit > 25)
            {
                _delay_ms(800);
                if(valeurCMDroit > 25)
                {
                    del.activateRed();
                    moteur.ajustementPWM(255, 0);
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
            }
            if(nouveauMurGauche())
            {
                mur = 'g';
            }
            if (videDroit == true && videGauche == true){
            
                //180
                moteur.ajustementPWM(150,80);
                _delay_ms(1500);
                videGauche = false;
            
            }
        }//end while mur droit
        
    }//end for
        
    return 0;
}
