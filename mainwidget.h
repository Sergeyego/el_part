#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "models.h"
#include "db/dbmapper.h"
#include "dialogloadchem.h"
#include <QSqlQuery>

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

private slots:
    void updPart();
    void updPartFlf(QString s);
    void refreshCont(int ind);
    void loadChem();
};

#endif // MAINWIDGET_H
