#ifndef JOBSQLMODEL_H
#define JOBSQLMODEL_H

#include <QtSql>
#include <QObject>
#include <QMessageBox>
#include "db/dbtablemodel.h"


class JobSqlModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit JobSqlModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index,int role = Qt::DisplayRole) const;
    void refresh(bool emp, QString zonSuf, bool fsm, int idSm, QDate dbeg, QDate dend, bool po);
    DbRelation *relSm;

private:
    QDate begdate;
    QDate enddate;
    bool fSm;
    int  smId;
signals:
    void sigUpd();
};

#endif // JOBSQLMODEL_H
