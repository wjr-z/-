#include "KMP.h"
#include "function.h"
KMP::KMP(bool*is)
    :length(0),is_case_sensitive(is)
{
    s=new QString;
    nxt=nullptr;
}
KMP::~KMP(){
    if(nxt!=nullptr)delete [] nxt;
    delete(s);
}
void KMP::get_nxt() {
    nxt = new int [length + 1];
    int i(0), j(-1);
    nxt[0] = -1;
    if(!*is_case_sensitive){
        *s=s->toLower();
    }
    while (i < length) {
        if (j == -1 || (*s)[i] == (*s)[j])
            nxt[++i] = ++j;
        else j = nxt[j];
    }
}
bool KMP::find(std::string&g, const int&n) {
    int i(0) , j(0);
    if(!*is_case_sensitive){
        while (i < n && j < length) {
            if (j == -1||tolower(g[i])==(*s)[j])
                ++i, ++j;
            else j = nxt[j];
        }
    }else{
        while (i < n && j < length) {
            if (j == -1||g[i]==(*s)[j])
                ++i, ++j;
            else j = nxt[j];
        }
    }
    return j==length;
}
void KMP::clear(){
    if(nxt!=nullptr)delete [] nxt,nxt=nullptr;
    s->clear();
}
void KMP::init(QString*p){
    *s=*p;
    length=s->length();
    get_nxt();
}
