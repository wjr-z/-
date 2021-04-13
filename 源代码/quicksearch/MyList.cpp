#include "MyList.h"
#include <algorithm>
void MyQStringList::clear(){
    if(wjr!=nullptr)delete [] wjr,wjr=nullptr;
    Size=0;
    for(MyQStringListItem*i=head,*j;i!=nullptr;){
        j=i->nxt;
        delete i;
        i=j;
    }
    head=nullptr;
    tail=nullptr;
}
void MyQStringList::initial(){
    wjr=new mytableitem [Size+1];
    int i=0;
    for(MyQStringListItem*p=head;p!=nullptr;p=p->nxt,++i)
        wjr[i]=*p->wjr;
}

bool sort0(const mytableitem&a,const mytableitem&b){
    return *a.FileName<*b.FileName;
}

bool sort1(const mytableitem&a,const mytableitem&b){
    return *a.Path<*b.Path;
}

bool sort2(const mytableitem&a,const mytableitem&b){
    return a.Size<b.Size;
}

