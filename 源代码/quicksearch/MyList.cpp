#include "MyList.h"
#include <algorithm>
MyQStringListItem::MyQStringListItem(const QString&path){
    nxt=nullptr;
    s=new QString(path);
}
MyQStringListItem::~MyQStringListItem(){
    s->~QString();
}
MyQStringList::MyQStringList(){
    head=nullptr;
    tail=nullptr;
    qwq=nullptr;
    size=0;
}
MyQStringList::~MyQStringList(){
    clear();
}
void MyQStringList::push_back(const QString&path){
    MyQStringListItem*p=new MyQStringListItem(path);
    ++size;
    if(head==nullptr)
        head=p;
     else
        tail->nxt=p;
    tail=p;
}
void MyQStringList::clear(){
    for(MyQStringListItem*i=head,*j;i!=nullptr;){
        j=i->nxt;
        delete i;
        i=j;
    }
    head=nullptr;
    tail=nullptr;
    if(qwq!=nullptr)delete [] qwq,qwq=nullptr;
    size=0;
}
void MyQStringList::sort(){
    qwq=new QString [size+1];
    int i=0;
    for(MyQStringListItem*p=head;p!=nullptr;p=p->nxt,++i)
        qwq[i]=*p->s;
    std::sort(qwq,qwq+size);
}
