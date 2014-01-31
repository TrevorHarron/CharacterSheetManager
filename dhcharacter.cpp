#include "dhcharacter.h"

DHCharacter::DHCharacter()
{
    QString aryId[] = {"name", "class", "level", "experience", "max_wounds", "current_wounds","corruption","insanity"};
    for(int index = 0; index < 8; ++index)
        ID_NAMES.push_back(aryId[index]);

    QString aryStats[] = {"weapon_skill", "ballistic_skill","strength", "toughness","agility",
                          "intelligence", "perception", "will_power","fellowship"};
    for(int index = 0; index < 9; ++index)
        BASE_STAT_NAMES.push_back(aryStats[index]);

    QString arySaves[] = {"will_power", "agility", "toughness"};
    for(int index = 0; index < 3; ++index)
        SAVE_STAT_NAMES.push_back(arySaves[index]);

    Character::initAttributes();
    die = new Die(100);
    system = "dh";
}

//=======================//
// Get Modifier function //
//=======================//
int DHCharacter::getMod(QString key){
    if(!Character::isSkill(key) || !ID_NAMES.contains(key))
        return attributes.value(key).toInt() / 10;
    else
        return -9000; // on the off chance someone tries to cheat
}

//=================//
// Rolling Methods //
//=================//
int DHCharacter::makeAttack(QString type,QString mod,int bonus){
    return makeRoll(type,mod,bonus);
}

int DHCharacter::makeSave(QString type, QString mod, int bonus){
    return makeRoll(type,mod,bonus);
}

int DHCharacter::makeRoll(QString,QString,int){
    return die->roll();
}

//=================================//
// Clean up and End of DHCharacter //
//=================================//
DHCharacter::~DHCharacter()
{
    this->~Character();
}
