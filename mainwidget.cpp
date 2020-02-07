#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());
    ui->pushButtonUpd->setIcon(this->style()->standardIcon(QStyle::SP_BrowserReload));

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

    mapper->setDefaultFocus(1);
    mapper->addLock(ui->dateEditBeg);
    mapper->addLock(ui->dateEditEnd);
    mapper->addLock(ui->pushButtonUpd);
    mapper->addLock(ui->comboBoxOnly);
    mapper->addEmptyLock(ui->tableViewGlass);
    mapper->addEmptyLock(ui->tableViewGlassCont);
    mapper->addEmptyLock(ui->tableViewGlassPar);
    mapper->addEmptyLock(ui->tableViewDoz);
    mapper->addEmptyLock(ui->tableViewBrak);
    mapper->addEmptyLock(ui->tableViewPress);
    mapper->addEmptyLock(ui->tableViewChem);
    mapper->addEmptyLock(ui->tableViewMech);
    mapper->addEmptyLock(ui->toolButtonCopyPar);

    ui->comboBoxOnly->setModel(Rels::instance()->relMark->model());
    ui->comboBoxOnly->setModelColumn(1);
    ui->comboBoxOnly->setCurrentIndex(-1);

    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(updPart()));
    connect(ui->comboBoxOnly,SIGNAL(currentIndexChanged(QString)),this,SLOT(updPart()));
    connect(ui->comboBoxOnly,SIGNAL(currentTextChanged(QString)),this,SLOT(updPartFlf(QString)));
    updPart();
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::updPart()
{
    int ind=ui->comboBoxOnly->currentIndex();
    int id_el=-1;
    if (ind>=0){
        id_el=ui->comboBoxOnly->model()->data(ui->comboBoxOnly->model()->index(ind,0),Qt::EditRole).toInt();
    }
    modelPart->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),id_el);
}

void MainWidget::updPartFlf(QString s)
{
    if (s.isEmpty()){
        ui->comboBoxOnly->setCurrentIndex(-1);
    }
}
