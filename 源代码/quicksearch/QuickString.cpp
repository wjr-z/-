#include "QuickString.h"
QuickString::QuickString(){
    s=new char [1];
    s[0]='\0';
    length=1;
    nowlength=0;
}
QuickString::QuickString(const QuickString&other){
    resize(0);
    append(other.s);
}
QuickString::~QuickString(){
    delete [] s;
}

void QuickString::maintain(const int&len) {
    if(length>=len)return;
    if(len>>11){
        while(length<len)length<<=1;
    }else{
        while(length<len)length<<=2;
    }
    char*g=new char [length];
    memcpy(g,s,nowlength+1);
    delete [] s;
    s=g;
}
QuickString* QuickString::append(const char* g) {
    int len=strlen(g);
    maintain(nowlength+len+1);
    memcpy(s+nowlength,g,len);
    nowlength+=len;
    s[nowlength]='\0';
    return this;
}
QuickString*QuickString::operator+=(const char*g){
    return append(g);
}
void QuickString::resize(const int&len){
    maintain(len+1);
    nowlength=len;
    s[nowlength]='\0';
}
void QuickString::toLower(){
    for(int i=0;i<nowlength;++i)
        s[i]=tolower(s[i]);
}
