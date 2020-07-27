#include "dialogloadchem.h"
#include "ui_dialogloadchem.h"

DialogLoadChem::DialogLoadChem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLoadChem)
{
    ui->setupUi(this);

    QFileSystemModel *dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    dirModel->setRootPath(QDir::rootPath());

    QFileSystemModel *fileModel = new QFileSystemModel(this);
    fileModel->setRootPath(QDir::rootPath());

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(dirModel);

    ui->treeView->setModel(proxyModel);
    ui->listView->setModel(fileModel);

    connect(ui->treeView,SIGNAL(clicked(QModelIndex)),this,SLOT(setCurrentDir(QModelIndex)));


}

DialogLoadChem::~DialogLoadChem()
{
    delete ui;
}

void DialogLoadChem::setCurrentDir(QModelIndex index)
{
}
