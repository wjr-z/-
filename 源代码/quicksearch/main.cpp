#include "mainwindow.h"

#include <QApplication>
#include <windows.h>

#pragma execution_character_set("utf-8")
int main(int argc, char *argv[])
{
    SetConsoleOutputCP(65001);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
