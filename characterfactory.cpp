#include "characterfactory.h"
#include "character.h"
#include "dndcharacter.h"
#include "dhcharacter.h"

int CharacterFactory::DnD = 0;
int CharacterFactory::DH = 1;
char CharacterFactory::NotFound = 'e';

CharacterFactory::CharacterFactory(int _characterSystem)
    : characterSystem(_characterSystem) {}
//==================================================//
// Basic Factory that only creates one of two types //
//==================================================//
void CharacterFactory::setCharSystem(int _characterSystem){
    characterSystem = _characterSystem;
}

Character* CharacterFactory::createCharacter(){
    if(characterSystem == DH)
        return new DHCharacter();
    else if(characterSystem == DnD)
        return new DnDCharacter();
    else
        throw NotFound;
    //For larger project more implmentations and error handling
}

CharacterFactory::~CharacterFactory(){}
