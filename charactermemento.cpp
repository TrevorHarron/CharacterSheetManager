#include "charactermemento.h"

CharacterMemento::CharacterMemento(QHash<QString, QString> _characterState) :
    characterState(_characterState) {}

QHash<QString,QString> CharacterMemento::getCharacterState(){
    return characterState;
}

CharacterMemento::~CharacterMemento()
{
    characterState.clear();
}
