#include "mainwindow.h"
#include <cstdlib>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile* logfile=new QFile(QCoreApplication::applicationDirPath()+QString::fromUtf8("/currentlog"));
    if(logfile->exists()) logfile->remove();
    logfile->open(QIODevice::ReadWrite);
    logfile->write(QString("Start simuLift<br>\n").toUtf8());
    logfile->close();
    delete logfile;
    MainWindow w;
    w.show();

    return a.exec();
}
