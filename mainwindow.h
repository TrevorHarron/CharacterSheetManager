#ifndef CHARACTERWINDOW_H
#define CHARACTERWINDOW_H

#include <QMainWindow>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "characterfactory.h"
#include "charactermemento.h"

class Character;
class QLineEdit;
class QCloseEvent;
class QVBoxLayout;
class QHBoxLayout;
class QBoxLayout;
class QLabel;
class QMenuBar;
class TabInstance;
class QComboBox;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    static QString TITLE_DND;
    static QString TITLE_DH;

    int cTab;
    int maxTabIndex;
    QString dir;
    CharacterFactory *factory;

    QTabWidget *tabs;
    QHash<int,TabInstance*> *tabInfo;
    QHash<int,bool> *tabSaves;
    QHash<int,QLinkedList<CharacterMemento>*> *undoStacks; //QList used to limit stack size

    QHash<QString,QString> *characterHash;
    QVector<QString> *vStats;

    QLineEdit *skillName;
    QLineEdit *skillValue;

    QComboBox *skills;
    QComboBox *stats;
    QComboBox *saves;

    QLineEdit *bonusText;

    QLabel *healthStatus;
    QTextEdit *feats;

    void makeWindow(QTabWidget* tabs, QString name, int index);
    void makeActions(QHBoxLayout* layout);
    void makeOption(QMenuBar* menu,QString name,const char* method);
    void makeOption(QMenu* menu, QString name,const char* method);
    void makeLabel(QLayout* layout, QString name, int width = 100);
    void makeButton(QLayout* layout, QString name, const char* method);
    QLineEdit* makeEdit(QLayout* layout, QString name, QString value = "", int width = 200);
    QComboBox* makeComboBox(QLayout* layout, QList<QString> items, int width = 200);
    QVBoxLayout* makeLayout(QBoxLayout* layout);

    void makeStatFields();
    void addStatFields(QString key, QString value);
    void cleanLayout();
    void redoComboBoxes();

    void updateUndoStack();
    void clearUndoStack(int index);

    bool needToSave();
    bool needToSave(bool isExit);
    bool anySavesLeft();

    void writeCharacter(QXmlStreamWriter* xmlWriter);
    void writeFeats(QXmlStreamWriter* xmlWriter);

    void readCharacter(QXmlStreamReader* xmlReader);
    void readFeats(QXmlStreamReader* xmlReader);

public:
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    ~MainWindow();
public slots:
    void save();
    void load();
    void undo();
    void newDnDChar();
    void newDHChar();
    void clear();
    void addSkill();
    void removeSkill();
    void rollAtk();
    void rollSave();
    void rollSkill();
    void tabChange(int index);
    void addTab();
    void removeTab();
    void removeTab(int index);
    void textChanged(QString value);
    void textChanged();
};

#endif // CHARACTERWINDOW_H
