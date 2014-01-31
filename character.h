#ifndef CHARACTER_H
#define CHARACTER_H

#include <QString>
#include <QHash>
#include <QVector>
#include "charactermemento.h"

class Die;

class Character
{

public:
    ~Character();

    virtual int makeAttack(QString type,QString mod, int bonus = 0) = 0;
    virtual int makeSave(QString type, QString mod, int bonus = 0) = 0;
    virtual int makeRoll(QString type, QString mod, int bonus = 0) = 0;

    virtual int getMod(QString key) = 0;

    void setValue(QString key, QString value);
    void setValue(QString key, int value);
    QString getValue(QString key);
    void removeValue(QString key);

    QString getFeats();
    void setFeats(QString _feats);

    QString getSystem();

    bool containsValue(QString key);

    QHash<QString,int>* getStats();
    QHash<QString,int>* getSaves();
    QHash<QString,int>* getSkills();

    QHash<QString,QString>* toHash();
    CharacterMemento createMemento();
    QVector<QString>* getStatVector();

protected:
    QString system;
    QString feats;
    Die *die;
    QVector<QString> ID_NAMES;
    QVector<QString> BASE_STAT_NAMES;
    QVector<QString> SAVE_STAT_NAMES;
    QVector<QString> allStatNames;
    QHash<QString, QString> attributes;

    void initAttributes();
    bool isSkill(QString key);

};

class Die {
public:
    Die(int _sides);
    ~Die();
    int roll();
    void reset();
private:
    int sides;
};

#endif // CHARACTER_H
