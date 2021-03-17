#include "myfile.h"
extern int res_thread;
void my_file::clear() {
    len = 1;
    p1 = p2 = str;
    if(str!=nullptr)free(str),str=nullptr;
    str=(char*)malloc(sizeof(char));
}
my_file::my_file() {
    len = 1;
    p1 = p2 = str;
    str=(char*)malloc(sizeof(char));
    buff=(char*)malloc(sizeof(char)*(1<<22));//4M缓冲区
}
my_file::~my_file(){
    if(str!=nullptr)free(str);
    if(buff!=nullptr)free(buff);
}
bool my_file::read_FILE(std::string& s, int MAX_SIZE) {//读入一个文件
    err = fopen_s(&fp, s.c_str(), "rb");
    if (err) {
        siz = 0;
        return false;
    }
    setvbuf(fp, buff, _IOFBF, 1<<22);
    fseek(fp, 0, SEEK_END);
    siz = ftell(fp);
    if (siz >= MAX_SIZE) {//限制查找文件的大小
        siz=0;
        fclose(fp);
        return false;
    }
    rewind(fp);
    if (len < siz) {//自制简易vector
        while (len < siz)
            len <<= 1;
        if(str!=nullptr)free(str),str=nullptr;
        str = (char*)malloc(sizeof(char)*len);
    }

    if (siz < MAXN||*ResThread<2)fread(str, 1, siz, fp);
    else {//磁盘读写优化
        head=str;
        while (num= fread(head, 1, MAXN, fp)){
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            head+=num;
        }
    }
    p1 = str;
    p2 = str + siz;
    fclose(fp);
    return true;
}
void my_file::ReadFilesIntoString(string& s,const int &MAXSIZE) {
    __file="";
    if (!read_FILE(s, MAXSIZE))return;
    getall(__file);
}

void my_file::getline(std::string& s) {
    while (*p1 != '\r' && *p1 != '\n' && p1 < p2)
        s.push_back(*p1++);
    if (*p1 == '\r')++p1;
    if (*p1 == '\n')++p1;
}
void my_file::getall(std::string& s) {
    s.append(p1,p2-p1);
}
void my_file::get_index(int& x) {
    x = 0;
    while (*p1 != '\r' && *p1 != '\n' && p1 < p2)
        x = (x << 1) + (x << 3) + *(p1++) - '0';
    if (*p1 == '\r')++p1;
    if (*p1 == '\n')++p1;
}
