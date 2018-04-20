#include "capteurDistance.h"

void capteurDistance::lireDetecteur()
{
    coteGauche_ = detecteur.lecture(0);						//On lit la valeur du detecteur.
    coteDroit_ = detecteur.lecture(1);
    
    if(coteGauche_ < 204){
        videGauche_ = true;
        if(videGauche_ == 1){
            etaitVideGauche_ = 1;
        }
    }
    if(coteDroit_ < 204){
        videDroit_ = true;
        if(videDroit_ == 1){
            etaitVideDroit_ = 1;
        }
    }
    if(coteGauche_ > 204){
        videGauche_ = false;
    }
    if(coteDroit_ > 204){
        videDroit_ = false;
    }
}
uint16_t capteurDistance::obtenirCoteGauche()
{
    return coteGauche_;
}
uint16_t capteurDistance::obtenirCoteDroit()
{
    return coteDroit_;
}
bool capteurDistance::obtenirVideGauche()
{
    return videGauche_;    
}
bool capteurDistance::obtenirVideDroit()
{
    return videDroit_;
}
bool capteurDistance::obtenirEtaitVideGauche()
{
    return etaitVideGauche_;
}
bool capteurDistance::obtenirEtaitVideDroit()
{
    return etaitVideDroit_;
}
    
