#ifndef _SETTING_H
#define _SETTING_H
#include <unordered_map>
#include <QString>
#include "QuickString.h"
using namespace std;

class _setting {//设置值
public:
    QString *root_path,*FileName,*FileContent;
    int MinSize,MaxSize,is_thread;
    bool fun_1, fun_2,is_case_sensitive;
    unordered_map<string, bool>__exclude;
    _setting();//构造函数
    _setting(const _setting&other);//拷贝构造函数
    _setting&operator=(const _setting&other);//赋值运算函数
    ~_setting();
    void clear();
    bool _exclude(const string&_extension){
        return __exclude.count(_extension)!=0;
    }
private:

};

#endif // _SETTING_H
