#ifndef MODELS_H
#define MODELS_H

#include <QObject>
#include "db/dbtablemodel.h"
#include "rels.h"

class ModelPart : public DbTableModel
{
    Q_OBJECT
public:
    ModelPart(QObject *parent);
    void refresh(const QDate &beg, const QDate &end, int id_el);
};

struct range {
    QVariant min;
    QVariant max;
};

class ModelChem : public DbTableModel
{
public:
    ModelChem(QString tname, QObject *parent=0);
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    ~ModelChem();
    void refresh(int id_part);
private:
    QMap <int,range> map;
    QColor getColor(const QModelIndex &index) const;
    QString getToolTip(const QModelIndex &index) const;

protected:
    QString tablename;
    int colVal;
    int colIdChem;
    int colIdPart;
    QString flt;
    QString tuQuery;
};

class ModelChemSrc : public ModelChem
{
public:
    ModelChemSrc(QObject *parent=0);
};

class ModelMechSrc : public ModelChem
{
public:
    ModelMechSrc(QObject *parent=0);
};

#endif // MODELS_H
