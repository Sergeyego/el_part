#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "models.h"
#include "db/dbmapper.h"
#include "dialogloadchem.h"
#include <QSqlQuery>
#include <QSettings>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    ModelPart *modelPart;
    DbMapper *mapper;
    ModelChemSrc *modelChem;
    ModelMechSrc *modelMech;
    ModelRab *modelRab;
    ModelMix *modelMix;
    ModelGlass *modelGlass;
    ModelConsStatData *modelConsStatData;
    ModelConsStatPar *modelConsStatPar;
    int currentIdPart();
    void loadSettings();
    void saveSettings();

private slots:
    void updPart();
    void refreshCont(int ind);
    void loadChem();
    void refreshGlassData(QModelIndex index);
    void insertMark(int rcpIndex);
    void insertProvol(int markIndex);
    void copyPar();
    void insertChemSamp();
    void lockChemSampCh();
    void lockChangedMap(bool lock);
    void setCurrentChemDev(int index);
    void setDefaultPack();
};

#endif // MAINWIDGET_H
