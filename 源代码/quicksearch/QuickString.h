#ifndef QUICKSTRING_H
#define QUICKSTRING_H
#include <string.h>
#include <QString>
class QuickString {
public:
    char*s;
    int length,nowlength;//length代表总长度,nowlength代表字符串长度
    QuickString();
    QuickString(const QuickString&other);
    QuickString&operator=(const QString&other){
        resize(0);
        append(other.toLocal8Bit());
        return *this;
    }
    ~QuickString();
    void maintain(const int&len);
    QuickString* append(const char* g);
    QuickString*operator+=(const char*g);
    void resize(const int&len=0);
    const char*c_str(){return s;}
    void toLower();
};
#endif // QUICKSTRING_H
