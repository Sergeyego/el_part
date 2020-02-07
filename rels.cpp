#include "rels.h"

Rels* Rels::rels_instance=0;

Rels *Rels::instance()
{
    if (rels_instance==0)
        rels_instance = new Rels();
    return rels_instance;
}


Rels::Rels(QObject *parent) : QObject(parent)
{
    relGlass = new DbRelation(QString("select id, nam from matr where id_type=4 order by nam"),0,1,this);
    relPar = new DbRelation(QString("select id, nam from glass_par order by nam"),0,1,this);
    relSrc= new DbRelation(QString("select id, nam from istoch order by nam"),0,1,this);
    relRcp = new DbRelation(QString("select id, nam, lev from rcp_nam order by nam"),0,1,this);
    relMark = new DbRelation(QString("select id, marka from elrtr order by marka"),0,1,this);
    relProv = new DbRelation(QString("select id, nam from provol order by nam"),0,1,this);

    relRcp->proxyModel()->setFilterKeyColumn(2);
    relRcp->proxyModel()->setFilterFixedString("1");
}

void Rels::refresh()
{
    relGlass->refreshModel();
    relPar->refreshModel();
    relSrc->refreshModel();
    relRcp->refreshModel();
    relMark->refreshModel();
    relProv->refreshModel();

    emit sigRefresh();
}
