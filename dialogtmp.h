#ifndef DIALOGTMP_H
#define DIALOGTMP_H

#include <QDialog>
#include "models.h"

namespace Ui {
class DialogTmp;
}

class DialogTmp : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTmp(QWidget *parent = 0);
    ~DialogTmp();
    void load(int id_part, int id_dev, QList<int> ids);

private:
    Ui::DialogTmp *ui;
    ModelChemSrc *modelChem;
};

#endif // DIALOGTMP_H
