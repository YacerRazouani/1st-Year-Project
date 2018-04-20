#include "capteurDistance.h"
 
void capteurDistance::lireDetecteur()
{
    uint16_t moyenneGauche = 0;
    uint16_t moyenneDroit = 0;
    for (uint8_t i = 0; i < 30; i++)
    {
        coteGauche_ = detecteur.lecture(0);						//On lit la valeur du detecteur.
        coteDroit_ = detecteur.lecture(1);
        moyenneGauche += coteGauche_;
        moyenneDroit += coteDroit_;
    }
    moyenneDroit /= 30;
    moyenneGauche /= 30;
    
    coteGauche_ = moyenneGauche;
    coteDroit_ = moyenneDroit;
}
uint16_t capteurDistance::obtenirCoteGauche()
{
    return coteGauche_;
}
uint16_t capteurDistance::obtenirCoteDroit()
{
    return coteDroit_;
}

    
