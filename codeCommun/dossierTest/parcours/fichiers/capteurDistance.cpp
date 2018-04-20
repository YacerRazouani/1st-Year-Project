#include "capteurDistance.h"
 
void capteurDistance::lireDetecteur()
{
    coteGauche_ = detecteur.lecture(0);						//On lit la valeur du detecteur.
    coteDroit_ = detecteur.lecture(1);
}
uint16_t capteurDistance::obtenirCoteGauche()
{
    return coteGauche_;
}
uint16_t capteurDistance::obtenirCoteDroit()
{
    return coteDroit_;
}

    
