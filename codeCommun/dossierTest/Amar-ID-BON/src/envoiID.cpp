#include "UART.h"
#include "MessageDuLogiciel.h"
#include "MessageDuRobot.h"
#include "roues.h"
#include <avr/io.h> 
#include <avr/interrupt.h>: Interrupts
#include "lumiereDel.h"
#include "capteurDistance.h"
#include "can.h"

//Variable GLOBALE
UART uart; 
capteurDistance capteur;
lumiereDel lumiere;
can detecteur;
roues moteur;

volatile uint8_t buttonPressed = 0x01 ;


void initialisationMinuterie ( void ) {
    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'etre interrompu alors que
    // le microcontroleur n'est pas pret...
    cli ();

    // sei permet de recevoir a nouveau des interruptions.
    sei ();
}

ISR(INT0_vect) {

    _delay_ms(30);

    if(buttonPressed == 0x00)
        buttonPressed = 0x01;
    else
        buttonPressed =0x00;


    uart.transmissionUART(EtatInterrupt);
    uart.transmissionUART(buttonPressed);

    EIFR |= (1 << INTF0) ;

}	
void initialisationInterrupt ( void ) {

    cli ();
    DDRD &= ~(1 << 3);
    EIMSK |= (1 << INT0) ;
    EICRA |= (1 << ISC00) ;
    sei ();

}

struct message {
        uint8_t taille;
        uint8_t code;
        uint8_t nom[9];
};


void transmissionId(message nomMessage) {
 
    uart.transmissionUART(nomMessage.code);
    for (uint8_t i = 0; i < nomMessage.taille; i++){
                uart.transmissionUART(nomMessage.nom[i]);
    }
}

void transmissionDonneeDel (uint8_t donneeDel) {
        if(donneeDel == 01){
            lumiere.activateGreen();
        }
            
        if(donneeDel == 02){
            lumiere.activateRed();
        }
            
        if(donneeDel == 00){
            lumiere.activateNeutral();
        }
}




void transmissionDonneeRoueDroite(int8_t donneeRoue){
    switch(donneeRoue)
    {
        case (0) :
            moteur.activateNeutral();
            break;
        case (25) :
            moteur.activateForwardDroite(140);
            break;
        case (50) :
            moteur.activateForwardDroite(180);
            break;
        case (75) :
            moteur.activateForwardDroite(200);
            break;
        case (100) :
            moteur.activateForwardDroite(255);
            break;
        case (-25) :
            moteur.activateReverseDroite(140);
            break;
        case (-50) :
            moteur.activateReverseDroite(180);
            break;
        case (-75) :
            moteur.activateReverseDroite(200);
            break;
        case (-100) :
            moteur.activateReverseDroite(255);
            break;
    }
    
}


void transmissionDonneeRoueGauche( int8_t donneeRoue){
    
    switch(donneeRoue)
    {
        case (0):
            moteur.activateNeutral();
            break;
        case (25) :
            moteur.activateForwardGauche(140);
            break;
        case (50) :
            moteur.activateForwardGauche(180);
            break;
        case (75) :
            moteur.activateForwardGauche(200);
            break;
        case (100) :
            moteur.activateForwardGauche(255);
            break;
        case (-25) :
            moteur.activateReverseGauche(140);
            break;
        case (-50) :
            moteur.activateReverseGauche(180);
            break;
        case (-75) :
            moteur.activateReverseGauche(200);
            break;
        case (-100) :
            moteur.activateReverseGauche(255);
            break;
    }

}

volatile uint16_t valeurGauche;
volatile uint16_t valeurDroit;
volatile uint16_t valeurCMGauche;
volatile uint16_t valeurCMDroit;

ISR(TIMER2_COMPA_vect)
{
        capteur.lireDetecteur();
        
        valeurGauche = capteur.obtenirCoteGauche();
        valeurCMGauche = round(pow(4187.8/valeurGauche, 1.1060));

        valeurDroit = capteur.obtenirCoteDroit();
        valeurCMDroit = round(pow(4187.8/valeurDroit, 1.1060));
        
        uart.transmissionUART(0xf6);
        uart.transmissionUART(valeurCMGauche);
        uart.transmissionUART(0xf7);
        uart.transmissionUART(valeurCMDroit);
}

void partirMinuterie ( uint16_t duree ) {
    // mode CTC du timer 1 avec horloge divisee par 1024
    // interruption après la duree specifiee
    TCNT2 = 0;
    
    OCR2A = duree;
    TCCR2A |= _BV(WGM21);
    TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);  
    TIMSK2 |= _BV(OCIE2A); 
}

int main()
{
    uart.initialisationUART();
    partirMinuterie(255);
    initialisationInterrupt();
    //MES VARIABLES
    uint8_t typeMessage = 0;
    uint8_t operande = 0;
    DDRD |= (1<<4) | (1<<5)  | (1<<6) | (1<<7) ; 
    DDRA = 0x00; // PORT A est en mode entree
    DDRB = 0xff; // PORT B est en mode sortie
    message nomRobot {3, NomRobot,{'I','j','i'}};
    message numEquipe {4, NumeroEquipe, {'6','9','7','2'}};
    message groupe {1, Groupe, {0x03}};
    message session {4, Session, {'1', '8','-' ,'1'}};
    message couleurBase {1, CouleurBaseRobot, {0x01}};
  
                
    while(1){
        
        typeMessage = uart.lireUART();
        if (typeMessage == CouleurDel || typeMessage == VitesseRoueD || typeMessage == VitesseRoueG)
            operande = uart.lireUART();

        switch(typeMessage)
        {
            
            case (InfoId) :
                cli();
                transmissionId(nomRobot);
                transmissionId(numEquipe);
                transmissionId(session);
                transmissionId(groupe);
                transmissionId(couleurBase);
                sei();
                break;
            
            case (CouleurDel):
                transmissionDonneeDel(operande);
                break;
            
            case (VitesseRoueD):
                transmissionDonneeRoueDroite(operande); 
                break; 
                
            case (VitesseRoueG):
                transmissionDonneeRoueGauche(operande); 
                break; 
        }     
        
         

    }
    return 0;
}
