#ifndef KMP_H
#define KMP_H

#include <QString>
#include "QuickString.h"
class Sunday{
private:
    const int MaxNum=256;
    int*nxt;//KMP所用nxt数组
    int*shift;//Sunday数组,uchar范围是0~255
    QuickString*pattern;
    int length;//模式串长度
    bool*case_sensitive_mode;
public:
    Sunday();
    ~Sunday();
    void initial();//初始化nxt和shift数组
    void toLower();//大小写不敏感，则全部先变成小写再比较
    bool KMP(const uchar*g,const int&n);
    bool KMP(const char*g,const int&n);
    void setpattern(const QString&other){
        *pattern=other;
        length=other.length();
    }
    bool search(const char*g,const int&n);
    bool search(const uchar*g,const int&n);
    char modec(const char&c){return (*case_sensitive_mode)?c:tolower(c);}
    void setmode(bool&mode){case_sensitive_mode=&mode;}
};

#endif // KMP_H
