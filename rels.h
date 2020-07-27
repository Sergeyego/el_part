#ifndef RELS_H
#define RELS_H

#include <QObject>
#include "db/dbtablemodel.h"

class Rels : public QObject
{
    Q_OBJECT
public:
    static Rels *instance();
    DbRelation *relGlass;
    DbRelation *relPar;
    DbRelation *relSrc;
    DbRelation *relRcp;
    DbRelation *relMark;
    DbRelation *relProv;
    DbRelation *relChem;
    DbRelation *relMech;

protected:
    explicit Rels(QObject *parent = 0);

private:
    static Rels *rels_instance;

signals:
    void sigRefresh();

public slots:
    void refresh();

};

#endif // RELS_H
