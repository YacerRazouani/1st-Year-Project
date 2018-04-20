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



int main()
{
    uart.initialisationUART();
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
<<<<<<< HEAD
  
                
    while(1){
        
        typeMessage = uart.lireUART();
        if (typeMessage == CouleurDel || typeMessage == VitesseRoueD || typeMessage == VitesseRoueG)
            operande = uart.lireUART();

=======
    
    
    uint16_t valeurGauche;
    uint16_t valeurDroit;
    uint16_t valeurCMGauche;
    uint16_t valeurCMDroit;
    
    while(1){
        
        typeMessage = uart.lireUART();   //Reste bloquer ici tant que aucun bouton sur le clavier n'est appuyer
        
        capteur.lireDetecteur();
        
        valeurGauche = capteur.obtenirCoteGauche();
        valeurCMGauche = round(pow(4187.8/valeurGauche, 1.1060));

        valeurDroit = capteur.obtenirCoteDroit();
        valeurCMDroit = round(pow(4187.8/valeurDroit, 1.1060));
        
        uart.transmissionUART(0xf6);
        uart.transmissionUART(valeurCMGauche);
        uart.transmissionUART(0xf7);
        uart.transmissionUART(valeurCMDroit);
        
>>>>>>> a5dba4b65cc16f4d6f8c55506fef00a449aa3633
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
                transmissionDonneeDel(operande);
                break;
            
            case (VitesseRoueD):
                transmissionDonneeRoueDroite(operande); 
                break; 
                
            case (VitesseRoueG):
                transmissionDonneeRoueGauche(operande); 
                break; 
<<<<<<< HEAD
        }     
=======
        } 
        
        
>>>>>>> a5dba4b65cc16f4d6f8c55506fef00a449aa3633
        
        /*
         * 9cm = 532.48
         * 10cm = 501.76
         * 11cm = 471.04
         * 12cm = 
         * 
         */
        
        
//         uart.transmissionUART(0xf7);
//         uart.transmissionUART(valeurCMDroit);
         

    }//end of while
    return 0;
}
