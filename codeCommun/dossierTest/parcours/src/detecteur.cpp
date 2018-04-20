#include "lumiereDel.h"
#include "can.h"
#include "roues.h" 
//#include "capteurDistance.h"
#include <avr/io.h> 
#include <avr/interrupt.h>

//OBJETS GLOBAUX
roues roues;
lumiereDel del;
//capteurDistance capteur;
can detecteur = can()
//VALEURS GLOBALES
volatile uint16_t coteGauche = 0;
volatile uint16_t coteDroit = 0;
volatile bool videGauche = false;
volatile bool videDroit = false;
volatile bool etaitVideGauche = false;
volatile bool etaitVideDroit = false;
volatile char mur;
volatile uint16_t valeurGauche = 0;
volatile uint16_t valeurDroit = 0;
volatile uint16_t valeurCMGauche = 0;
volatile uint16_t valeurCMDroit = 0;

void initialisationMinuterie ( void ) {
    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'etre interrompu alors que
    // le microcontroleur n'est pas pret...
    cli ();

    // configurer et choisir les ports pour les entrees
    // et les sorties. DDRx... Initialisez bien vos variables
    DDRA = 0x00; // Output A

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

void verifierVide()
{
    if (valeurCMGauche > 60){
        videGauche = 1;
        etaitVideGauche = 1;
    }
    if (valeurCMGauche < 60){
        videGauche = 0;
    }
    if (valeurCMDroit > 60){
        videDroit = 1;
        etaitVideDroit = 1;
    }
    if (valeurCMDroit < 60){
        videDroit = 0;
    }
}

void changerDeMur()
{
    del.activateAmbre();
    if(mur == 'd'){
        while(!(valeurCMGauche > 13 && valeurCMGauche < 17)){
            roues.ajustementPWM(80, 160);
        }
        mur = 'g';
    }

    if(mur == 'g'){
        while(!(valeurCMDroit > 13 && valeurCMDroit < 17)){
            roues.ajustementPWM(160, 80);
        }
        mur = 'd';
    }
}

ISR(TIMER2_COMPA_vect) {
    //capteur.lireDetecteur();
        
    valeurGauche = detecteur.lecture(0);
    valeurCMGauche = round(pow(4187.8/valeurGauche, 1.1060));
    
    valeurDroit = detecteur.lecture(1);
    valeurCMDroit = round(pow(4187.8/valeurDroit, 1.1060));
    
    if(valeurGauche < 430.8)
        valeurCMGauche = 0;
    if(valeurDroit < 430.8)
        valeurCMDroit = 0;
    
    verifierVide();
    if(valeurCMGauche > 11 && valeurCMGauche < 18){
         mur = 'g';
        if(videDroit == 0 && etaitVideDroit == 1){               //si le vide n'est plus la on change de mur.
            changerDeMur();
            etaitVideDroit = 0;
        }
    }
    if(valeurCMDroit > 11 && valeurCMDroit < 18){
        mur = 'd';
        if(videGauche == 0 && etaitVideGauche == 1){               //si le vide n'est plus la on change de mur.
            changerDeMur();
            etaitVideGauche = 0;
        }
    }
    if(videGauche == 1 && videDroit == 1){                  //si il y a un vide a gauche ainsi que a droite on longe le mur de son autre cote (U-TURN).
        roues.ajustementPWM(40, 220);
        _delay_ms(2000);
    }
}


int main()
{   

    DDRB = 0xff;    //en sortie
    DDRA = 0x00;    //en entree
    DDRD = 0xff;    //en sortie
    
    initialisationMinuterie();
    partirMinuterie(200);
    del.activateGreen();
    _delay_ms(500);
    del.activateRed();
    _delay_ms(500);
    del.activateGreen();
    _delay_ms(500);
    for(;;)
    {
        if(valeurCMGauche > 13 && valeurCMGauche < 17){
            del.activateGreen();
            _delay_ms(1000);
            roues.ajustementPWM(127,127); 
        }
        if(valeurCMGauche <= 13){
            del.activateRed();
            roues.ajustementPWM(127,85);
        }
        if(valeurCMGauche >= 17 && valeurCMGauche <= 60){
            del.activateRed();
            roues.ajustementPWM(85,127);
        }
        
        if(valeurCMDroit > 13 && valeurCMDroit < 17){
            del.activateGreen();
            _delay_ms(1000);
            roues.ajustementPWM(127,127); 
        }
        if(valeurCMDroit <= 13){
            del.activateRed();
            roues.ajustementPWM(85,127);
        }
        if(valeurCMDroit >= 17 && valeurCMDroit <= 60){
            del.activateRed();
            roues.ajustementPWM(127,85);
        }
        
    }//end for
    return 0;
}
