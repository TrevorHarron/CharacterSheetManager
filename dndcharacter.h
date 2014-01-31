#ifndef DNDCHARACTER_H
#define DNDCHARACTER_H

#include <QString>
#include <QHash>
#include <QVector>
#include "character.h"

class DnDCharacter : public Character{
public:
    DnDCharacter();
    ~DnDCharacter();

    int makeAttack(QString type,QString mod, int bonus = 0);
    int makeSave(QString type, QString mod, int bonus = 0);
    int makeRoll(QString type, QString mod, int bonus = 0);

    int getMod(QString key);
};

#endif // DNDCHARACTER_H
