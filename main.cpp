
#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Diagram");
    w.setWindowIcon(QIcon(":/ico/icon.png"));
    w.setMinimumSize(450,450);
    w.resize(450,450);
    w.show();
    return a.exec();
}
