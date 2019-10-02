#ifndef ZONWIDGET_H
#define ZONWIDGET_H

#include <QWidget>
#include <QtGui>
#include <QtSql>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>

class ZonCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    ZonCheckBox(const QString & text, QWidget * parent = 0);
    void setId(int id_zon);
    int getId();
private:
    int idzon;
};

class ZonWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZonWidget(QWidget *parent = 0);
    QString getSuf();
    bool pres();
    bool pack();
    
signals:
    
public slots:
private:
    ZonCheckBox **zonCheckBox;
    int N;

private slots:
    void check();
signals:
    void supd();
};

#endif // ZONWIDGET_H
