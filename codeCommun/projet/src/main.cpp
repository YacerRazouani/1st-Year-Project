/*
 * Titre: Projet Final INF1995
 * Contributeurs: Yacer Razouani, Georges Louis, Amar Ghaly et Mazigh Ouanes
 */

/*
 * INCLUDES
 */
//INCLUDES POUR LE LOGICIEL DIAGNOSTIQUE
#include "UART.h"
#include "MessageDuLogiciel.h"
#include "MessageDuRobot.h"
#include "roues.h"
#include <avr/io.h> 
#include <avr/interrupt.h>
#include "lumiereDel.h"
#include "capteurDistance.h"
#include "can.h"
//INCLUDES POUR LE PARCOURS
#include "son.h"
#define F_CPU 8000000UL
//INCLUDES POUR MAIN.CPP
#include "memoire_24.h"
/*
 * VARIABLES GLOBALES
 */
//VARIABLES GLOBALES POUR LE MAIN.CPP
volatile uint8_t routine = 0x00;
//VARIABLES GLOBALES POUR LE LOGICIEL DIAGNOSTIQUE 
volatile uint8_t buttonPressed = 0x01 ;
volatile uint16_t valeurGauche;
volatile uint16_t valeurDroit;
volatile uint16_t valeurCMGauche;
volatile uint16_t valeurCMDroit;
//VARIABLES GLOBALES POUR LE PARCOURS 
volatile bool videDroit = false;
volatile bool videGauche = false;
volatile bool etaitVideDroit = false;
volatile bool etaitVideGauche = false;
volatile bool debut = true;
volatile uint16_t longueur= 0;
volatile uint16_t moyenneCMGauche = 0;
volatile uint16_t moyenneCMDroit = 0; 
enum state {Default, MurGauchePasLeDroitDeChanger, MurGaucheDroitDeChanger, MurDroitPasLeDroitDeChanger, MurDroitDroitDeChanger};
volatile uint8_t state = Default;

/* 
 * LE NECESSAIRE AU LOGICIEL DIAGNOSTIQUE
 */
//OBJETS GLOBAUX POUR LE LOGICIEL DIAGNOSTIQUE
UART uart; 
capteurDistance capteur;
lumiereDel del;
can detecteur;
roues moteur;

//STRUCT LOGICIEL DIAGNOSTIQUE
struct message {
        uint8_t taille;
        uint8_t code;
        uint8_t nom[9];
};

//FONCTIONS DU LOGICIEL DIAGNOSTIQUE
/*
 * nom de la fonction : transmissionId
 * description: transmet un message au logiciel diagnostique
 * parametres: nomMessage de type message 
 * retour: void
 */
void transmissionId(message nomMessage) {
 
    uart.transmissionUART(nomMessage.code);
    for (uint8_t i = 0; i < nomMessage.taille; i++){
                uart.transmissionUART(nomMessage.nom[i]);
    }
}
/*
 * nom de la fonction : transmissionDonneDel
 * description: allume la DEL d'une certaine couleur
 * parametres: donneeDel qui indique la demande du logiciel recue
 * retour: void
 */
void transmissionDonneeDel (uint8_t donneeDel) {
        if(donneeDel == 01){
            del.activateGreen();
        }
            
        if(donneeDel == 02){
            del.activateRed();
        }
            
        if(donneeDel == 00){
            del.activateNeutral();
        }
}
/*
 * nom de la fonction: transmissionDonneRoueDroite 
 * description: tourne la roue droite vers l'avant ou vers l'arriere selon un certain pourcentage
 * parametres: int8_t donneeRoue
 * retour: void
 */
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
/*
 * nom de la fonction: transmissionDonneRoueGauche 
 * description: tourne la roue gauche vers l'avant ou vers l'arriere selon un certain pourcentage
 * parametres: int8_t donneeRoue
 * retour: void
 */
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

//LES INTERRUPTS DU LOGICIEL DIAGNOSTIQUE
/*
 * nom de la fonction: initialisationMinuterie
 * description: initialisation pour le timer
 * parametres: aucun
 * retour: void
 */
void initialisationMinuterie ( void ) {
    // cli est une routine qui bloque toutes les interruptions.
    // Il serait bien mauvais d'etre interrompu alors que
    // le microcontroleur n'est pas pret...
    cli ();

    // sei permet de recevoir a nouveau des interruptions.
    sei ();
}
/*
 * nom de la fonction: initialisationInterrupt
 * description: initialisation pour le interrupt
 * parametres: aucun
 * retour: void
 */
void initialisationInterrupt ( void ) {

    cli ();
    DDRD &= ~(1 << 3);
    EIMSK |= (1 << INT0) ;
    EICRA |= (1 << ISC00) ;
    sei ();

}
/*
 * nom de la fonction: ISR
 * description: permet d'interrompre le main lorsque le bouton interrupt est appuyer 
 * parametres: INT0_vect
 * retour: void
 */
ISR(INT0_vect) {

    _delay_ms(30);
	//LOGICIEL DIAGNOSTIQUE
    if(routine == 0x00)                                             
    {
        if(buttonPressed == 0x00)
            buttonPressed = 0x01;
        else
            buttonPressed =0x00;


        uart.transmissionUART(EtatInterrupt);
        uart.transmissionUART(buttonPressed);
    }
	//PARCOURS
    if(routine == 0x01)                                             
    {
        if(valeurCMGauche < valeurCMDroit)
        {
            cli();
            moteur.ajustementPWM(200,0);
            _delay_ms(1400);
            moteur.ajustementPWM(0,0);
            _delay_ms(1400);
            state = MurDroitPasLeDroitDeChanger;
            sei();
            /*temp =valeurCMDroit;
            valeurCMDroit = valeurCMGauche;
            valeurCMGauche = temp;*/
        }
        else if(valeurCMDroit < valeurCMGauche)
        {
            cli();
            moteur.ajustementPWM(0,200);
            _delay_ms(1400);
            moteur.ajustementPWM(0,0);
            _delay_ms(1400);
            state = MurGauchePasLeDroitDeChanger;
            /*temp =valeurCMDroit;
            valeurCMDroit = valeurCMGauche;
            valeurCMGauche = temp;*/
            sei();
        }
        moteur.ajustementPWM(100,100);
    }
    EIFR |= (1 << INTF0) ;
}
/*
 * nom de la fonction: ISR
 * description: permet d'interrompre le main lorsque le timer est activer
 * parametres: TIMER2_COMPA_vect
 * retour: void
 */	
ISR(TIMER2_COMPA_vect)
{
        capteur.lireDetecteur();
        /*  TRANSFORMER LES VOLTS EN CENTIMETRES */
        valeurGauche = capteur.obtenirCoteGauche();
        valeurCMGauche = round(pow(4187.8/valeurGauche, 1.1060));
        valeurDroit = capteur.obtenirCoteDroit();
        valeurCMDroit = round(pow(4187.8/valeurDroit, 1.1060));
		//LOGICIEL DIAGNOSTIQUE
        if(routine == 0x00)                                         
        {
            uart.transmissionUART(0xf6);
            uart.transmissionUART(valeurCMGauche);
            uart.transmissionUART(0xf7);
            uart.transmissionUART(valeurCMDroit);
        }
		//PARCOURS
        if(routine == 0x01)                                         
        {
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
/*
 * FIN DU NECESSAIRE AU LOGICIEL DIAGNOSTIQUE
 */

/*
 * NECESSAIRE AU PARCOURS
 */

//OBJETS GLOBAUX POUR LE PARCOURS
son son;

//FONCTIONS DU PARCOURS
/*
 * nom de la fonction: jouerSon
 * description: joue un son lorsque appeller dans la fonction detecterPoteau
 * parametres: aucun
 * retour: void
 */
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
/*
 * nom de la fonction: detecterPoteau
 * description: lorsqu'un vide est combler mais il subitement de retour retourne true
 * parametres: aucun
 * retour: void
 */
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
/*
 * nom de la fonction: nouveauMurDroit
 * description: lorsqu'un vide du coter droit est combler retourne true
 * parametres: aucun
 * retour: void
 */
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
/*
 * nom de la fonction: nouveauMurGauche
 * description: lorsqu'un vide du coter gauche est combler retourne true
 * parametres: aucun
 * retour: void
 */
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
/*
 * nom de la fonction: ajusterMurGauche
 * description: allume la lumiere verte et roule tout droit si le mur est a environ 15cm du mur,
 * 		s'il est trop loin la lumiere est rouge et il se rapproche, s'il est trop proche 
 * 		la lumiere est rouge et il s'eloigne.
 * parametres: aucun
 * retour: void
 */
void ajusterMurGauche()
{
    cli();
    if(valeurCMGauche >= 13 && valeurCMGauche < 17)
    {
        del.activateGreen();
        moteur.ajustementPWM(120, 120);
    }
	//trop proche du mur
    else if(valeurCMGauche < 13)     						
    {
        del.activateRed();
        moteur.ajustementPWM(120, 100);	// 100,80
    }
	//trop loin du mur
    else if(valeurCMGauche >= 17 && valeurCMGauche < 25)     
    {
        del.activateRed();
        moteur.ajustementPWM(100,120);	// 80,100
    }
    sei();
}
/*
 * nom de la fonction: ajusterMurDroit
 * description: allume la lumiere verte et roule tout droit si le mur est a environ 15cm du mur,
 * 		s'il est trop loin la lumiere est rouge et il se rapproche, s'il est trop proche 
 * 		la lumiere est rouge et il s'eloigne.
 * parametres: aucun
 * retour: void
 */
void ajusterMurDroit()
{
     cli();
    if(valeurCMDroit >= 13 && valeurCMDroit < 17)
    {
        del.activateGreen();
        moteur.ajustementPWM(120, 120);
    }
	//trop proche du mur
    else if(valeurCMDroit < 13)      					
    {
        del.activateRed();
        moteur.ajustementPWM(100,130);	// 80,110
    }
	//trop loin du mur
    else if(valeurCMDroit >= 17 && valeurCMDroit < 25)      
    {
        del.activateRed();
        moteur.ajustementPWM(110, 100); // 90,80
    }
     sei();
}
/*
 * nom de la fonction: verifierMurDroit
 * description: fait un tour de 90 degrees vers la droite s'avance au nouveau mur, et revient comme il etait
 * parametres: aucun
 * retour: void
 */
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
    uint16_t temps = round((distanceAParcourir * 1500) / 20);
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
/*
 * nom de la fonction: verifierMurGauche
 * description: fait un tour de 90 degrees vers la gauche s'avance au nouveau mur, et revient comme il etait
 * parametres: aucun
 * retour: void
 */
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
        uint16_t temps = round((distanceAParcourir * 1000)/ 20);
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

//LES INTERRUPTS DU PARCOURS
void initialisationInterruptParcours ( void ) {
    cli ();
    
    DDRD &= ~(1 << PD2);
    EIMSK |= (1 << INT0);
    EICRA &= ~(1 << ISC00);
    EICRA |= (1 << ISC01);

    sei ();
}
/*
 * FIN NECESSAIRE AU PARCOURS
 */

//FONCTIONS POUR LE MAIN.CPP
/*
 * nom de la fonction: initialisationRoutine
 * description: permet de changer de mode a chaque reset en reecrivant la routine selon ce qu'il y avait
 * parametres: aucun
 * retour: void
 */
void initialisationRoutine()
{
    Memoire24CXXX mem;
    mem.lecture(0x00, (uint8_t*)&routine);
    if(routine == 0x00)
        mem.ecriture(0x00, 0x01);
    else
        mem.ecriture(0x00, 0x00);
}

//MAIN
int main()
{
    DDRA = 0x00;
    initialisationRoutine();
    //LOGICIEL DIAGNOSTIQUE
    if(routine == 0x00)
    {
        //DEMARRAGE LOGICIEL DIAGNOSTIQUE
        del.activateGreen();
        _delay_ms(2000);
        del.activateNeutral();
        
        
        uart.initialisationUART();
        partirMinuterie(255);
        initialisationInterrupt();
        //MES VARIABLES
        uint8_t typeMessage = 0;
        uint8_t operande = 0;
        DDRD |= (1<<4) | (1<<5)  | (1<<6) | (1<<7) ; 
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
            }//end of switch-case
        }//end of while
    }//end of if
    
    //PARCOURS
    if(routine == 0x01)
    {
        //DEMARRAGE PARCOURS
        del.activateRed();
        _delay_ms(2000);
        del.activateNeutral();
        
        DDRB = 0xff;
        DDRD = 0xff;
        initialisationInterruptParcours();
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
    }//end of if
    return 0;
}
