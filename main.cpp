#include "formrepnorm.h"
#include <QApplication>
#include "db/dblogin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/ico.ico"));
    QPixmap logo(":/images/simb_labl.png");
    DbLogin d(QString::fromUtf8("Выполнение норм : производство электродов"),logo);
    if (d.exec()!=QDialog::Accepted) exit(1);
    FormRepNorm w;
    w.show();

    return a.exec();
}
