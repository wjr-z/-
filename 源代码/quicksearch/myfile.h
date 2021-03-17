#ifndef MYFILE_H
#define MYFILE_H
#include <thread>
using namespace std;
class my_file{//读入的文件
private:
    int len,num;
    char*str,*p1,*p2,*head;
    const int MAXN=1<<22;//优化磁盘读写,4M休眠一会
    errno_t err;
    FILE*fp;
    char*buff;
public:
    int*ResThread;
    long long siz;
    string __file;
    my_file();
    ~my_file();
    void clear();
    bool read_FILE(std::string& s, int MAX_SIZE);
    void getline(std::string& s);
    void getall(std::string& s);
    void get_index(int& x);
    void ReadFilesIntoString(string& s,const int &MAXSIZE);
};


#endif // MYFILE_H
