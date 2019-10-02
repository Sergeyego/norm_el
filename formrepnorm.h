#ifndef FORMREPNORM_H
#define FORMREPNORM_H

#include <QWidget>
#include <QtGui>
#include <QtSql>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include "jobsqlmodel.h"
#include "zonwidget.h"
#include "db/dbtablemodel.h"

namespace Ui {
class FormRepNorm;
}

class FormRepNorm : public QWidget
{
    Q_OBJECT
    
public:
    explicit FormRepNorm(QWidget *parent = 0);
    ~FormRepNorm();
    
private:
    Ui::FormRepNorm *ui;
    ZonWidget *zonWidget;
    JobSqlModel *jobmodel;
    QSortFilterProxyModel *proxyJobModel;
    QString getProf(int id_rb, QDate date);
    DbRelation *relEd;
    QString signRep;
    bool getTotalVip(double &vip, int &d, double &kvo, const int id_rb, const int id_sm=-1);

private slots:
    void chSm();
    void upd();
    void saveXls();
    void saveXlsPer();
    void updTotal(QModelIndex index);
    void goRep();
};

#endif // FORMREPNORM_H
