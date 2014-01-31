#include "mainwindow.h"
#include "tabinstance.h"
#include "character.h"

#include <QLinkedList>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QMenuBar>
#include <QTextEdit>
#include <QLabel>
#include <QDebug>

QString MainWindow::TITLE_DND = "Character Manager (DnD creation mode)";
QString MainWindow::TITLE_DH = "Character Manager (DH creation mode)";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QSize maxSize(512,1024);
    setMaximumSize(maxSize);
    setWindowTitle(TITLE_DND);

    maxTabIndex = cTab = 0;
    factory = new CharacterFactory();//Defaults to DnD

    //Top Menu Setup
    QMenu *newMenu = new QMenu("&New");
    makeOption(newMenu,"New Dungeons and Dragons Character", SLOT(newDnDChar()));
    makeOption(newMenu, "New Dark Heresy Character",SLOT(newDHChar()));

    QMenu *fileMenu = new QMenu("&File");
    fileMenu->addMenu(newMenu);
    makeOption(fileMenu,"&Save", SLOT(save()));
    makeOption(fileMenu, "&Load", SLOT(load()));
    makeOption(fileMenu,"&Clear",SLOT(clear()));
    makeOption(fileMenu,"&Undo",SLOT(undo()));
    makeOption(fileMenu, "&Exit",SLOT(close()));

    QMenuBar *bar = new QMenuBar();
    bar->addMenu(fileMenu);
    makeOption(bar, "&Add Tab",SLOT(addTab()));
    makeOption(bar, "&Remove Tab", SLOT(removeTab()));

    setMenuBar(bar);
    menuBar()->setNativeMenuBar(false);

    tabInfo = new QHash<int, TabInstance*>();
    tabSaves = new QHash<int,bool>();
    undoStacks = new QHash<int,QLinkedList<CharacterMemento>*>();

    tabs  = new QTabWidget();

    //Initial Tab and window setup
    setCentralWidget(tabs);
    makeWindow(tabs, QString::number(cTab), cTab);
    connect(tabs,SIGNAL(currentChanged(int)),this,SLOT(tabChange(int)));

    tabs->setTabsClosable(true);
    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(removeTab(int)));

    //Setting of nessesary variables
    cTab = tabs->currentIndex();
    tabSaves->insert(cTab , false);
    dir = "";
}

//=====================//
// GUI making fuctions //
//=====================//
void MainWindow::makeWindow(QTabWidget* tabs, QString name, int index){

    QWidget *main = new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout();
    main->setLayout(mainLayout);

    QVBoxLayout *characterLayout = new QVBoxLayout();
    mainLayout->addLayout(characterLayout);
    characterLayout->setAlignment(Qt::AlignTop);

    //Character side of the Layout
    characterLayout->addWidget(new QLabel("Character"));

    //Essentials for initial character
    TabInstance *info;
    try{
        info = new TabInstance(factory->createCharacter(), characterLayout);
    } catch (char e){
        qDebug() << "Unknown Character Type " << e;
        exit(e);
    }
        Character *cCharacter = info->character;
        characterHash = cCharacter->toHash();
        vStats = cCharacter->getStatVector();
        tabInfo->insert(index,info);


    makeStatFields();

    makeActions(mainLayout);

    //Setting the Scroll Area
    QScrollArea *scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setWidget(main);

    tabs->insertTab(index,scroll,name);
    tabs->setTabEnabled(index,true);
    undoStacks->insert(cTab, new QLinkedList<CharacterMemento>());
}

// Character Layout Creation methods //
void MainWindow::makeStatFields(){
    QVector<QString>::Iterator itCharacter;
    for(itCharacter = vStats->begin(); itCharacter != vStats->end(); ++itCharacter){
        QString key = *itCharacter;
        QString value = *(characterHash->find(key));
        addStatFields(key, value);
    }
}
//In the case of adding addtional fields
void MainWindow::addStatFields(QString key, QString value){
    TabInstance *info = tabInfo->value(cTab);
    if(!info->lineEdits->contains(key)){

        QHBoxLayout *layout = new QHBoxLayout();
        layout->setAlignment(Qt::AlignJustify);
        info->layout->addLayout(layout);

        QLineEdit *lineEdit = new QLineEdit(value);
        lineEdit->setAlignment(Qt::AlignRight);
        lineEdit->setAccessibleName(key);
        lineEdit->setMaximumWidth(64);
        layout->addWidget(lineEdit);

        QLabel *label = new QLabel(key);
        label->setAlignment(Qt::AlignLeft);
        label->setMinimumWidth(80);
        layout->addWidget(label);

        info->linesLabels->push_back(qMakePair(lineEdit,label));
        info->lineEdits->insert(key,lineEdit);
        connect(lineEdit, SIGNAL(textEdited(QString)), this, SLOT(textChanged(QString)));

        characterHash->insert(key,value);
    }
}

//Action Side of Layout
void MainWindow::makeActions(QHBoxLayout* mainLayout){
    //Actions side of layout
    QVBoxLayout *actionLayout = makeLayout(mainLayout);

    makeLabel(actionLayout, "Actions: ");
    QVBoxLayout *addSkillLayout = makeLayout(actionLayout);

    makeLabel(addSkillLayout,"Skill Name");
    skillName = makeEdit(addSkillLayout,"nSkill");

    makeLabel(addSkillLayout,"Value");
    skillValue = makeEdit(addSkillLayout,"vSkill");

    makeButton(addSkillLayout,"Add Skill",SLOT(addSkill()));
    makeButton(addSkillLayout,"Remove Skill",SLOT(removeSkill()));

    QVBoxLayout *rollLayout = makeLayout(actionLayout);
    makeButton(rollLayout,"Melee or Str Attack", SLOT(rollAtk()));
    makeButton(rollLayout, "Ranged or Dex Attack",SLOT(rollAtk()));

    Character* cCharacter = tabInfo->value(cTab)->character;

    QList<QString> list = cCharacter->getSaves()->keys();
    saves = makeComboBox(rollLayout,list,128);
    makeButton(rollLayout, "Roll Save",SLOT(rollSave()));

    makeLabel(rollLayout, "Skill Name");
    list = cCharacter->getSkills()->keys();
    skills = makeComboBox(rollLayout,list,128);
    skills->addItem("None");

    makeLabel(rollLayout, "Stat Used");
    list = cCharacter->getStats()->keys();
    stats = makeComboBox(rollLayout,list,128);

    makeLabel(rollLayout,"Bonus:");
    bonusText = makeEdit(rollLayout,"bonus");
    connect(bonusText,SIGNAL(textChanged(QString)),this,SLOT(textChanged(QString)));
    makeButton(rollLayout,"Roll Skill", SLOT(rollSkill()));

    QVBoxLayout *statusLayout = makeLayout(actionLayout);

    healthStatus = new QLabel("Health Status:");
    statusLayout->addWidget(healthStatus);

    statusLayout->addWidget(new QLabel("Feats and Talents:"));

    feats = new QTextEdit();
    feats->setMinimumSize(QSize(128,128));
    feats->setMaximumSize(QSize(128,256));
    connect(feats,SIGNAL(textChanged()), this, SLOT(textChanged()));
    statusLayout->addWidget(feats);
}

//========================================//
// Widget Makers:                         //
// Basically creating a certain widget    //
// and allocating it into a parent widget //
//========================================//
void MainWindow::makeOption(QMenuBar* menu, QString name, const char* method){
    QAction *action = menu->addAction(name);
    action->setCheckable(false);
    connect(action, SIGNAL(triggered()), this, method);
}

void MainWindow::makeOption(QMenu* menu, QString name, const char* method){
    QAction *action = menu->addAction(name);
    action->setCheckable(false);
    connect(action, SIGNAL(triggered()), this, method);
}

void MainWindow::makeLabel(QLayout* layout, QString name, int width){
    QLabel *label = new QLabel(name);
    label->setAlignment(Qt::AlignTop);
    label->setMaximumWidth(width);
    layout->addWidget(label);
}

QLineEdit* MainWindow::makeEdit(QLayout* layout, QString name, QString value, int width){
    QLineEdit *edit = new QLineEdit(value);
    edit->setAlignment(Qt::AlignTop);
    edit->setAccessibleName(name);
    layout->addWidget(edit);
    edit->setMaximumWidth(width);
    return edit;
}

void MainWindow::makeButton(QLayout* layout, QString name, const char* method){
    QPushButton *button = new QPushButton(name);
    connect(button, SIGNAL(clicked()),this, method);
    layout->addWidget(button);
    button->setMaximumWidth(200);
}

QComboBox* MainWindow::makeComboBox(QLayout* layout, QList<QString> items, int width){
    QComboBox *combo = new QComboBox();
    if(!items.isEmpty())
        combo->addItems(items);
    combo->setEditable(false);
    layout->addWidget(combo);
    combo->setMaximumWidth(width);
    return combo;
}

QVBoxLayout* MainWindow::makeLayout(QBoxLayout* parent){
    QVBoxLayout *layout = new QVBoxLayout();
    parent->addLayout(layout);
    layout->setAlignment(Qt::AlignTop);
    return layout;
}

/*SLOT METHODS */
//================================//
// Menu SLOTs  and Helper methods //
//================================//
//New methods
void MainWindow::newDnDChar(){
    factory->setCharSystem(CharacterFactory::DnD);
    addTab();
    setWindowTitle(TITLE_DND);
}

void MainWindow::newDHChar(){
    factory->setCharSystem(CharacterFactory::DH);
    addTab();
    setWindowTitle(TITLE_DH);
}

// IO File Handling //
void MainWindow::save(){
    if(maxTabIndex < 0) return;

    QFileDialog *dialog = new QFileDialog(this,"Enter filename to save", dir,"XML Files (*.xml)");
    dialog->setDefaultSuffix("xml");
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    dialog->exec();

    QString fileName = dialog->selectedFiles().first();
    dir = dialog->directory().absolutePath();
    delete dialog;

    if(fileName.isEmpty()) return;

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)){
        QMessageBox::warning(this, "Input/Output Error", "Failed to save");
        return;
     }

    QXmlStreamWriter *xmlWriter  = new QXmlStreamWriter(&file);
    xmlWriter->setAutoFormatting(true);

    xmlWriter->writeStartDocument();
    writeCharacter(xmlWriter);
    xmlWriter->writeEndDocument();

    delete xmlWriter;
    tabSaves->insert(cTab,false);
    file.close();
}

void MainWindow::writeCharacter(QXmlStreamWriter* xmlWriter){

    xmlWriter->writeStartElement("character");
    xmlWriter->writeAttribute("system", tabInfo->value(cTab)->character->getSystem());
    //Write
    QVector<QPair<QLineEdit*,QLabel*> > *pairs = tabInfo->value(cTab)->linesLabels;
    QVector<QPair<QLineEdit*,QLabel*> >::iterator itLines;
    for(itLines = pairs->begin(); itLines != pairs->end(); ++itLines){
        QPair<QLineEdit*,QLabel*> pair = *itLines;

        QString key = pair.first->accessibleName();
        xmlWriter->writeEmptyElement(key);
        xmlWriter->writeAttribute("value", tabInfo->value(cTab)->character->getValue(key));
    }
    writeFeats(xmlWriter);
    xmlWriter->writeEndElement();
}

void MainWindow::writeFeats(QXmlStreamWriter *xmlWriter){
    xmlWriter->writeStartElement("feats");
    xmlWriter->writeAttribute("value",feats->toPlainText());
    xmlWriter->writeEndElement();
}

void MainWindow::load(){
    if(!needToSave()){
        QFileDialog *dialog = new QFileDialog(this,"Enter filename to load", dir,"XML Files (*.xml)");
        dialog->exec();

        QString fileName = dialog->selectedFiles().first();
        dir = dialog->directory().absolutePath();
        delete dialog;

        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::warning(this, "Input/Output Error", "Failed to load");
            return;
        }

        if(maxTabIndex < 0) addTab();

        QXmlStreamReader *xmlReader = new QXmlStreamReader(&file);
        readCharacter(xmlReader);
        //Additional reads like inventory

        skills->removeItem(skills->findText(skillName->text()));
        delete xmlReader;

        redoComboBoxes();
        clearUndoStack(cTab);
        undoStacks->insert(cTab,new QLinkedList<CharacterMemento>());
        tabSaves->insert(cTab, false);

        file.close();
    }
}

void MainWindow::readCharacter(QXmlStreamReader* xmlReader){
    QString key, value;
    bool readSys = false;
    QHash<QString, QLineEdit*> *lineEdit = tabInfo->value(cTab)->lineEdits;
    while(!xmlReader->atEnd()){
        xmlReader->readNext();

        //For a loaded character check the system it uses and set up accordingly
        if(xmlReader->isStartElement() && xmlReader->name() == "character" && !readSys){
            cleanLayout();

            QString sys = xmlReader->attributes().value("system").toString();

            TabInstance *info = tabInfo->value(cTab);
            delete info->character;

            int characterSys = (sys == "dh") ?
                                characterSys = CharacterFactory::DH :
                                characterSys = CharacterFactory::DnD;

            factory->setCharSystem(characterSys);
            try{
                info->character = factory->createCharacter();
            } catch(char e){
                qDebug() << "unknown character type " << e;
                exit(e);
            }
            characterHash = info->character->toHash();
            vStats = info->character->getStatVector();

            makeStatFields();
            readSys = true;
        }
        if(xmlReader->isStartElement() && xmlReader->name() != "character"){

            if (xmlReader->name() == "feats")
                readFeats(xmlReader);
            else {
                key = xmlReader->name().toString();
                value = xmlReader->attributes().value("value").toString();

                tabInfo->value(cTab)->character->setValue(key,value);
                characterHash->insert(key,value);
                addStatFields(key, value);
                lineEdit->value(key)->setText(value);
            }
        } else if((xmlReader->isEndElement() && xmlReader->name() == "character")) break;

        if(xmlReader->hasError())
            QMessageBox::warning(this, "QXMLStreamError", xmlReader->errorString());
    }
}

void MainWindow::readFeats(QXmlStreamReader *xmlReader){
    QString strFeats = xmlReader->attributes().value("value").toString();
    tabInfo->value(cTab)->character->setFeats(strFeats);
    feats->setText(tabInfo->value(cTab)->character->getFeats());
}

//Clear the current tab and set everything to 0
void MainWindow::clear(){
    if(maxTabIndex < 0) return;
    if(!needToSave()){

        updateUndoStack();

        TabInstance *info = tabInfo->value(cTab);
        delete info->character;
        try{
            info->character = factory->createCharacter();
        } catch (char e){
            qDebug() << "Unknown Character Type " << e;
            exit(e);
        }

        Character *cCharacter = info->character;
        characterHash = cCharacter->toHash();
        vStats = cCharacter->getStatVector();

        tabInfo->insert(cTab,info);

        for(int index = 0; index < info->linesLabels->size(); ++index){
            QLineEdit *edit = info->linesLabels->at(index).first;
            QString key = info->lineEdits->key(edit);
            if(characterHash->contains(key))
                info->lineEdits->value(key)->setText(characterHash->value(key));
            else {
                delete info->linesLabels->at(index).first;
                delete info->linesLabels->at(index).second;
                info->linesLabels->remove(index);
                info->lineEdits->remove(key);
            }
        }
        feats->setText("");
        tabSaves->insert(cTab,true);
    }
}

void MainWindow::undo(){
    if(maxTabIndex < 0) return;
    if(!undoStacks->value(cTab)->isEmpty()){

        CharacterMemento memento = undoStacks->value(cTab)->back();
        QHash<QString,QString> state = memento.getCharacterState();

        Character* cCharacter = tabInfo->value(cTab)->character;
        QList<QString> cKeys = characterHash->keys();

        for( QList<QString>::Iterator itKeys = cKeys.begin(); itKeys != cKeys.end(); ++itKeys){
            QString key  = *itKeys;
            if(state.contains(key))
                cCharacter->setValue(key, state.value(key));
            else
                cCharacter->removeValue(key);
        }

        characterHash = cCharacter->toHash();
        vStats = cCharacter->getStatVector();

        cleanLayout();
        makeStatFields();
        redoComboBoxes();
        cCharacter->setFeats(memento.getCharacterFeats());
        feats->setText(cCharacter->getFeats());


        memento.~CharacterMemento();
        undoStacks->value(cTab)->pop_back();
    }
}

//====================//
// Skill Button SLOTs //
//====================//
void MainWindow::addSkill(){
    QString key = skillName->text();
    QString value = skillValue->text();

    int amount = value.toInt(0,10);
    if(value == QString::number(amount)){
        updateUndoStack();

        tabInfo->value(cTab)->character->setValue(key, amount);
        addStatFields(key, value);
        skills->addItem(key);
        tabSaves->insert(cTab,true);
    }else{
        QMessageBox::warning(this, "Conversion Error", "Please input an integer");
        return;
    }
}

void MainWindow::removeSkill(){

    TabInstance *info = tabInfo->value(cTab);

    if(!info->character->getSkills()->contains(skillName->text())){
        QMessageBox::warning(this, "Error", "That is not a skill, please name a skill.");
        return;
    }

    QVector<QPair<QLineEdit*, QLabel*> >::Iterator itLines;
    for(itLines = info->linesLabels->begin(); itLines != info->linesLabels->end(); ++itLines){
        QPair<QLineEdit*, QLabel*> cPair = *itLines;

         if(cPair.first == info->lineEdits->value(skillName->text())){

             updateUndoStack();

             delete cPair.first;
             delete cPair.second;
             info->linesLabels->remove(info->linesLabels->indexOf(cPair));
             info->lineEdits->remove(skillName->text());

             tabSaves->insert(cTab,true);
             skills->removeItem(skills->findText(skillName->text()));
             break;
         }
    }
}

//===============//
// Rolling SLOTS //
//===============//
void MainWindow::rollAtk(){
    QString text = dynamic_cast<QPushButton*>(QObject::sender())->text();
    Character *cCharacter = tabInfo->value(cTab)->character;
    QString mod;
    if (text == "Melee or Str Attack")
        mod = "strength";
    else
        mod = "dexterity";
    int result = cCharacter->makeAttack("attack",mod);
    if(cCharacter->getSystem() == "dnd"){
        int roll = result - cCharacter->getMod(mod) - cCharacter->getValue("bab").toInt();
        if(roll == 20) QMessageBox::information(this,"Wow!", "Natural 20!");
        if(roll == 1) QMessageBox::information(this,"Ouch...", "Natural 1.. You miss...");
    }
    QMessageBox::information(this, "Result", "You rolled a " + QString::number(result));
}

void MainWindow::rollSave(){
    QString save = saves->currentText();
    Character *cCharacter = tabInfo->value(cTab)->character;

    QString mod = "base_mod";
    int result =  cCharacter->makeSave(save, mod);

    QMessageBox::information(this, "Result", "Your " + save +" save is a " + QString::number(result));
}

void MainWindow::rollSkill(){
    int bonus = bonusText->text().toInt();
    QString type = skills->currentText();
    QString mod = stats->currentText();

    Character* cCharacter = tabInfo->value(cTab)->character;
    int result = cCharacter->makeRoll(type,mod,bonus);

    QMessageBox::information(this, "Result", "You rolled a " + QString::number(result)+ " for the skill: " + type);
}

//===================//
// TAB RELATED SLOTS //
//===================//
void MainWindow::tabChange(int index){
    cTab = index;
    if(index == -1) return;

    Character *cCharacter = tabInfo->value(cTab)->character;
    vStats = cCharacter->getStatVector();
    characterHash = cCharacter->toHash();

    makeStatFields();
}

void MainWindow::addTab(){
    ++maxTabIndex;
    cTab = maxTabIndex;
    //Insert selecting character type
    makeWindow(tabs, QString::number(cTab), cTab);
    cTab = tabs->currentIndex();
}

void MainWindow::removeTab(){
    removeTab(cTab);
}

void MainWindow::removeTab(int index){

    if(!needToSave()){
        tabs->removeTab(index);
        if(index < maxTabIndex){
            //since tabs automatically shifts keys shift keys here
            for(int key = index; key < maxTabIndex; key++){
                tabInfo->insert(key, tabInfo->value(key+1));
                tabSaves->insert(key,tabSaves->value(key+1));
                undoStacks->insert(key,undoStacks->value(key+1));
            }
            //and remove final tab
            tabInfo->remove(maxTabIndex);
            tabSaves->remove(maxTabIndex);
            undoStacks->remove(maxTabIndex);
        }
       --maxTabIndex;
        cTab = tabs->currentIndex();
    }
}

//=================//
// Line Edit Slots //
//=================//
void MainWindow::textChanged(QString value){

    QLineEdit *edit = dynamic_cast<QLineEdit*>(QObject::sender());
    QString key = edit->accessibleName();
    if(!(key == "name" || key == "race" || key == "class") && !(value == "" || value == "-")){
        int numVal = value.toInt();
        if(QString::number(numVal) != value){
            QMessageBox::warning(this, "Conversion Error", "Please input a number");
            edit->setText("");
            tabSaves->insert(cTab, true);
            return;
        } else if(key == "current_hp" || key == "current_wounds"){
            int maxHP;
            if(tabInfo->value(cTab)->character->getSystem() == "dh")
                maxHP = characterHash->value("max_wounds").toInt();
            else
                maxHP = characterHash->value("max_health").toInt();
            if(numVal > maxHP){
                edit->setText(QString::number(maxHP));
                healthStatus->setText("STATUS: " + characterHash->value("name")+ " is fine.");
            } else if(numVal > maxHP/4 && numVal <= maxHP/2)
                healthStatus->setText("STATUS: " + characterHash->value("name")+ " is hurting.");
            else if(numVal <= maxHP/4 && numVal >= 0)
                healthStatus->setText("STATUS: " + characterHash->value("name")+ " is bloody.");
            else if(numVal <= -10)
                healthStatus->setText("STATUS: " + characterHash->value("name")+ " is dead.");
            else if(numVal < 0)
                healthStatus->setText("STATUS: " + characterHash->value("name")+ " is unconscious\n or in critical condition.");
            else
                healthStatus->setText("STATUS: " + characterHash->value("name")+ " is fine.");
        }
    }
    updateUndoStack();

    tabInfo->value(cTab)->character->setValue(key,value);
    characterHash->insert(key,value);

    if(key == "name") tabs->setTabText(cTab,value);

    if(!(key == "bonus")) tabSaves->insert(cTab, true);
}

void MainWindow::textChanged(){
    updateUndoStack();
    tabInfo->value(cTab)->character->setFeats(feats->toPlainText());
    tabSaves->insert(cTab, true);
}

//=====================//
//Other helper fuctions//
//=====================//
void MainWindow::updateUndoStack(){
    Character *cCharacter = tabInfo->value(cTab)->character;
    CharacterMemento memento = cCharacter->createMemento();
    if (!undoStacks->value(cTab)->isEmpty() && memento == undoStacks->value(cTab)->back()) return;
    if(undoStacks->value(cTab)->size() >= 50)
        undoStacks->value(cTab)->pop_front(); //prevent infinite growth
    undoStacks->value(cTab)->push_back(memento);
}

void MainWindow::clearUndoStack(int index){
    QLinkedList<CharacterMemento> *undoStack = undoStacks->value(index);
    while(!undoStack->isEmpty()){
        undoStack->back().~CharacterMemento();
        undoStack->pop_back();
    }
}

// Cleanup the layout when its not valid
void MainWindow::cleanLayout(){
    TabInstance *info = tabInfo->value(cTab);
    for(int index = 0; index < info->linesLabels->size(); ++index){
        delete info->linesLabels->at(index).first;
        delete info->linesLabels->at(index).second;
    }
    info->lineEdits->clear();
    info->linesLabels->clear();
}

void MainWindow::redoComboBoxes(){

    TabInstance *info = tabInfo->value(cTab);
    saves->clear();
    saves->addItems(info->character->getSaves()->keys());

    skills->clear();
    skills->addItems(info->character->getSkills()->keys());
    skills->addItem("None");

    stats->clear();
    stats->addItems(info->character->getStats()->keys());

    tabs->setTabText(cTab,info->character->getValue("name"));
}

bool MainWindow::needToSave(){
    return needToSave(false);
}

bool MainWindow::needToSave(bool isExit){
    if (maxTabIndex < 0) return false;
    if(tabSaves->value(cTab) || (isExit && anySavesLeft())){
        QMessageBox messageBox;
        messageBox.setText("The document has been modified.");
        messageBox.setInformativeText("Do you want to save changes");
        messageBox.setStandardButtons(QMessageBox::Save| QMessageBox::Discard| QMessageBox::Cancel);
        messageBox.setDefaultButton(QMessageBox::Save);
        int ret = messageBox.exec();
        if(ret == QMessageBox::Save){
            save();
            if(tabSaves->value(cTab)) return true;
            return false;
        }
        if(ret == QMessageBox::Cancel) return true;
    }
    return false;
}

bool MainWindow::anySavesLeft(){
    QList<bool> values = tabSaves->values();
    return values.contains(true);
}

void MainWindow::closeEvent(QCloseEvent *event){
     if(needToSave(true)){
        event->ignore();
        return;
     } else QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow(){
    tabSaves->clear();
    for(int tabNum = 0; tabNum <= maxTabIndex; ++tabNum){
        tabInfo->value(tabNum)->~TabInstance();
        clearUndoStack(tabNum);
        undoStacks->remove(tabNum);
    }
}
