#include "charactermemento.h"

CharacterMemento::CharacterMemento(QHash<QString, QString> _characterState, QString _feats) :
    characterState(_characterState), feats(_feats) {}

QHash<QString,QString> CharacterMemento::getCharacterState(){
    return characterState;
}

QString CharacterMemento::getCharacterFeats(){
    return feats;
}


CharacterMemento::~CharacterMemento()
{
    characterState.clear();
}
