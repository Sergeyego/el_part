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

#endif // MODELS_H
