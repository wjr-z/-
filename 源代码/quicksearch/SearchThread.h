#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H
#include <string>
#include "MyList.h"
#include "function.h"
#include "_setting.h"
class QuickSearchThread{
private:
    _setting*setting;//设置项（全局）
    KMP*FileName,*FileContent;//搜索项（全局）
    my_file *ReadFile;//读入文件（独有）
public:
    MyQStringList*wjr;//（独有）
    int cnt1,cnt2;//(独有)
    int*ResThread,*NowThread;
    QuickSearchThread(_setting*SETTING=nullptr,KMP*FILENAME=nullptr,KMP*FILECONTENT=nullptr,int*RESTHREAD=nullptr,int*NOWTHREAD=nullptr);
    ~QuickSearchThread();
    void clear();
    void getFiles(string& path);
    void thread_get_Files_1();
    void thread_get_Files_2();
};



#endif // SEARCHTHREAD_H
