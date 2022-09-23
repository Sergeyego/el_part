#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    loadSettings();

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(QDate(QDate::currentDate().year(),12,31));
    ui->pushButtonUpd->setIcon(this->style()->standardIcon(QStyle::SP_BrowserReload));

    ui->comboBoxChemDev->setModel(Rels::instance()->relChemDev->model());
    ui->comboBoxChemDev->setModelColumn(1);

    modelChem = new ModelChemSrc(this);
    ui->tableViewChem->setModel(modelChem);
    ui->tableViewChem->setColumnHidden(0,true);
    ui->tableViewChem->setColumnHidden(1,true);
    ui->tableViewChem->setColumnWidth(2,80);
    ui->tableViewChem->setColumnWidth(3,70);
    ui->tableViewChem->setColumnWidth(4,70);
    ui->tableViewChem->setColumnWidth(5,110);

    if (ui->comboBoxChemDev->model()->rowCount()){
        setCurrentChemDev(ui->comboBoxChemDev->currentIndex());
    }

    modelMech = new ModelMechSrc(this);
    ui->tableViewMech->setModel(modelMech);
    ui->tableViewMech->setColumnHidden(0,true);
    ui->tableViewMech->setColumnWidth(1,180);
    ui->tableViewMech->setColumnWidth(2,80);

    modelRab = new ModelRab(this);
    ui->tableViewPress->setModel(modelRab);
    ui->tableViewPress->setColumnHidden(0,true);
    ui->tableViewPress->setColumnHidden(1,true);
    ui->tableViewPress->setColumnWidth(2,75);
    ui->tableViewPress->setColumnWidth(3,120);
    ui->tableViewPress->setColumnWidth(4,120);
    ui->tableViewPress->setColumnWidth(5,65);
    ui->tableViewPress->setColumnWidth(6,65);
    ui->tableViewPress->setColumnWidth(7,65);
    ui->tableViewPress->setColumnWidth(8,65);
    ui->tableViewPress->setColumnWidth(9,65);

    modelMix = new ModelMix(this);
    ui->tableViewDoz->setModel(modelMix);
    ui->tableViewDoz->setColumnHidden(0,true);
    ui->tableViewDoz->setColumnWidth(1,350);
    ui->tableViewDoz->setColumnWidth(2,70);

    modelGlass = new ModelGlass(this);
    ui->tableViewGlass->setModel(modelGlass);
    ui->tableViewGlass->setColumnHidden(0,true);
    ui->tableViewGlass->setColumnHidden(1,true);
    ui->tableViewGlass->setColumnWidth(2,100);
    ui->tableViewGlass->setColumnWidth(3,300);

    modelConsStatData = new ModelConsStatData(this);
    modelConsStatData->refresh(-1);
    ui->tableViewGlassCont->setModel(modelConsStatData);
    ui->tableViewGlassCont->setColumnHidden(0,true);
    ui->tableViewGlassCont->setColumnWidth(1,40);
    ui->tableViewGlassCont->setColumnWidth(2,70);
    ui->tableViewGlassCont->setColumnWidth(3,50);
    ui->tableViewGlassCont->setColumnWidth(4,50);
    ui->tableViewGlassCont->setColumnHidden(5,true);

    modelConsStatPar = new ModelConsStatPar(this);
    modelConsStatPar->refresh(-1,-1);
    ui->tableViewGlassPar->setModel(modelConsStatPar);
    ui->tableViewGlassPar->setColumnHidden(0,true);
    ui->tableViewGlassPar->setColumnWidth(1,80);
    ui->tableViewGlassPar->setColumnWidth(2,60);
    ui->tableViewGlassPar->setColumnWidth(3,50);
    ui->tableViewGlassPar->setColumnWidth(4,60);

    modelPart = new ModelPart(this);
    ui->tableViewPart->setModel(modelPart);
    ui->tableViewPart->setColumnHidden(0,true);
    ui->tableViewPart->setColumnHidden(3,true);
    ui->tableViewPart->setColumnHidden(4,true);
    ui->tableViewPart->setColumnHidden(5,true);
    for (int i=8; i<ui->tableViewPart->model()->columnCount(); i++){
        ui->tableViewPart->setColumnHidden(i,true);
    }
    ui->tableViewPart->setColumnWidth(1,50);
    ui->tableViewPart->setColumnWidth(2,70);
    ui->tableViewPart->setColumnWidth(6,100);
    ui->tableViewPart->setColumnWidth(7,50);

    mapper = new DbMapper(ui->tableViewPart,this);
    ui->horizontalLayoutMap->insertWidget(0,mapper);
    mapper->addMapping(ui->lineEditPart,1);
    mapper->addMapping(ui->dateEditPart,2);
    mapper->addMapping(ui->comboBoxSrc,3);
    mapper->addMapping(ui->lineEditKvo,4);
    mapper->addMapping(ui->comboBoxRcp,5);
    mapper->addMapping(ui->comboBoxMark,6);
    mapper->addMapping(ui->lineEditDiam,7);
    mapper->addMapping(ui->comboBoxProv,8);
    mapper->addMapping(ui->plainTextEdit,9);
    mapper->addMapping(ui->lineEditPlot,10);
    mapper->addMapping(ui->lineEditVyaz,11);
    mapper->addMapping(ui->lineEditObm,12);
    mapper->addMapping(ui->lineEditOst,13);
    mapper->addMapping(ui->lineEditKfmp,14);
    mapper->addMapping(ui->lineEditDl,15);
    mapper->addMapping(ui->lineEditPokr,16);
    mapper->addMapping(ui->lineEditFil,17);
    mapper->addMapping(ui->lineEditDel,18);
    mapper->addMapping(ui->lineEditPartProv,19);
    mapper->addMapping(ui->lineEditMasDry,20);
    mapper->addMapping(ui->lineEditMasGl,21);
    mapper->addMapping(ui->comboBoxPack,22);
    mapper->addMapping(ui->comboBoxLong,23);
    mapper->addMapping(ui->comboBoxVar,24);

    mapper->setDefaultFocus(1);
    mapper->addLock(ui->dateEditBeg);
    mapper->addLock(ui->dateEditEnd);
    mapper->addLock(ui->pushButtonUpd);
    mapper->addLock(ui->comboBoxOnly);
    mapper->addEmptyLock(ui->tableViewGlass);
    mapper->addEmptyLock(ui->tableViewGlassCont);
    mapper->addEmptyLock(ui->tableViewGlassPar);
    mapper->addEmptyLock(ui->tableViewDoz);
    mapper->addEmptyLock(ui->tableViewPress);
    mapper->addEmptyLock(ui->tableViewChem);
    mapper->addEmptyLock(ui->tableViewMech);
    mapper->addEmptyLock(ui->toolButtonCopyPar);
    mapper->addEmptyLock(ui->toolButtonChem);
    mapper->addEmptyLock(ui->toolButtonSamp);

    ui->comboBoxOnly->setModel(Rels::instance()->relMark->model());
    ui->comboBoxOnly->setModelColumn(1);
    ui->comboBoxOnly->setCurrentIndex(-1);

    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshCont(int)));

    connect(ui->tableViewGlass->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(refreshGlassData(QModelIndex)));

    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(updPart()));
    connect(ui->comboBoxOnly,SIGNAL(currentIndexChanged(int)),this,SLOT(updPart()));
    connect(ui->comboBoxOnly->lineEdit(),SIGNAL(textChanged(QString)),this,SLOT(updPart()));
    connect(ui->toolButtonChem,SIGNAL(clicked(bool)),this,SLOT(loadChem()));
    connect(modelChem,SIGNAL(sigUpd()),modelPart,SLOT(refreshState()));
    connect(modelChem,SIGNAL(sigUpd()),this,SLOT(lockChemSampCh()));
    connect(modelMech,SIGNAL(sigUpd()),modelPart,SLOT(refreshState()));
    connect(ui->comboBoxRcp,SIGNAL(currentIndexChanged(int)),this,SLOT(insertMark(int)));
    connect(ui->comboBoxMark,SIGNAL(currentIndexChanged(int)),this,SLOT(insertProvol(int)));
    connect(ui->toolButtonCopyPar,SIGNAL(clicked(bool)),this,SLOT(copyPar()));
    connect(ui->toolButtonSamp,SIGNAL(clicked(bool)),this,SLOT(insertChemSamp()));
    connect(mapper,SIGNAL(lockChanged(bool)),this,SLOT(lockChangedMap(bool)));
    connect(ui->comboBoxChemDev,SIGNAL(currentIndexChanged(int)),this,SLOT(setCurrentChemDev(int)));
    connect(ui->lineEditDiam,SIGNAL(editingFinished()),this,SLOT(setDefaultPack()));

    updPart();
}

MainWidget::~MainWidget()
{
    saveSettings();
    delete ui;
}

int MainWidget::currentIdPart()
{
    QModelIndex indp=ui->tableViewPart->model()->index(mapper->currentIndex(),0);
    QVariant val=ui->tableViewPart->model()->data(indp,Qt::EditRole);
    return val.isNull()? 0 : val.toInt();
}

void MainWidget::loadSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    this->restoreGeometry(settings.value("main_geometry").toByteArray());
    this->ui->splitter->restoreState(settings.value("main_splitter_width").toByteArray());
}

void MainWidget::saveSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("main_geometry", this->saveGeometry());
    settings.setValue("main_splitter_width",ui->splitter->saveState());
}

void MainWidget::updPart()
{
    int ind=ui->comboBoxOnly->currentIndex();
    int id_el=-1;
    if (/*ind>=0*/!ui->comboBoxOnly->currentText().isEmpty()){
        id_el=ui->comboBoxOnly->model()->data(ui->comboBoxOnly->model()->index(ind,0),Qt::EditRole).toInt();
    }
    if (sender()==ui->pushButtonUpd){
        modelMix->refreshRel(ui->dateEditBeg->date().addYears(-1),ui->dateEditEnd->date().addYears(1));
        Rels::instance()->refresh();
    }
    modelPart->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),id_el);
}

void MainWidget::refreshCont(int ind)
{
    QModelIndex ip=ui->tableViewPart->model()->index(ind,0);
    int id_part=ui->tableViewPart->model()->data(ip,Qt::EditRole).toInt();
    modelChem->refresh(id_part);
    modelMech->refresh(id_part);
    modelRab->refresh(id_part);
    modelMix->refresh(id_part);
    modelGlass->refresh(id_part);
    ui->tableViewGlass->setCurrentIndex(ui->tableViewGlass->model()->index(0,1));
    lockChemSampCh();
}

void MainWidget::loadChem()
{
    DialogLoadChem d;
    if (d.exec()==QDialog::Accepted){
        QList <int> l = modelChem->ids();
        foreach (int key,l){
            QString chem=Rels::instance()->relChem->data(QString::number(key)).toString();
            double val=d.chemVal(chem);
            if (val>0){
                modelChem->addChem(key,val,2);
            }
        }
    }
    modelChem->select();
    modelPart->refreshState();
}

void MainWidget::refreshGlassData(QModelIndex index)
{
    QVariant id_l=ui->tableViewGlass->model()->data(ui->tableViewGlass->model()->index(index.row(),3),Qt::EditRole);
    int id_load = id_l.isNull()? -1 : id_l.toInt();
    modelConsStatData->refresh(id_load);
    modelConsStatPar->refresh(id_load,currentIdPart());
    ui->tableViewGlassCont->resizeColumnsToContents();
    ui->tableViewGlassPar->resizeColumnsToContents();
    ui->toolButtonCopyPar->setEnabled(modelConsStatPar->rowCount()>0);
}

void MainWidget::insertMark(int rcpIndex)
{
    if (modelPart->isAdd()){
        QModelIndex ind=ui->comboBoxRcp->model()->index(rcpIndex,3);
        int id_el=ui->comboBoxRcp->model()->data(ind,Qt::EditRole).toInt();
        QModelIndex elIndex=modelPart->index(mapper->currentIndex(),6);
        modelPart->setData(elIndex,id_el,Qt::EditRole);
    }
}

void MainWidget::insertProvol(int markIndex)
{
    if (modelPart->isAdd()){
        if (!ui->comboBoxMark->currentText().isEmpty()){
            QModelIndex ind=ui->comboBoxMark->model()->index(markIndex,2);
            int id_pr=ui->comboBoxMark->model()->data(ind,Qt::EditRole).toInt();
            QModelIndex prIndex=modelPart->index(mapper->currentIndex(),8);
            modelPart->setData(prIndex,id_pr,Qt::EditRole);
        }
    }
}

void MainWidget::copyPar()
{
    if (modelConsStatPar->rowCount()){
        int n=QMessageBox::question(this,QString::fromUtf8("Подтвердите действия"),QString::fromUtf8("Скопировать значения?"),QMessageBox::Yes,QMessageBox::No);
        if (n==QMessageBox::Yes){
            for (int i=0; i<modelConsStatPar->rowCount(); i++){
                int id_par=modelConsStatPar->data(modelConsStatPar->index(i,0),Qt::EditRole).toInt();
                double val=modelConsStatPar->data(modelConsStatPar->index(i,2),Qt::EditRole).toDouble();
                if (id_par==2){
                    QModelIndex plIndex=modelPart->index(mapper->currentIndex(),10);
                    modelPart->setData(plIndex,val,Qt::EditRole);
                } else if (id_par==1){
                    QModelIndex vIndex=modelPart->index(mapper->currentIndex(),11);
                    modelPart->setData(vIndex,val,Qt::EditRole);
                }
            }
            modelPart->submit();
        }
    }
}

void MainWidget::insertChemSamp()
{
    if (modelChem->isEmpty()){
        int id_dev=ui->comboBoxChemDev->model()->data(ui->comboBoxChemDev->model()->index(ui->comboBoxChemDev->currentIndex(),0),Qt::EditRole).toInt();
        QList <int> l = modelChem->ids();
        foreach (int key,l){
            modelChem->addChem(key,0.0,id_dev);
        }
        modelChem->select();
        lockChemSampCh();
        modelPart->refreshState();
    }
}

void MainWidget::lockChemSampCh()
{
    ui->toolButtonSamp->setEnabled(modelChem->isEmpty());
}

void MainWidget::lockChangedMap(bool lock)
{
    if (!lock){
        lockChemSampCh();
        ui->toolButtonCopyPar->setEnabled(modelConsStatPar->rowCount()>0);
    }
}

void MainWidget::setCurrentChemDev(int index)
{
    int id_dev=ui->comboBoxChemDev->model()->data(ui->comboBoxChemDev->model()->index(index,0),Qt::EditRole).toInt();
    modelChem->setDefaultValue(4,id_dev);
    modelChem->select();
}

void MainWidget::setDefaultPack()
{
    QSqlQuery query;
    query.prepare("select id_pack_el_default, id_long_el_default from diam where diam=:d");
    query.bindValue(":d",ui->lineEditDiam->text().toDouble());
    if (query.exec()){
        while (query.next()){
            modelPart->setData(modelPart->index(mapper->currentIndex(),22),query.value(0).toInt(),Qt::EditRole);
            modelPart->setData(modelPart->index(mapper->currentIndex(),23),query.value(1).toInt(),Qt::EditRole);
        }
    } else {
        QMessageBox::critical(this,tr("Error"),query.lastError().text(),QMessageBox::Cancel);
    }
}
