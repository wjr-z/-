#include "quickfile.h"
#include "function.h"
QuickFile::QuickFile(){

}
QuickFile::~QuickFile(){

}
void QuickFile::QuickFileRead(const char*path,const int &MinSize,const int&MaxSize){
    file.setFileName(QString::fromLocal8Bit(path));
    file.open(QIODevice::ReadOnly);
    len=file.size();
    if(~MaxSize&&(len<MinSize||len>MaxSize)){
        len=0;
        file.close();
        ptr=nullptr;
        return;
    }
    ptr=file.map(0,len);
    if(!ptr){
        len=0;
        file.close();
        ptr=nullptr;
        return;
    }
    tail=ptr+len;
}
void QuickFile::close(){
    if(len)file.unmap(ptr);
    file.close();
}
