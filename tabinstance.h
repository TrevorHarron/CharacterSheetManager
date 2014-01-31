#ifndef TABINSTANCE_H
#define TABINSTANCE_H

#include <QPair>
#include <QHash>
#include <QVector>

class Character;
class QString;
class QLineEdit;
class QLabel;
class QVBoxLayout;

class TabInstance{
public:
    TabInstance(Character*, QHash<QString,QLineEdit*>*,
                  QVector<QPair<QLineEdit*,QLabel*> >*,
                  QVBoxLayout*);

    TabInstance(Character*, QHash<QString,QLineEdit*>*,
                  QVector<QPair<QLineEdit*,QLabel*> >*);

    TabInstance(Character*,QVBoxLayout*);
    ~TabInstance();

    Character *character;
    QHash<QString,QLineEdit*> *lineEdits;
    QVector<QPair<QLineEdit*,QLabel*> > *linesLabels;
    QVBoxLayout *layout;
};

#endif // TABINSTANCE_H
