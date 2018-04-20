#include "can.h"

class capteurDistance
{
public:
    void lireDetecteur();
    uint16_t obtenirCoteGauche();
    uint16_t obtenirCoteDroit();
    bool obtenirVideGauche();
    bool obtenirVideDroit();
    bool obtenirEtaitVideGauche();
    bool obtenirEtaitVideDroit();
    
private:
    can detecteur = can();
    uint16_t coteGauche_ = 0;
    uint16_t coteDroit_ = 0;
    bool videGauche_ = false;
    bool videDroit_ = false;
    bool etaitVideGauche_ = false;
    bool etaitVideDroit_ = false;
};
