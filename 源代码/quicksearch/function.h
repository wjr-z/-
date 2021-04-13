#ifndef FUNCTION_H
#define FUNCTION_H
#include <mutex>
#include <vector>
#include <fstream>

#include <QString>
#include <QFile>
using namespace std;

#define charpoint 46

inline string path_extension(const char*s) {
    int len(strlen(s)), i(len-1);
    while ((~i) && (s[i] ^ charpoint))--i;
    if(~i){
        ++i;
        string g;
        while(i<len)
            g.push_back(s[i++]);
        return g;
    }else return "";
}
inline QString StrToQStr(const string&str){
    return QString::fromLocal8Bit(str.data());
}inline QString CharToQStr(const char*str){
    return QString::fromLocal8Bit(str);
}
extern void MainTainPath(QString&s);
template <class T>
inline int getsize(T*const&p){
    return _msize(p)/sizeof(p);
}
inline QString getSize(long long x){
    return (x>>10)?(x>>20)?(x>>30)?QString("%1GB").arg(x>>30):QString("%1MB").arg(x>>20):QString("%1KB").arg(x>>10):QString("%1B").arg(x);
}
inline void QuickSwap(int&a,int&b){
    a^=b;
    b^=a;
    a^=b;
}

inline int chartoint(const char&s){
    return int(uchar(s));
}
inline int uchartoint(const uchar&s){
    return int(s);
}
inline bool FileExists(const QString&path){
    QFile file(path);
    return file.exists();
}
#endif // FUNCTION_H
