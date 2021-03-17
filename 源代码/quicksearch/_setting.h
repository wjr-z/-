#ifndef _SETTING_H
#define _SETTING_H
#include <unordered_map>
#include "myfile.h"
#include "KMP.h"
using namespace std;
void end_work();
class _setting {//设置值
public:
    QString *root_path,*FileName,*FileContent;
    int MAX_SIZE,is_thread;
    bool fun_1, fun_2,is_case_sensitive;
    unordered_map<string, bool>__exclude;
    _setting();
    ~_setting();
    void clear();
    bool _exclude(const string&_extension);
private:

};

#endif // _SETTING_H
