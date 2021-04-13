#include "Sunday.h"
#include "function.h"
Sunday::Sunday(){
    pattern=new QuickString;
    shift=new int [MaxNum];
    nxt=nullptr;
    length=0;
}
Sunday::~Sunday(){
    delete pattern;
    if(nxt!=nullptr)delete [] nxt;
    if(shift!=nullptr)delete [] shift;
}
void Sunday::initial(){
    if(nxt!=nullptr)delete [] nxt;
    nxt = new int [length + 1];
    int i(0), j(-1);
    nxt[0] = -1;
    if(case_sensitive_mode)
        toLower();
    while (i < length)
        (j==-1||pattern->s[i]==pattern->s[j])?nxt[++i]=++j:j=nxt[j];
    for(i=0;i<MaxNum;++i)
        shift[i]=length+1;
    for(i=0;i<length;++i)
        shift[chartoint(pattern->s[i])]=length-i;
}
bool Sunday::KMP(const uchar*s,const int&n){
    int i(0) , j(0);
    while (i < n && j < length)
        (j==-1||modec(s[i])==pattern->s[j])?++i,++j:j=nxt[j];
    return j==length;
}
bool Sunday::KMP(const char*s,const int&n){
    int i(0) , j(0);
    while (i < n && j < length)
        (j==-1||modec(s[i])==pattern->s[j])?++i,++j:j=nxt[j];
    return j==length;
}
bool Sunday::search(const char*s,const int&n){
    int i=0,delta=n-length,j,step=0,MAXSTEP=n<<1;
    while(i<delta){
        j=0;
        while(modec(s[i+j])==pattern->s[j])
            ++j,++step;
        if(j==length)return true;
        if(step>=MAXSTEP)return KMP(s+i,n-i);
        i+=shift[chartoint(modec(s[i+length]))];
    }
    if(i==delta){
        j=0;
        while(j<length&&modec(s[i+j])==pattern->s[j]){
            ++j;
        }
        if(j==length)return true;
    }
    return false;
}
bool Sunday::search(const uchar*s,const int&n){
    int i=0,delta=n-length,j,step=0,MAXSTEP=n<<1;
    while(i<delta){
        j=0;
        while(modec(s[i+j])==pattern->s[j])
            ++j,++step;
        if(j==length)return true;
        if(step>=MAXSTEP)return KMP(s+i,n-i);
        i+=shift[chartoint(modec(s[i+length]))];
    }
    if(i==delta){
        j=0;
        while(j<length&&modec(s[i+j])==pattern->s[j])
            ++j;
        if(j==length)return true;
    }
    return false;
}

void Sunday::toLower(){
    pattern->toLower();
}
