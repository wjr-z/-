#ifndef MYFILE_H
#define MYFILE_H
#include <thread>
#include <QFile>
using namespace std;
class QuickFile{//读入的文件
private:
public:
    long long len;
    QFile file;
    uchar*ptr,*tail;
    QuickFile();
    ~QuickFile();
    void QuickFileRead(const char*path,const int &MinSize=0,const int&MaxSize=-1);
    void close();
};


#endif // MYFILE_H
