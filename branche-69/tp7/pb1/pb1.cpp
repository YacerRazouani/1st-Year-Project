/*
 Fichier: pb1.cpp
 Auteur: Yacer Razouani et Amar Ghaly
 Date: 20 fevrier 2018
 Mise a jour : 28 fevrier 2018 à 10h07
 Description: 
 Ce programme a pour but de résoudre le problème suivant :
    Quand le bouton est enfoncé, un compteur qui incrémente 10 fois par seconde est activé.
    Quand le bouton est relâché ou lorsque le compteur atteint 120, la lumière clignote vert
    pendant 1/2 seconde. Ensuite, la carte mère ne fait rien. Puis, deux secondes plus tard, 
    la lumière rouge s'allume. Elle devra clignoter (compteur / 2) fois au rythme de 2 fois par seconde. 
    Ensuite, la lumière tourne au vert pendant une seconde. Finalement, elle s'éteint et le robot 
    revient à son état original. [Source : Énoncé TP7 INF1995]
 
 Mode des Ports : (IN)- PORTD, (OUT) - PORTB<1..2> (bit 1 et 2)
*/
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>: Interrupts

volatile uint8_t state = 0;
volatile int compteur = 0;
volatile int bouttonPoussoir = 0;

/*
 Fonction:   activateNeutral()
 Description:
 Fonction qui eteint la lumiere DEL.
 ParamËtres: Aucun
 Retour: Aucun
 */
void activateNeutral()
{
    PORTB = 0;
}

/*
 Fonction:   activateRed()
 Description:
 Fonction qui allume la lumiere DEL en rouge.
 ParamËtres: Aucun
 Retour: Aucun
 */
void activateRed()
{
    PORTB = 2;
}

/*
 Fonction:   activateGreen()
 Description:
 Fonction qui allume la lumiere DEL en verte.
 ParamËtres: Aucun
 Retour: Aucun
 */
void activateGreen()
{
    PORTB = 1;
}

/*
 Fonction:   initialisation(void)
 Description:
 Fonction qui ajuste bien des paramètres avant le début du traitement principal. C'est une routine d'initialisation.
 ParamËtres: void
 Retour: Aucun
 */
void initialisation(void)
{

    // cli est une routine qui bloque toutes les interruptions.

    // Il serait bien mauvais d'être interrompu alors que

    // le microcontroleur n'est pas prêt...

    cli();

    // configurer et choisir les ports pour les entrées

    // et les sorties. DDRx... Initialisez bien vos variables

    DDRB = 0xff; // PORT B est en mode sortie
    DDRD = 0x00; // PORT D est en mode entree

    // cette procédure ajuste le registre EIMSK

    // de l’ATmega324PA pour permettre les interruptions externes

    EIMSK |= (1 << INT0);

    // il faut sensibiliser les interruptions externes aux

    // changements de niveau du bouton-poussoir

    // en ajustant le registre EICRA

    EICRA |= (1 << ISC00);                                          //On rends le registre EICRA sensible a chaque changement d'etat du boutton
                                                                    //en activant ISC00.
    // sei permet de recevoir à nouveau des interruptions.

    sei();
}

/*
 Fonction:   ISR(INT0_vect)
 Description:
 Fonction qui utilise l'interruption externe zéro puisque le bouton de la section «Interrupt» de la carte mère est relié au port D2.
 ParamËtres: INT0_vect
 Retour: Aucun
 */
ISR(INT0_vect)
{
    _delay_ms(20);
    // anti-rebond
    bouttonPoussoir++;          //On incremente la valeur bouttonPoussoir a chaque fois qu'il y a une interruption,
    state = 0;                  //une interruption s'agit d'un changement d'etat du boutton 
    EIFR |= (1 << INTF0);       //(voir registre EICRA dans la fonction initialisation).
}

/*
 Fonction:   incrementerCompteur()
 Description:
 Fonction qui incremente le compteur 10 fois par seconde puis passe a l'etat 1 si le bouton est relache ou si le compteur atteint 120. 
 ParamËtres: Aucun
 Retour: Aucun
 */
void incrementerCompteur()
{
    compteur++;
    _delay_ms(100);
    if (bouttonPoussoir == 2)
    {
        state = 1;
        bouttonPoussoir = 0;
    }
    if (compteur == 120)
    {
        state = 1;
        bouttonPoussoir = 0;
    }
}

/*
 Fonction:   clignoterVert()
 Description:
 Fonction qui fait clignoter la lumiere DEL en vert pendant 1/2 seconde. 
 ParamËtres: Aucun
 Retour: Aucun
 */
void clignoterVert()
{
    for (int i = 0; i < 5; i++)
    {
        activateGreen();
        _delay_ms(100);
        activateNeutral();
        _delay_ms(100);
    }
}

int main()
{
    initialisation();
    for (;;)
    {
        switch (state)
        {
        case 0:                                                 //Etat initial.
        {
            if (bouttonPoussoir == 1)                           //Si le bouton est appuyer alors on incremente le compteur.
            {
                incrementerCompteur();
            }
            break;
        }
        case 1:                                                 //Etat 1.
        {
            clignoterVert();                                    //La lumiere clignote verte pendant 1/2 seconde.
            activateNeutral();                                  //La lumiere s'eteint 
            _delay_ms(2000);                                    //et la carte mere ne fait rien pendant 2 secondes.
            state = 2;                                          //On passe a l'etat suivant.
            break;
        }
        case 2:                                                 //Etat 2.
        {
            for (int i = 0; i < (compteur / 2); i++)            //La lumiere s'allume rouge puis clignote (compteur / 2) fois
            {                                                   // au rythme de 2 fois par seconde.
                activateRed();                              
                _delay_ms(250);                                 
                activateNeutral();
                _delay_ms(250);
                activateRed();
                _delay_ms(250);
                activateNeutral();
                _delay_ms(250);
            }                                       
            state = 3;                                          //On passe a l'etat suivant.
            break;
        }
        case 3:                                                 //Etat 3.
        {
            activateGreen();                                    //La lumiere tourne verte pendant une seconde.
            _delay_ms(1000);
            activateNeutral();                                  //La umiere s'eteint avant de revenir a l'etat inital.
            compteur = 0;                                       //Le compteur est remis a zero avant de revenir a l'etat original.
            state = 0;                                          //On revient a l'etat originale (initial).
            break;
        }
        }//end switch
    }//end for
    return 0;
}//end main