#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "Application started";
    MainWindow w;
    w.show();
    qDebug() << "Main window shown";
    return a.exec();
}
