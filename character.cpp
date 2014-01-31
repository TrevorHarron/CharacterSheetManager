#include "character.h"

#include <QTime>

// Getter and Setters //
void Character::setValue(QString key, int value){
    if(!ID_NAMES.contains(key))
        this->attributes[key] = *(new QString(value));
}

void Character::setValue(QString key, QString value){
    this->attributes[key] = value;
}

QString Character::getValue(QString key){
    return this->attributes[key];
}

void Character::removeValue(QString key){
    if(isSkill(key))
        this->attributes.remove(key);
}

QHash<QString,int>* Character::getStats(){
    QHash<QString,int> *stats =  new QHash<QString,int>();
    QVector<QString>::Iterator itStats;
    for(itStats = BASE_STAT_NAMES.begin(); itStats != BASE_STAT_NAMES.end(); ++itStats)
        stats->insert(*itStats, this->attributes[*itStats].toInt());
    return stats;
}

QHash<QString,int>* Character::getSaves(){
    QHash<QString,int> *saves = new QHash<QString,int>();
    QVector<QString>::Iterator itSaves;
    for(itSaves = SAVE_STAT_NAMES.begin(); itSaves != SAVE_STAT_NAMES.end(); ++itSaves)
        saves->insert(*itSaves, this->attributes[*itSaves].toInt());
    return saves;
}

QHash<QString,int>* Character::getSkills(){
    QHash<QString,int> *skills = new QHash<QString,int>();
    QList<QString> keys = attributes.keys();
    QList<QString>::Iterator itSkills;
    for(itSkills = keys.begin(); itSkills != keys.end(); ++itSkills){
        QString key = *itSkills;
        if(isSkill(key))
            skills->insert(key, attributes[key].toInt());
    }
    return skills;
}

QString Character::getSystem(){
    return system;
}

//Helper fuctions
bool Character::isSkill(QString _item){
    return  !(this->ID_NAMES.contains(_item) ||
            this->BASE_STAT_NAMES.contains(_item) ||
            this->SAVE_STAT_NAMES.contains(_item));
}

bool Character::containsValue(QString item){
    return attributes.contains(item);
}

//to<Container> methods//
QHash<QString,QString>* Character::toHash(){
    return &(attributes);
}

QVector<QString>* Character::getStatVector(){
    return &(allStatNames);
}

//Memento Creation does not save the system for now
CharacterMemento Character::createMemento(){
    QHash<QString,QString> state = *this->toHash();
    return *(new CharacterMemento(state));
}

//Attribute initiallization
void Character::initAttributes(){

    QVector<QString>::Iterator attrIterator;

    for(attrIterator = ID_NAMES.begin(); attrIterator != ID_NAMES.end(); ++attrIterator){
        this->attributes[*attrIterator] = "0";
        allStatNames.push_back(*attrIterator);
    }

    for(attrIterator = BASE_STAT_NAMES.begin(); attrIterator != BASE_STAT_NAMES.end(); ++attrIterator){
        this->attributes[*attrIterator] = "0";
        allStatNames.push_back(*attrIterator);
    }

    for(attrIterator = SAVE_STAT_NAMES.begin(); attrIterator != SAVE_STAT_NAMES.end(); ++attrIterator){
        this->attributes[*attrIterator] = "0";
        allStatNames.push_back(*attrIterator);
    }
}

//===============================//
// Clean up and End of Character //
//===============================//
Character::~Character()
{
    if(&allStatNames != NULL)
        allStatNames.clear();
    if(&attributes != NULL)
        attributes.clear();
    if(&ID_NAMES != NULL)
        ID_NAMES.clear();
    if(&SAVE_STAT_NAMES)
         SAVE_STAT_NAMES.clear();
    if(&BASE_STAT_NAMES != NULL)
         BASE_STAT_NAMES.clear();
    if(&die != NULL)
        delete die;
}

//=====================//
// Die Class for rolls //
//=====================//
Die::Die(int _sides) : sides(_sides){
    qsrand(QTime::currentTime().msec());
}

int Die::roll(){
    return qrand() % ((sides+1) - 1) + 1;
}

void Die::reset(){
    qsrand(QTime::currentTime().msec());
}

Die::~Die(){}


