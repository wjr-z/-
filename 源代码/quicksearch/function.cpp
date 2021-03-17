#include <windows.h>
#include "io.h"
#include "function.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

string path_extension(char* s) {
    int len(strlen(s)), i(0);
    while (i < len && s[i] != '.')++i;
    ++i;
    if (i >= len)return "";
    string g;
    g.append(s+i,len-i);
    return g;
}
QString string_to_QString(string&str){
    return QString::fromLocal8Bit(str.data());
}

void MainTainPath(QString&s){
    s.replace("/","\\");
    while(s.endsWith("\\"))s.chop(1);
    s=s.trimmed();
}
