#include "memoire_24.h"
#include "lumiereDel.h"
#include "roues.h"
#include "son.h"
#include "outin.h"

//Sequence de demarrage
void demarrage()
{
    lumiereDel del = lumiereDel('A');      // utilise PORTA pour allumer la DEL
    del.activateRed();
    _delay_ms(500);
    del.activateGreen();
    _delay_ms(500);
    del.activateRed();
    _delay_ms(500);
    del.activateGreen();
    _delay_ms(500);
    del.activateNeutral();
}

void initialisationUART ( void ) {

// 2400 bauds. Nous vous donnons la valeur des deux

// premiers registres pour vous éviter des complications

UBRR0H = 0;

UBRR0L = 0xCF;

// permettre la reception et la transmission par le UART0

UCSR0A = (1 << UDRE0) ;  /* USART Data Register Empty. Set when the UDR0 register is
                            empty and new data can be transmitted. */

UCSR0B = (1 << RXEN0) | (1 << TXEN0) ; /* Receiver Enable AND Transmitter enable */

// Format des trames: 8 bits, 1 stop bits, none parity

UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) ; /* 8-bit (011) we didnt activate UCSZ20 */

}

void transmissionUART ( uint8_t donnee ) {

    while ( !( UCSR0A & (1 << UDRE0))) {            /* Cannot put "donnee" 
                                                     into UDR0 if UDRE bit is not set */
    }
    
    UDR0 = donnee;
}

unsigned char lireUART () {
    
    while( !(UCSR0A & (1<<RXC0))){                  /* Activate RXC0 to signal that data is             
                                                    available and has not been read yet */
    }
    
    return UDR0;
}

//Enumeration des mnemoniques en fonction de leur instructions binaires
enum instruction 
{
    dbt = 0x01,
    att = 0x02,
    dal = 0x44, 
    det = 0x45,
    sgo = 0x48,
    sar = 0x09,
    mar = 0x60,
    marr = 0x61,
    mav = 0x62,
    mre = 0x63,
    trd = 0x64,
    trg = 0x65,
    dbc = 0xC0,
    fbc = 0xC1,
    fin = 0xFF
};

//VARIABLES
uint16_t adresse = 0x00;             //Commence a 0x00 car adresse debut
uint8_t instruction;
uint8_t operande;
uint8_t compteur;
uint8_t alreadyCounting = 0;

//Creation d'un objet de classe Memoire24CXXX
Memoire24CXXX memory = Memoire24CXXX();    

//Fonction permettant la lecture de la prochaine instruction dans le switch
void readNextInstruction()
{
    memory.lecture(adresse++, &instruction);
    memory.lecture(adresse++, &operande);
    transmissionUART(instruction);
    transmissionUART(operande);
    if (instruction == dbc && alreadyCounting == 0){
        compteur = operande;
        alreadyCounting = 1; 
    }

}

int main ()
{   
    //Creation d'un objet ports de classe outin a l'aide du constructeur par defaut (tous les ports en sorties)
    outin ports = outin();
    
    //Appel de la fonction de demarrage
    demarrage();
    
    initialisationUART();
    
    uint16_t adresseRetour; 
    
    //Creation d'un objet del de classe lumiereDel en mettant le portA en sortie
    lumiereDel del = lumiereDel('A');
    
    //Creation d'un objet moteur de classe roues 
    roues moteur;
    
    //Creation d'un objet son de classe son 
    son son;
    
    //Initialisation de l'objet son en mettant le portB en mode sortie
    son.initialisationSon();
    
    
    memory.init();
    
    
    do{
        memory.lecture(adresse++, &instruction);
        memory.lecture(adresse++, &operande);
        transmissionUART(instruction);
        transmissionUART(operande);
    }while (instruction != dbt);
    
    do{
        switch (instruction)
        {
            //cas indiquant le debut de la sequence
            case dbt: {
                readNextInstruction();
                break;
            }
            
            //cas indiquant une attente. 
            //prend la valeur de l'operande et multiplie par 25
            case att: {
                uint16_t temps = operande * 25;
                for(uint16_t i = 0; i < temps; i++){             
                    _delay_ms(1);
                }
                readNextInstruction();
                break;
            }
            
            //cas qui allume la del 
            case dal: {
                del.activateGreen();
                _delay_ms(100);
                readNextInstruction();
                break;
            }
            
            //cas qui eteint la del 
            case det: 
                del.activateNeutral();
                _delay_ms(50);
                readNextInstruction();
                break;
            
            //cas qui joue une sonorité, la valeur de l'operande en parametre
            //correspond a la note qui sera jouer
            case sgo: {
                son.jouerSonorite(operande);
                readNextInstruction();
                break;
            }
            
            //cas qui arrete de jouer un son
            case sar: {
                son.arreterSon();
                readNextInstruction();
                break;
            }
            
            //cas qui arrete de jouer un son
           case mar: {
                moteur.activateNeutral();
                readNextInstruction();
                break;
            }
            
            //cas qui arrete de jouer un son   
            case marr: {
                moteur.activateNeutral();
                readNextInstruction();
                break;
            }
            
            //cas qui avance en ligne droite a une vitesse donne par operande/255*100
            case mav: {
                moteur.ajustementPWM(operande, operande);
                readNextInstruction();
                break;
            }
            
            //cas qui recule en ligne droite a une vitesse donne par operande/255*100
            case mre: {
                PORTD |= (1 << 6); 
                PORTD |= (1 << 7); 
                moteur.ajustementPWM(operande, operande);
                readNextInstruction();
                break;
            }
            
            //cas qui tourne a droite en donnant la valeur de 0 a la roue droite 
            case trd: {
                moteur.activateForward('g', 255);
                _delay_ms(1000);
                moteur.activateNeutral();
                readNextInstruction();
                break;
            }
            
            //cas qui tourne a gauche en donnant la valeur de 0 a la roue gauche
            case trg: {
                moteur.activateForward('d', 255);
                _delay_ms(1000);
                moteur.activateNeutral();
                readNextInstruction();
                break;
            }
            
            //cas de debut de la boucle qui emmagasine l'adresse du present point d'execution
            case dbc: {
                adresseRetour = adresse;
                readNextInstruction();
                break;
            }
            
            //si le compteur n'est pas = 0, retourne l'adresse de debut de boucle et
            //decremente le compteur
            case fbc: {
                if (compteur > 0){
                    adresse = adresseRetour;
                    compteur--;    
                }
                if (compteur == 0){
                    alreadyCounting = 0;
                }
                readNextInstruction();
                break;
            }
            
            //cas indiquant la fin de la sequence d'instruction 
            case fin: {
                break;
            }
            
        } // end of switch
        
    }while (instruction != fin);
    
    return 0;
}
