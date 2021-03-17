#include "_setting.h"
_setting::_setting() {//默认值
    root_path =new QString;
    FileName=new QString;
    FileContent=new QString;
    MAX_SIZE = 10485760;//限制搜索文件大小
    is_thread = 0;
    fun_1 = fun_2 = is_case_sensitive=false;
    __exclude[""] = __exclude["exe"] = __exclude["db"] = __exclude["dll"]
    = __exclude["jpg"] = __exclude["png"] = __exclude["webp"] = __exclude["gif"]
    = __exclude["zip"] = __exclude["rar"]=__exclude["a"]=__exclude["lib"]
    = true;
}
_setting::~_setting(){
    delete(root_path);
    delete(FileName);
    delete(FileContent);
}
void _setting::clear(){
    root_path->clear();
    FileName->clear();
    FileContent->clear();
    MAX_SIZE = 10000000;//限制搜索文件大小
    is_thread = 0;
    fun_1 = fun_2 = is_case_sensitive=false;
}
bool _setting::_exclude(const string &_extension){
    return __exclude.count(_extension)!=0;
}
