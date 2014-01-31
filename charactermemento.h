#ifndef CHARACTERMEMENTO_H
#define CHARACTERMEMENTO_H

#include <QHash>
#include <QString>

class CharacterMemento
{
    QHash<QString,QString> characterState;
public:
    CharacterMemento(QHash<QString, QString> _characterState);
    ~CharacterMemento();
    QHash<QString,QString> getCharacterState();
};

#endif // CHARACTERMEMENTO_H
