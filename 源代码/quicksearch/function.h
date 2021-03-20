#ifndef FUNCTION_H
#define FUNCTION_H
#include <mutex>
#include <vector>
#include <fstream>
#include <QList>
using namespace std;

inline string path_extension(char* s) {
    int len(strlen(s)), i(0);
    while (i < len && s[i] != '.')++i;
    ++i;
    if (i >= len)return "";
    string g;
    g.append(s+i,len-i);
    return g;
}
inline QString string_to_QString(string&str){
    return QString::fromLocal8Bit(str.data());
}
extern void MainTainPath(QString&s);


#endif // FUNCTION_H
