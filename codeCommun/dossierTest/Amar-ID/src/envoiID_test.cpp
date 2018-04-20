#include "UART.h"
#include "MessageDuLogiciel.h"
#include "MessageDuRobot.h"
#include "roues.h"
#include <avr/io.h> 
#include <avr/interrupt.h>: Interrupts
#include "lumiereDel.h"
#include "capteurDistance.h"

//Variable GLOBALE
capteurDistance capteur;
UART uart; 
lumiereDel lumiere;
can detecteur;
roues moteur;
volatile uint8_t buttonPressed = 0x01 ;
volatile uint8_t typeMessage;

// ISR(INT0_vect) {
// 
//     _delay_ms(30);
// 
//     if(buttonPressed == 0x00)
//         buttonPressed = 0x01;
//     else
//         buttonPressed =0x00;
// 
// 
//     uart.transmissionUART(EtatInterrupt);
//     uart.transmissionUART(buttonPressed);
// 
//     EIFR |= (1 << INTF0) ;
// 
// }	
// void initialisationInterrupt ( void ) {
// 
//     cli ();
//     DDRD &= ~(1 << 3); // A MODIFIER POUR QUE INTERRUPT MARCHE AVEC ROUES SUR PORTD
//     EIMSK |= (1 << INT0) ;
//     EICRA |= (1 << ISC00) ;
//     sei ();
// 
// }

void initialisationMinuterie ( void ) {
    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'etre interrompu alors que
    // le microcontroleur n'est pas pret...
    cli ();

    // configurer et choisir les ports pour les entrees
    // et les sorties. DDRx... Initialisez bien vos variables
    DDRA = 0x00; // Output A
    DDRD = 0x00;

    // cette procédure ajuste le registre EIMSK
    // de ATmega324PA pour permettre les interruptions externes
    EIMSK |= _BV(INT0);

    // il faut sensibiliser les interruptions externes aux
    // changement de niveau du bouton-poussoir
    // en ajustant le registre EICRA
    EICRA |= _BV(ISC01) | _BV(ISC00);

    // sei permet de recevoir a nouveau des interruptions.
    sei ();
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


void transmissionDonneeRoueDroite (int8_t donneeRoueD){
    switch(donneeRoueD)
    {
//         case (0) :
//             moteur.activateNeutral();
//             break;
//         case (25) :
//             moteur.activateForward('d', 140);
//             break;
//         case (50) :
//             moteur.activateForward('d', 180);
//             break;
//         case (75) :
//             moteur.activateForward('d', 200);
//             break;
//         case (100) :
//             moteur.activateForward('d', 255);
//             break;
//         case (-25) :
//             moteur.activateReverse('d', 140);
//             break;
//         case (-50) :
//             moteur.activateReverse('d', 180);
//             break;
//         case (-75) :
//             moteur.activateReverse('d', 200);
//             break;
//         case (-100) :
//             moteur.activateReverse('d', 255);
//             break;
    }

}


void transmissionDonneeRoueGauche (int8_t donneeRoueG){
    switch(donneeRoueG)
    {
//         case (0):
//             moteur.activateNeutral();
//             break;
//         case (25) :
//             moteur.activateForward('g', 140);
//             break;
//         case (50) :
//             moteur.activateForward('g', 180);
//             break;
//         case (75) :
//             moteur.activateForward('g', 200);
//             break;
//         case (100) :
//             moteur.activateForward('g', 255);
//             break;
//         case (-25) :
//             moteur.activateReverse('g', 140);
//             break;
//         case (-50) :
//             moteur.activateReverse('g', 180);
//             break;
//         case (-75) :
//             moteur.activateReverse('g', 200);
//             break;
//         case (-100) :
//             moteur.activateReverse('g', 255);
//             break;
    }

}



int main()
{
    //INITIALISATIONS
    uart.initialisationUART();
    //initialisationInterrupt();
    initialisationMinuterie();
    
    //MES VARIABLES
    uint8_t donneeDel;
    uint8_t donneeInterrupt;
    int8_t donneeRoueD; 
    int8_t donneeRoueG; 
    
    DDRD = 0xff; // PORT D est en mode entree A MODIFIER POUR QUE INTERRUPT MARCHE AVEC ROUES SUR PORTD
    DDRA = 0x00; // PORT A est en mode entree
    DDRB = 0xff; // PORT B est en mode sortie
    message nomRobot {3, NomRobot,{'I','j','i'}};
    message numEquipe {4, NumeroEquipe, {'6','9','7','2'}};
    message groupe {1, Groupe, {0x03}};
    message session {4, Session, {'1', '8','-' ,'1'}};
    message couleurBase {1, CouleurBaseRobot, {0x01}};
    
    while(1){
        typeMessage = uart.lireUART();   //Reste bloquer ici tant que aucun bouton sur le clavier n'est appuyer
        partirMinuterie(255);

        
        switch(typeMessage)
        {
            
            case (InfoId) :
                transmissionId(nomRobot);
                transmissionId(numEquipe);
                transmissionId(session);
                transmissionId(groupe);
                transmissionId(couleurBase);
                break;
            
            case (CouleurDel):
                donneeDel = uart.lireUART();
                transmissionDonneeDel(donneeDel);
                break;
            
            case (VitesseRoueD):
                donneeRoueD = uart.lireUART();
                transmissionDonneeRoueDroite(donneeRoueD); 
                break; 
                
            case (VitesseRoueG):
                donneeRoueG = uart.lireUART();
                transmissionDonneeRoueGauche(donneeRoueG); 
                break; 
        } 
        
        
        /*
         * 9cm = 532.48
         * 10cm = 501.76
         * 11cm = 471.04
         * 12cm = 
         * 
         */
        
    }//end of while
    return 0;
}
