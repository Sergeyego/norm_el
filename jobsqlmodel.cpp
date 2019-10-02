#include "jobsqlmodel.h"
#include <QtGui>


JobSqlModel::JobSqlModel(QObject *parent) :
    QSqlQueryModel(parent)
{
    relSm = new DbRelation("select id, fname from wire_smena order by fname",0,1,this);
}

void JobSqlModel::refresh(bool emp, QString zonSuf, bool fsm, int idSm, QDate dbeg, QDate dend, bool po)
{
    begdate=dbeg;
    enddate=dend;
    smId=idSm;
    fSm=fsm;
    QString sufSm;
    if (idSm!=-1) {
        sufSm=" and (CASE WHEN exists "
              "(select rj.id from rab_job as rj inner join rab_nams as rn on rj.lid=rn.lid where rj.datf=j.datf and rj.id_rb=j.id_rb and rn.id=28) "
              "THEN 1 ELSE 0 END) = "+QString::number(idSm);
    } else {
        sufSm=" ";
    }
    QString order;
    if (!fsm){
        order= emp? " order by sm, p.nam, r.snam, j.datf" : " order by r.snam, j.datf, sm, p.id";
    } else {
        order=" group by j.id_rb, r.snam order by r.snam";
    }

    QString qu;
    if (!fsm){
        qu="select j.id, j.datf, "
           "(CASE WHEN exists "
           "(select rj.id from rab_job as rj inner join rab_nams as rn on rj.lid=rn.lid where rj.datf=j.datf and rj.id_rb=j.id_rb and rn.id=28) "
           "THEN 1 ELSE 0 END) as sm, "
           "p.nam, r.snam, j.chas_sm, right(n.fnam, length(n.fnam)-COALESCE(length(lt.liter),0)), "
           "get_norm_el(j.id, 11) as norms11, "
           "get_norm_el(j.id, j.chas_sm) as norm, "
           "j.kvo_fact, "
           "j.kvo_fact*100/get_norm_el(j.id, j.chas_sm) as vip, "
           "j.prim, "
           "j.id_rb, lt.id_ed "
           "from rab_job as j "
           "inner join pres as p on j.id_press=p.id "
           "inner join rab_rab as r on j.id_rb=r.id "
           "inner join rab_nams as n on j.lid=n.lid "
           "inner join rab_liter as lt on n.id=lt.id "
           "where j.datf between '"+begdate.toString("yyyy-MM-dd")+"' and '"
           +enddate.toString("yyyy-MM-dd")+"'";
    } else {
        qu="select NULL, NULL, NULL, NULL, r.snam, NULL, NULL, NULL, NULL, NULL, "
                "sum(j.kvo_fact*100/get_norm_el(j.id, j.chas_sm) )/ "
                "(select count(distinct datf) from rab_job where datf between '"+begdate.toString("yyyy-MM-dd")+"' and '"
                +enddate.toString("yyyy-MM-dd")+"' and id_rb = j.id_rb)as vip, "
                "NULL, j.id_rb, NULL "
                "from rab_job as j "
                "inner join rab_rab as r on j.id_rb=r.id "
                "inner join rab_nams as n on j.lid=n.lid "
                "inner join rab_liter as lt on n.id=lt.id "
                "where j.datf between '"+begdate.toString("yyyy-MM-dd")+"' and '"
                +enddate.toString("yyyy-MM-dd")+"' and lt.id_ed=1 and get_norm_el(j.id, j.chas_sm) is not NULL";
    }
    if (po) qu=qu.replace("j.kvo_fact","j.kvo");
    setQuery(qu+sufSm+zonSuf+order);
    if (lastError().isValid())
    {
        QMessageBox* errmes= new QMessageBox("Error", lastError().text(), QMessageBox::Critical,NULL,QMessageBox::Cancel,NULL);
        errmes->exec();
        delete errmes;
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Дата"));
        setHeaderData(2, Qt::Horizontal,tr("Смена"));
        setHeaderData(3, Qt::Horizontal,tr("Пресс"));
        setHeaderData(4, Qt::Horizontal,tr("Бригадир"));
        setHeaderData(5, Qt::Horizontal,tr("Отр,ч."));
        setHeaderData(6, Qt::Horizontal,tr("Задание"));
        setHeaderData(7, Qt::Horizontal,tr("Нор.11"));
        setHeaderData(8, Qt::Horizontal,tr("Норма"));
        setHeaderData(9, Qt::Horizontal,tr("Вып."));
        setHeaderData(10, Qt::Horizontal,tr("% вып."));
        setHeaderData(11, Qt::Horizontal,tr("Примечание"));
        emit sigUpd();
    }
}

QVariant JobSqlModel::data(const QModelIndex &index,int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    switch (role) {
    case Qt::DisplayRole: // Данные для отображения
    {
        if (index.column()==1) {
            return value.isNull()? QVariant() : QSqlQueryModel::data(index,Qt::EditRole).toDate().toString("dd.MM.yy");
        }
        if (index.column()==2) {
            return value.isNull()? QVariant() : relSm->data(QString::number(value.toInt()));
        }
        if (index.column()>=7 && index.column()<=9) {
            return value.isNull() ? QVariant() : QString("%1").arg(QSqlQueryModel::data(index,Qt::EditRole).toDouble(),0,'f',3);
        }
        if (index.column()==10) {
            return value.isNull() ? QString("-") : QString("%1").arg(value.toDouble(),0,'f',2);
        } else return value;
        break;
    }
    case Qt::EditRole: // Данные для редактирования
    {
        if (index.column()==2) {
            return value.isNull()? QVariant() : relSm->data(QString::number(value.toInt()));
        } else return value;
        break;
    }
    case Qt::TextAlignmentRole: // Выравнивание
        if(index.column() == 5 || (index.column()>=7 && index.column()<=10) )
            return int(Qt::AlignRight | Qt::AlignVCenter);
        else return int(Qt::AlignLeft | Qt::AlignVCenter);
        break;
    }
    return value;
}

