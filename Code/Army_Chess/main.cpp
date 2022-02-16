#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w; // Make sure the application only generates 1 MainWindow!!!
    w.show();
    return a.exec();
}
