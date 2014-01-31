#include "dndcharacter.h"

DnDCharacter::DnDCharacter()
{
    QString aryId[] = {"name", "class", "race", "level", "experience", "max_health", "current_hp","bab"};
    for(int index = 0; index < 8; ++index)
        ID_NAMES.push_back(aryId[index]);

    QString aryStats[] = {"strength", "dexterity", "constitution", "intellegence", "wisdom","charisma"};
    for(int index = 0; index < 6; ++index)
        BASE_STAT_NAMES.push_back(aryStats[index]);

    QString arySaves[] = {"will", "reflex", "fortitude"};
    for(int index = 0; index < 3; ++index)
        SAVE_STAT_NAMES.push_back(arySaves[index]);

    Character::initAttributes();
    die = new Die(20);
    system = "dnd";
}

// Get Modifier function //
int DnDCharacter::getMod(QString key){
    if(!Character::isSkill(key) || !ID_NAMES.contains(key))
        return (attributes.value(key).toInt() -10) / 2;
    else
        return -9000; // on the off chance someone tries to cheat
}

//=================//
// Rolling Methods //
//=================//
int DnDCharacter::makeAttack(QString, QString mod,int bonus){
    if(mod == "dexterity")
        return die->roll() + getMod(mod) + getValue("bab").toInt() + bonus;
     else
        return die->roll() + getMod("strength") + getValue("bab").toInt() + bonus;
}

int DnDCharacter::makeRoll(QString type, QString mod, int bonus){
    if(type == "attack")
        return makeAttack(type, mod,bonus);
    else if(type == "will" || type == "fortitude" || type == "reflex")
        return makeSave(type,mod,bonus);
    else if(type == "None")
        return die->roll() + getMod(mod) + bonus;
    else
        return die->roll() + getValue(type).toInt() + getMod(mod) + bonus;
}

int DnDCharacter::makeSave(QString type, QString, int bonus){
    if(type == "will")
        return die->roll() + getMod("wisdom")+ getValue(type).toInt() + bonus;
    else if(type == "fortitude")
        return die->roll() + getMod("constitution") + getValue(type).toInt() + bonus;
    else
        return die->roll() + getMod("dexterity") + getValue(type).toInt() + bonus;
}

//==================================//
// Clean up and End of DnDCharacter //
//==================================//
DnDCharacter::~DnDCharacter()
{
    this->~Character();
}
