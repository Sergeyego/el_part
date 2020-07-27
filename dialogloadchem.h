#ifndef DIALOGLOADCHEM_H
#define DIALOGLOADCHEM_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include <QFileSystemModel>

namespace Ui {
class DialogLoadChem;
}

class DialogLoadChem : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLoadChem(QWidget *parent = 0);
    ~DialogLoadChem();

private:
    Ui::DialogLoadChem *ui;

private slots:
    void setCurrentDir(QModelIndex index);
};

#endif // DIALOGLOADCHEM_H
