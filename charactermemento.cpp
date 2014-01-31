#include "charactermemento.h"

CharacterMemento::CharacterMemento(QHash<QString, QString> _characterState, QString _feats) :
    characterState(_characterState), feats(_feats) {}

QHash<QString,QString> CharacterMemento::getCharacterState(){
    return characterState;
}

QString CharacterMemento::getCharacterFeats(){
    return feats;
}

bool CharacterMemento::operator==(CharacterMemento other){
    return characterState == other.getCharacterState() &&
            feats == other.getCharacterFeats();
}

CharacterMemento::~CharacterMemento()
{
    characterState.clear();
}
