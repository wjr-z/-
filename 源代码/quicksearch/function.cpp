#include <windows.h>
#include "io.h"
#include "function.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"



void MainTainPath(QString&s){
    s.replace("/","\\");
    while(s.endsWith("\\"))s.chop(1);
    s=s.trimmed();
}
