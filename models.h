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
    QColor getColor(const QModelIndex &index) const;
    QString getToolTip(const QModelIndex &index) const;

protected:
    QMap <int,range> map;
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
    bool addChem(int id_chem, double val);
    QList <int> ids();
};

class ModelMechSrc : public ModelChem
{
public:
    ModelMechSrc(QObject *parent=0);
};

class ModelRab : public DbTableModel
{
public:
    ModelRab(QObject *parent=0);
    void refresh(int id_part);
private:
    DbRelation *relRab;
};

class ModelMix : public DbTableModel
{
public:
    ModelMix(QObject *parent=0);
    void refresh(int id_part);
    void refreshRel(QDate beg, QDate end);
private:
    DbRelation *relMix;
    QString relQuery(QDate beg, QDate end);
};

class ModelGlass : public DbTableModel
{
public:
    ModelGlass(QObject *parent=0);
    void refresh(int id_part);
private:
    DbRelation *relConsLoad;
    QString relQuery(int id_part);
};
#endif // MODELS_H
