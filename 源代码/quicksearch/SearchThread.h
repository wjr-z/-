#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H
#include "MyList.h"
#include "function.h"
#include "quickfile.h"
#include "Sunday.h"
#include "_setting.h"
class QuickSearchThread{
private:
    _setting*setting;//设置项（全局）
    Sunday*FileName,*FileContent;//搜索项（全局）
    QuickFile *ReadFile;//读入文件（独有）
public:
    MyQStringList*wjr;//（独有）
    int cnt1,cnt2;//(独有)
    int*NowThread;
    int SearchFile;
    long long SearchSize;
    QuickSearchThread(_setting*SETTING=nullptr,Sunday*FILENAME=nullptr,Sunday*FILECONTENT=nullptr,int*NOWTHREAD=nullptr);
    ~QuickSearchThread();
    void clear();
    void getFiles(const char* path);
    void thread_get_Files_1();
    void thread_get_Files_2();
};



#endif // SEARCHTHREAD_H
