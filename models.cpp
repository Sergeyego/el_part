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
    addColumn("id_pack",QString::fromUtf8("Упаковка"),Rels::instance()->relPack);
    addColumn("id_long",QString::fromUtf8("Длина"),Rels::instance()->relLong);
    addColumn("id_var",QString::fromUtf8("Вариант"),Rels::instance()->relVar);

    setDecimals(10,3);
    setDecimals(12,2);
    setDecimals(13,2);
    setDecimals(14,2);
    setDecimals(17,2);
    setDecimals(18,2);
    setDefaultValue(24,1);

    setSort("parti.n_s, parti.dat_part");
}

QVariant ModelPart::data(const QModelIndex &index, int role) const
{
    if((role == Qt::BackgroundColorRole)) {
        int area = colorState.value(data(this->index(index.row(),0),Qt::EditRole).toInt());
        if(area == 1) return QVariant(QColor(255,170,170)); else
            if(area == 2) return QVariant(QColor(170,255,170)); else
                if(area == 3) return QVariant(QColor(Qt::yellow)); else
                    return DbTableModel::data(index,role);
    } else return DbTableModel::data(index,role);
}

void ModelPart::refresh(const QDate &beg, const QDate &end, int id_el)
{
    dBeg=beg;
    dEnd=end;
    QString f="parti.dat_part between '"+beg.toString("yyyy-MM-dd")+"' and '"+end.toString("yyyy-MM-dd")+"'";
    if (id_el>=0){
        f.append(" and parti.id_el = "+QString::number(id_el));
    }
    setFilter(f);
    refreshState();
    select();
}

bool ModelPart::insertRow(int row, const QModelIndex &parent)
{
    int old_num=0;
    if (rowCount()>0) old_num=this->data(this->index(rowCount()-1,1),Qt::EditRole).toInt();
    setDefaultValue(1,QString("%1").arg((old_num+1),4,'d',0,QChar('0')));
    setDefaultValue(2,QDate::currentDate());
    return DbTableModel::insertRow(row,parent);
}

void ModelPart::refreshState()
{
    QSqlQuery query;
    query.prepare("select parti.id, "
                  "(select case when exists(select id from parti_chem where id_part=parti.id) "
                  "then 1 else 0 end "
                  "+ "
                  "case when exists(select id from parti_mech where id_part=parti.id) "
                  "then 2 else 0 end "
                  "as r) from parti where parti.dat_part between :d1 and :d2");
    query.bindValue(":d1",dBeg);
    query.bindValue(":d2",dEnd);
    if (query.exec()){
        colorState.clear();
        while (query.next()){
            colorState[query.value(0).toInt()]=query.value(1).toInt();
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
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
    QString qu=tuQuery;
    qu=qu.replace(":id",QString::number(id_part));
    query.prepare(qu);
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
    addColumn("id_chem",QString::fromUtf8("Элем."),Rels::instance()->relChem);
    addColumn("kvo",QString::fromUtf8("Сод., %"));
    addColumn("id_dev",QString::fromUtf8("Прибор"),Rels::instance()->relChemDev);
    addColumn("dt_cre",QString::fromUtf8("Дата"));
    setValidator(3,new QDoubleValidator(0,100,3,this));
    setSort(tablename+".id_chem, "+tablename+".dt_cre");
    flt=tableName+".id_part";
    setColumnFlags(5,Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    colIdPart=1;
    colIdChem=2;
    colVal=3;
    tuQuery="select c.id_chem, c.min, c.max from chem_tu as c where "
            "c.id_el = (select p.id_el from parti as p where p.id = :id ) "
            "and c.id_var = (select p.id_var from parti as p where p.id = :id )";
}

bool ModelChemSrc::addChem(int id_chem, double val, int id_dev)
{
    int id_part=defaultValue(colIdPart).toInt();
    QSqlQuery query;
    query.prepare("insert into parti_chem (id_part, id_chem, kvo, id_dev) values (:id_part, :id_chem, :kvo, :id_dev)");
    query.bindValue(":id_part",id_part);
    query.bindValue(":id_chem",id_chem);
    query.bindValue(":kvo",val);
    query.bindValue(":id_dev",id_dev);
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
    addColumn("id_mech",QString::fromUtf8("Параметр"),Rels::instance()->relMech);
    addColumn("kvo",QString::fromUtf8("Значение"));
    setSort("parti_mech.id_mech");
    setDecimals(2,2);
    flt=tableName+".id_part";
    colIdPart=0;
    colIdChem=1;
    colVal=2;
    tuQuery="select m.id_mech, m.min, m.max from mech_tu as m where "
            "m.id_el = (select p.id_el from parti as p where p.id = :id )"
            "and m.id_var = (select p.id_var from parti as p where p.id = :id )";
}

ModelRab::ModelRab(QObject *parent) : DbTableModel("part_prod",parent)
{

    QString qu=QString("select r.id, r.snam "
            "from rab_rab r inner join rab_qual q on q.id_rab=r.id "
            "inner join rab_prof p on p.id=q.id_prof "
            "inner join rab_razr z on z.id=q.id_razr "
            "where q.dat=(select max(dat) from rab_qual where dat<='%1' and id_rab=r.id and id_prof=1 and id_razr in (2,3)) "
            "order by r.snam").arg(QDate::currentDate().toString("yyyy-MM-dd"));
    relRab = new DbRelation(qu,0,1,this);

    addColumn("id","id");
    addColumn("id_part","id_part");
    addColumn("dat",QString::fromUtf8("Дата"));
    addColumn("id_press",QString::fromUtf8("Пресс"),Rels::instance()->relPress);
    addColumn("id_brig",QString::fromUtf8("Бригадир"),relRab);
    addColumn("kvo",QString::fromUtf8("Кол-во,кг"));
    addColumn("davl",QString::fromUtf8("Давлен."));
    addColumn("loss",QString::fromUtf8("Отх.ших."));
    addColumn("rods",QString::fromUtf8("Пров.,кг"));
    addColumn("garb",QString::fromUtf8("Отх.мус."));

    setSort("part_prod.dat");
}

bool ModelRab::insertRow(int row, const QModelIndex &parent)
{
    setDefaultValue(2,QDate::currentDate());
    return DbTableModel::insertRow(row,parent);
}

void ModelRab::refresh(int id_part)
{
    DbRelationalModel *relModel=qobject_cast<DbRelationalModel *>(relRab->model());
    if (relModel){
        QString qu=QString("select r.id, r.snam "
                "from rab_rab r inner join rab_qual q on q.id_rab=r.id "
                "inner join rab_prof p on p.id=q.id_prof "
                "inner join rab_razr z on z.id=q.id_razr "
                "where q.dat=(select max(dat) from rab_qual where dat<=(select dat_part from parti where id = %1 ) and id_rab=r.id and id_prof=1 and id_razr in (2,3)) "
                "order by r.snam").arg(id_part);
        relModel->setQuery(qu);
    }

    setFilter("part_prod.id_part = "+QString::number(id_part));
    setDefaultValue(1,id_part);
    select();
}

ModelMix::ModelMix(QObject *parent) : DbTableModel("parti_mix",parent)
{
    relMix = new DbRelation(relQuery(QDate(QDate::currentDate().year()-1,1,1),QDate(QDate::currentDate().year(),12,31)),0,1,this);
    addColumn("id_part","id_part");
    addColumn("id_dos",QString::fromUtf8("Партия дозировки"),relMix);
    addColumn("kvo",QString::fromUtf8("Кол-во,кг"));
    setSort("parti_mix.id_dos");
}

void ModelMix::refresh(int id_part)
{
    setFilter("parti_mix.id_part = "+QString::number(id_part));
    setDefaultValue(0,id_part);
    select();
}

void ModelMix::refreshRel(QDate beg, QDate end)
{
    DbRelationalModel *relModel=qobject_cast<DbRelationalModel *>(relMix->model());
    if (relModel){
        relModel->setQuery(relQuery(beg,end));
    }
}

QString ModelMix::relQuery(QDate beg, QDate end)
{
    return QString("select d.id, cast(d.parti || '  ' || n.nam || '  ' || d.dat || ' ' || d.tm as varchar(64)) as nam "
                       "from dosage d inner join rcp_nam n on n.id=d.id_rcp "
                       "where d.dat between '%1' and '%2' "
                       "order by d.parti, d.dat desc, d.tm desc").arg(beg.toString("yyyy-MM-dd")).arg(end.toString("yyyy-MM-dd"));
}

ModelGlass::ModelGlass(QObject *parent) : DbTableModel("acc_glyba",parent)
{
    relConsLoad = new DbRelation(relQuery(-1),0,1,this);
    addColumn("id","id");
    addColumn("id_part","id_part");
    addColumn("id_glyb_part",QString::fromUtf8("Стекло"),Rels::instance()->relGlass);
    addColumn("id_glass_cons_load",QString::fromUtf8("Расходник"),relConsLoad);

    setSort("acc_glyba.id");
}

void ModelGlass::refresh(int id_part)
{
    DbRelationalModel *relModel=qobject_cast<DbRelationalModel *>(relConsLoad->model());
    if (relModel){
        relModel->setQuery(relQuery(id_part));
    }
    setFilter("acc_glyba.id_part = "+QString::number(id_part));
    setDefaultValue(1,id_part);
    select();
}

QString ModelGlass::relQuery(int id_part)
{
    return QString::fromUtf8("select st.id, ('№'||c.num||'; загр. '||to_char(l.dat_load,'dd.MM.yy')||'; '||COALESCE(mk.nam, ms.nam)||'; парт.'|| l.parti_glass) as info "
                   "from ( "
                   "select l.id from glass_cons_load as l "
                   "where l.dat_load=(select max(ll.dat_load) from glass_cons_load as ll where ll.dat_load<= (select dat_part-1 from parti where id = %1 ) and ll.id_cons=l.id_cons) "
                   "union "
                   "select l.id from glass_cons_load as l "
                   "where l.dat_load=(select max(ll.dat_load) from glass_cons_load as ll where ll.dat_load<= (select dat_part from parti where id = %2 ) and ll.id_cons=l.id_cons) "
                   "union "
                   "select l.id from glass_cons_load as l "
                   "where l.dat_load=(select max(ll.dat_load) from glass_cons_load as ll where ll.dat_load<= (select dat_part+1 from parti where id = %3 ) and ll.id_cons=l.id_cons) "
                   ") as st "
                   "inner join glass_cons_load as l on l.id=st.id "
                   "inner join glass_cons as c on c.id=l.id_cons "
                   "left join glass_korr_load as gl on gl.id=l.id_korr_load "
                   "left join glass_korr as k on k.id=gl.id_korr "
                   "left join matr as mk on mk.id=gl.id_matr "
                   "left join glass_sump_load as sl on sl.id=l.id_sump_load "
                   "left join matr as ms on ms.id=sl.id_matr "
                   "order by c.num, l.dat_load").arg(id_part).arg(id_part).arg(id_part);
}

ModelConsStatData::ModelConsStatData(QObject *parent) :ModelRo(parent)
{
    dec=1;
    connect(this,SIGNAL(newQuery()),this,SLOT(refreshInPar()));
}

void ModelConsStatData::refresh(int id_load)
{
    current_id_load=id_load;
    QSqlQuery query;
    query.prepare("(select d.id_load, d.proc, m.nam, l.part_lump, l.modul, NULL "
                  "from glass_korr_load_data as d "
                  "inner join glass_sump_load as l on l.id=d.id_sump_load "
                  "inner join matr as m on m.id=l.id_matr "
                  "inner join glass_sump as s on s.id=l.id_sump "
                  "where d.id_load = (select id_korr_load from glass_cons_load where id = :id_load1 ) order by proc) "
                  "union "
                  "(select NULL, 100.0, mm.nam, ll.part_lump, ll.modul, ll.id "
                  "from glass_sump_load as ll "
                  "inner join matr as mm on mm.id=ll.id_matr "
                  "inner join glass_sump as ss on ss.id=ll.id_sump "
                  "where ll.id = (select id_sump_load from glass_cons_load where id = :id_load2 ))");
    query.bindValue(":id_load1",id_load);
    query.bindValue(":id_load2",id_load);
    if (execQuery(query)){
        setHeaderData(1,Qt::Horizontal,QString::fromUtf8("%"));
        setHeaderData(2,Qt::Horizontal,QString::fromUtf8("Стекло"));
        setHeaderData(3,Qt::Horizontal,QString::fromUtf8("Парт.гл."));
        setHeaderData(4,Qt::Horizontal,QString::fromUtf8("Модуль"));
    }
}

QVariant ModelConsStatData::data(const QModelIndex &item, int role) const
{
    if (role==Qt::ToolTipRole){
        QVariant id=ModelRo::data(index(item.row(),0),Qt::EditRole);
        if (!id.isNull()){
            QString str;
            QList<QString> l=inPar.values(id.toInt());
            foreach (QString s, l) {
                if (!str.isEmpty()){
                    str+="\n";
                }
                str+=s;
            }
            if (!str.isEmpty()){
                str.prepend(QString::fromUtf8("Параметры корректора ")+QString::fromUtf8(":\n"));
            }
            return str;
        }
    }
    return ModelRo::data(item,role);
}

void ModelConsStatData::refreshInPar()
{
    QSqlQuery qu;
    qu.prepare("select l.id_load, p.nam, l.val, l.temp, l.dat "
               "from glass_korr_load_par as l "
               "inner join glass_par as p on p.id=l.id_param "
               "where l.id_load = (select id_korr_load from glass_cons_load where id= :id_load)");
    qu.bindValue(":id_load",current_id_load);
    inPar.clear();
    if (qu.exec()){
        while (qu.next()){
            QString val=qu.value(1).toString()+QString::fromUtf8(" = ")+qu.value(2).toString();
            if (!qu.value(3).isNull()){
                val+=QString::fromUtf8(" (")+qu.value(3).toString()+QString::fromUtf8("°С)");
            }
            val+=" "+qu.value(4).toDate().toString("dd.MM.yy");
            inPar.insert(qu.value(0).toInt(),val);
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),qu.lastError().text(),QMessageBox::Cancel);
    }
}

ModelConsStatPar::ModelConsStatPar(QObject *parent) : ModelRo(parent)
{
    dec=3;
}

QVariant ModelConsStatPar::data(const QModelIndex &item, int role) const
{
    if (role==Qt::DisplayRole && item.column()==3){
        return QLocale().toString(data(item,Qt::EditRole).toDouble(),'f',1);
    }
    return ModelRo::data(item,role);
}

void ModelConsStatPar::refresh(int id_load, int id_part)
{
    QSqlQuery query;
    query.prepare("select jp.id_param, gp.nam, jp.val, jp.temp, jp.dat from "
                  "(select id_load, id_param, max(dat) as dat "
                  "from glass_cons_load_par as p where id_load = :id_load "
                  "and dat<=(select dat_part from parti where id = :id_part ) "
                  "group by id_param, id_load) as p "
                  "inner join glass_cons_load_par as jp on jp.id_load= p.id_load and jp.id_param=p.id_param and jp.dat=p.dat "
                  "inner join glass_par as gp on (gp.id=jp.id_param) "
                  "order by jp.id_param desc, jp.dat");
    query.bindValue(":id_load",id_load);
    query.bindValue(":id_part",id_part);
    if (execQuery(query)){
        setHeaderData(1,Qt::Horizontal,QString::fromUtf8("Параметр"));
        setHeaderData(2,Qt::Horizontal,QString::fromUtf8("Значен."));
        setHeaderData(3,Qt::Horizontal,QString::fromUtf8("Тизм.,°С"));
        setHeaderData(4,Qt::Horizontal,QString::fromUtf8("Дата"));
    }
}
