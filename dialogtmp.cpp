#include "dialogtmp.h"
#include "ui_dialogtmp.h"

DialogTmp::DialogTmp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTmp)
{
    ui->setupUi(this);
    modelChem = new ModelChemSrc(this);
    ui->tableViewChem->setModel(modelChem);
    ui->tableViewChem->setModel(modelChem);
    ui->tableViewChem->setColumnHidden(0,true);
    ui->tableViewChem->setColumnHidden(1,true);
    ui->tableViewChem->setColumnWidth(2,80);
    ui->tableViewChem->setColumnWidth(3,70);
    ui->tableViewChem->setColumnWidth(4,70);
    ui->tableViewChem->setColumnWidth(5,110);
    connect(ui->pushButtonOk,SIGNAL(clicked(bool)),this,SLOT(accept()));
}

DialogTmp::~DialogTmp()
{
    delete ui;
}

void DialogTmp::load(int id_part, int id_dev, QList<int> ids)
{
    QString flt;
    foreach (int id, ids) {
        if (!flt.isEmpty()){
            flt+=", ";
        }
        flt+=QString::number(id);
    }
    flt="id in ("+flt+")";
    modelChem->setDefaultValue(4,id_dev);
    modelChem->refresh(id_part,flt);
}
