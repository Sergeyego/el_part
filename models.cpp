#include "models.h"

ModelPart::ModelPart(QObject *parent) : DbTableModel("parti",parent)
{
    addColumn("id",QString::fromUtf8("id"));
    addColumn("n_s",QString::fromUtf8("Парт."));
    addColumn("dat_part",QString::fromUtf8("Дата"));
    addColumn("id_ist",QString::fromUtf8("Источник"),Rels::instance()->relSrc);
    addColumn("ob_em",QString::fromUtf8("Объем"));
    addColumn("id_rcp",QString::fromUtf8("Рецептура"),Rels::instance()->relRcp);
    addColumn("id_el",QString::fromUtf8("Марка"),Rels::instance()->relMark);
    addColumn("diam",QString::fromUtf8("Диам."));
    addColumn("id_prfact",QString::fromUtf8("Проволока"),Rels::instance()->relProv);
    addColumn("prim",QString::fromUtf8("Примечание"));
    addColumn("dens",QString::fromUtf8("Плотность"));
    addColumn("visc",QString::fromUtf8("Вязкозть"));
    addColumn("liq_glass",QString::fromUtf8("% сод.в обм."));
    addColumn("dry_rst",QString::fromUtf8("% сух. ост."));
    addColumn("kfmp",QString::fromUtf8("кфмп"));
    addColumn("le",QString::fromUtf8("Дл. эл-да"));
    addColumn("lo",QString::fromUtf8("Дл. покр."));
    addColumn("dfil",QString::fromUtf8("ф филь."));
    addColumn("del",QString::fromUtf8("ф эл."));
    addColumn("parti_prov",QString::fromUtf8("Партия пров."));
    addColumn("mas_dry",QString::fromUtf8("Масса шихты"));
    addColumn("mas_lqgl",QString::fromUtf8("Масса ж.стекла"));

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

ModelChem::ModelChem(QString tname, QObject *parent) : DbTableModel(tname,parent), tablename(tname)
{
    colIdPart=0;
    colIdChem=1;
    colVal=2;
}

QVariant ModelChem::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role==Qt::BackgroundColorRole){
        return getColor(index);
    }
    if (role==Qt::ToolTipRole){
        return getToolTip(index);
    }
    return DbTableModel::data(index,role);
}

ModelChem::~ModelChem()
{

}

void ModelChem::refresh(int id_part)
{
    map.clear();
    QSqlQuery query;
    query.prepare(tuQuery);
    query.bindValue(":id",id_part);
    if (query.exec()){
        while (query.next()){
            range r;
            r.min=query.value(1);
            r.max=query.value(2);
            map.insert(query.value(0).toInt(),r);
        }
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
    setFilter(flt+" = "+QString::number(id_part));
    setDefaultValue(colIdPart,id_part);
    select();
}

QColor ModelChem::getColor(const QModelIndex &index) const
{
    if (!index.isValid()){
        return QColor();
    }
    int id_chem=data(this->index(index.row(),colIdChem),Qt::EditRole).toInt();
    double val=data(this->index(index.row(),colVal),Qt::EditRole).toDouble();
    range r=map.value(id_chem);
    QColor color=Qt::white;
    if (!r.min.isNull() || !r.max.isNull()){
        if (!r.max.isNull()){
            if ((val>r.min.toDouble()) && (val<r.max.toDouble())){
                color=QColor(170,255,170);
            } else if ((val<r.min.toDouble()) || (val>r.max.toDouble())){
                color=QColor(255,170,170);
            } else {
                color=Qt::yellow;
            }
        } else {
            if (val<r.min.toDouble()){
                color=QColor(255,170,170);
            } else if (val>r.min.toDouble()){
                color=QColor(170,255,170);
            } else {
                color=Qt::yellow;
            }
        }
    }
    return color;
}

QString ModelChem::getToolTip(const QModelIndex &index) const
{
    if (!index.isValid()){
        return QString();
    }
    int id_chem=data(this->index(index.row(),colIdChem),Qt::EditRole).toInt();
    QString el=data(this->index(index.row(),colIdChem),Qt::DisplayRole).toString();
    range r=map.value(id_chem);
    QString s;
    if (!r.min.isNull() || !r.max.isNull()){
        s+=el;
        if (r.max.isNull() && !r.min.isNull()){
            s+=QString(": min %1").arg(r.min.toDouble());
        } else if (!r.max.isNull() && r.min.isNull()){
            s+=QString(": max %1").arg(r.max.toDouble());
        } else {
            s+=QString(": min %1 max %2").arg(r.min.toDouble()).arg(r.max.toDouble());
        }
    }
    return s;
}

ModelChemSrc::ModelChemSrc(QObject *parent) : ModelChem("parti_chem",parent)
{
    addColumn("id","id");
    addColumn("id_part","id_part");
    addColumn("id_chem",tr("Элем."),Rels::instance()->relChem);
    addColumn("kvo",tr("Сод., %"));
    addColumn("dt_cre",tr("Дата"));
    setValidator(3,new QDoubleValidator(0,100,3,this));
    setSort(tablename+".id_chem, "+tablename+".dt_cre");
    flt=tableName+".id_part";
    setColumnFlags(4,Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    colIdPart=1;
    colIdChem=2;
    colVal=3;
    tuQuery="select c.id_chem, c.min, c.max from chem_tu as c where c.id_el = (select p.id_el from parti as p where p.id = :id )";
}

bool ModelChemSrc::addChem(int id_chem, double val)
{
    int id_part=defaultValue(colIdPart).toInt();
    QSqlQuery query;
    query.prepare("insert into parti_chem (id_part, id_chem, kvo) values (:id_part, :id_chem, :kvo)");
    query.bindValue(":id_part",id_part);
    query.bindValue(":id_chem",id_chem);
    query.bindValue(":kvo",val);
    bool ok=query.exec();
    if (!ok){
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
    return ok;
}

QList <int> ModelChemSrc::ids()
{
    return map.keys();
}

ModelMechSrc::ModelMechSrc(QObject *parent) : ModelChem("parti_mech",parent)
{
    addColumn("id_part","id_part");
    addColumn("id_mech",tr("Параметр"),Rels::instance()->relMech);
    addColumn("kvo",tr("Значение"));
    setSort("parti_mech.id_mech");
    setValidator(2,new QDoubleValidator(-1000000000,1000000000,2,this));
    flt=tableName+".id_part";
    colIdPart=0;
    colIdChem=1;
    colVal=2;
    tuQuery="select m.id_mech, m.min, m.max from mech_tu as m where m.id_el = (select p.id_el from parti as p where p.id = :id )";
}
