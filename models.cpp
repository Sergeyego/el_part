#include "models.h"

ModelPart::ModelPart(QObject *parent) : DbTableModel("parti",parent)
{
    addColumn("id",QString::fromUtf8("id"));
    addColumn("n_s",QString::fromUtf8("Парт."));
    addColumn("dat_part",QString::fromUtf8("Дата"));
    addColumn("id_ist",QString::fromUtf8("Источник"),NULL,Rels::instance()->relSrc);
    addColumn("ob_em",QString::fromUtf8("Объем"));
    addColumn("id_rcp",QString::fromUtf8("Рецептура"),NULL,Rels::instance()->relRcp);
    addColumn("id_el",QString::fromUtf8("Марка"),NULL,Rels::instance()->relMark);
    addColumn("diam",QString::fromUtf8("Диам."),new QDoubleValidator(0,10,1,this));
    addColumn("id_prfact",QString::fromUtf8("Проволока"),NULL,Rels::instance()->relProv);
    addColumn("prim",QString::fromUtf8("Примечание"));
    addColumn("dens",QString::fromUtf8("Плотность"),new QDoubleValidator(0,100,3,this));
    addColumn("visc",QString::fromUtf8("Вязкозть"),new QDoubleValidator(0,10000,1,this));
    addColumn("liq_glass",QString::fromUtf8("% сод.в обм."),new QDoubleValidator(0,100,1,this));
    addColumn("dry_rst",QString::fromUtf8("% сух. ост."),new QDoubleValidator(0,100,1,this));
    addColumn("kfmp",QString::fromUtf8("кфмп"),new QDoubleValidator(0,1000,2,this));
    addColumn("le",QString::fromUtf8("Дл. эл-да"),new QDoubleValidator(0,1000,1,this));
    addColumn("lo",QString::fromUtf8("Дл. покр."),new QDoubleValidator(0,1000,1,this));
    addColumn("dfil",QString::fromUtf8("ф филь."),new QDoubleValidator(0,100,2,this));
    addColumn("del",QString::fromUtf8("ф эл."),new QDoubleValidator(0,100,2,this));

    setSort("parti.dat_part, parti.n_s");
}

void ModelPart::refresh(const QDate &beg, const QDate &end, int id_el)
{
    QString f="parti.dat_part between '"+beg.toString("yyyy-MM-dd")+"' and '"+end.toString("yyyy-MM-dd")+"'";
    if (id_el>=0){
        f.append(" and parti.id_el = "+QString::number(id_el));
    }
    setFilter(f);
    select();
}
