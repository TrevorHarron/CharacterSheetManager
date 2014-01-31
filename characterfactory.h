#ifndef CHARACTERFACTORY_H
#define CHARACTERFACTORY_H

#include "character.h"

class CharacterFactory
{
public:
    static int DnD;
    static int DH;
    static char NotFound;
    CharacterFactory(int _characterSystem = 0);
    ~CharacterFactory();
    void setCharSystem(int characterSystem);
    Character* createCharacter();
private:
    int characterSystem;
};

#endif // CHARACTERFACTORY_H
