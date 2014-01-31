#ifndef DHCHARACTER_H
#define DHCHARACTER_H

#include <QString>
#include <QHash>
#include <QVector>
#include "character.h"

class DHCharacter : public Character
{
public:
    DHCharacter();
    ~DHCharacter();

    int makeAttack(QString type,QString mod, int bonus = 0);
    int makeSave(QString type, QString mod, int bonus = 0);
    int makeRoll(QString type, QString mod, int bonus = 0);

    int getMod(QString key);
};

#endif // DHCHARACTER_H
