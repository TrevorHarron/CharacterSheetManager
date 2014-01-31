#ifndef CHARACTERMEMENTO_H
#define CHARACTERMEMENTO_H

#include <QHash>
#include <QString>

class CharacterMemento
{
    QHash<QString,QString> characterState;
    QString feats;
public:
    CharacterMemento(QHash<QString, QString> _characterState, QString _feats);
    ~CharacterMemento();
    QHash<QString,QString> getCharacterState();
    QString getCharacterFeats();
    bool operator ==(CharacterMemento other);
};

#endif // CHARACTERMEMENTO_H
