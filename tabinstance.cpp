#include "tabinstance.h"
#include "character.h"

#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>

//===================================//
// Container for helping manage tabs //
// Esentially it holds the character,//
// LineEdits, Pairs, and layout      //
// used by the tabs consistantly     //
//===================================//
TabInstance::TabInstance(Character* _character, QHash<QString, QLineEdit*> *_lineEdits,
                             QVector<QPair<QLineEdit*, QLabel*> > *_linesLabels, QVBoxLayout* _layout)
    : character(_character), lineEdits(_lineEdits), linesLabels(_linesLabels), layout(_layout)
{}

TabInstance::TabInstance(Character* _character,QHash<QString, QLineEdit*> *_lineEdits,
                             QVector<QPair<QLineEdit*, QLabel*> > *_linesLabels)
    : character(_character), lineEdits(_lineEdits), linesLabels(_linesLabels), layout(new QVBoxLayout)
{}
TabInstance::TabInstance(Character* _character, QVBoxLayout* _layout) :
    character(_character), lineEdits(new QHash<QString, QLineEdit*>),
    linesLabels(new QVector<QPair<QLineEdit*, QLabel*> >), layout(_layout)
{}

TabInstance::~TabInstance(){
    delete character;
    //other items handled by their parent class
}
