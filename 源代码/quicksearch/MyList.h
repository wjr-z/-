#ifndef MYLIST_H
#define MYLIST_H
#include <QString>

struct mytableitem{//用于存放每一行的 文件名，路径，文件大小等
    QString*FileName,*Path;
    int Size;
    mytableitem(const QString&filename="",const QString path="",const int&size=0)
        :FileName(new QString(filename)),Path(new QString(path)),Size(size)
    {
    }
    mytableitem(const mytableitem&other)
        :FileName(new QString(*other.FileName)),Path(new QString(*other.Path)),Size(other.Size)
    {
    }
    mytableitem&operator=(const mytableitem&other){
        if(this==&other)return *this;
        *FileName=*other.FileName;
        *Path=*other.Path;
        Size=other.Size;
        return *this;
    }
    ~mytableitem(){
        delete FileName,FileName=nullptr;
        delete Path,Path=nullptr;
    }

};

struct MyQStringListItem{
    MyQStringListItem*nxt;
    mytableitem*wjr;
    MyQStringListItem(const QString&filename="",const QString&path="",const int&size=0)
        :nxt(nullptr),wjr(new mytableitem(filename,path,size))
    {
    }
    MyQStringListItem(const MyQStringListItem&other)
        :nxt(other.nxt),wjr(new mytableitem(*other.wjr->FileName,*other.wjr->Path,other.wjr->Size))
    {
    }
    ~MyQStringListItem(){
        delete wjr,wjr=nullptr;
        nxt=nullptr;
    }
};

struct MyQStringList{
    MyQStringListItem*head,*tail;
    mytableitem*wjr;
    int Size;
    MyQStringList()
        :head(nullptr),tail(nullptr),wjr(nullptr),Size(0)
    {
    }
    ~MyQStringList(){
        clear();
    }
    void push_back(const QString&filename="",const QString&path="",const int&size=0){
        MyQStringListItem*p=new MyQStringListItem(filename,path,size);
        ++Size;
        head==nullptr?head=p:tail->nxt=p;
        tail=p;
    }
    void clear();
    void initial();
};
extern bool sort0(const mytableitem&a,const mytableitem&b);
extern bool sort1(const mytableitem&a,const mytableitem&b);
extern bool sort2(const mytableitem&a,const mytableitem&b);


#endif // MYLIST_H
