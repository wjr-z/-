#include "_setting.h"


_setting::_setting() {//构造函数
    root_path =new QString;
    FileName=new QString;
    FileContent=new QString;
    MinSize = 0;
    MaxSize = 10485760;//限制搜索文件大小
    is_thread = 0;
    fun_1 = fun_2 = is_case_sensitive=false;
    __exclude[""] = __exclude["exe"] = __exclude["db"] = __exclude["dll"]
    = __exclude["jpg"] = __exclude["png"] = __exclude["webp"] = __exclude["gif"]
    = __exclude["zip"] = __exclude["rar"]=__exclude["a"]=__exclude["lib"]
    = __exclude["mp4"]
    = true;
}
_setting::_setting(const _setting&other){//拷贝构造函数
    root_path=new QString(*other.root_path);
    FileName=new QString(*FileName);
    FileContent=new QString(*other.FileContent);
    MinSize=other.MinSize;
    MaxSize=other.MaxSize;
    is_thread=other.is_thread;
    is_case_sensitive=other.is_case_sensitive;
    fun_1=other.fun_1;
    fun_2=other.fun_2;
    __exclude=other.__exclude;
}
_setting&_setting::operator=(const _setting&other){
    *root_path=*other.root_path;
    *FileName=*other.FileName;
    *FileContent=*other.FileContent;
    MinSize=other.MinSize;
    MaxSize=other.MaxSize;
    is_thread=other.is_thread;
    is_case_sensitive=other.is_case_sensitive;
    fun_1=other.fun_1;
    fun_2=other.fun_2;
    __exclude=other.__exclude;
}
_setting::~_setting(){
    delete root_path,root_path=nullptr;
    delete FileName,FileName=nullptr;
    delete FileContent,FileContent=nullptr;
}
void _setting::clear(){
    root_path->clear();
    FileName->clear();
    FileContent->clear();
    MinSize=0;
    MaxSize=10485760;
    is_thread = 0;
    fun_1 = fun_2 = is_case_sensitive=false;
}
