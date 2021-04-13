#include <io.h>
#include "SearchThread.h"
#include "function.h"
#include "mainwindow.h"
#include "QuickString.h"
#include <iostream>
#include <QDebug>

QuickSearchThread::QuickSearchThread(_setting*SETTING,Sunday*FILENAME,Sunday*FILECONTENT,int*NOWTHREAD)
    :setting(SETTING),FileName(FILENAME),FileContent(FILECONTENT),NowThread(NOWTHREAD)
{
    ReadFile=new QuickFile;
    wjr=new MyQStringList;
}
QuickSearchThread::~QuickSearchThread(){
    delete ReadFile,ReadFile=nullptr;
    delete wjr,wjr=nullptr;
}

void QuickSearchThread::clear(){
    wjr->clear();
    cnt1=cnt2=0;
}

void QuickSearchThread::getFiles(const char* path) {
    if(*NowThread)return;
    QuickString*p,*q;
    p=new QuickString,q=new QuickString;
    p->append(path)->append("\\*");
    int len=strlen(path)+1;
    _finddata_t fileinfo;
    long hFile;
    if (~(hFile=_findfirst(p->c_str(), &fileinfo))) {
        do {
            if(fileinfo.attrib&6)continue;
            ++SearchFile;
            q->resize(0);
            p->resize(len);
            q->append(fileinfo.name);
            p->append(q->c_str());
            if (setting->fun_1&&FileName->search(q->c_str(), q->nowlength)) {
                wjr->push_back(CharToQStr(q->c_str()),CharToQStr(p->c_str()));
                ++cnt1;
            }
            if (fileinfo.attrib & _A_SUBDIR) {
                if (strcmp(q->c_str(), ".") != 0 && strcmp(q->c_str(), "..") != 0)
                    getFiles(p->c_str());
            }
            else {
                if (!setting->fun_2 || setting->__exclude.count(path_extension(fileinfo.name)))
                    continue;
                ReadFile->QuickFileRead(p->c_str(),setting->MinSize,setting->MaxSize);
                SearchSize+=ReadFile->len;
                if (FileContent->search(ReadFile->ptr, ReadFile->len)) {
                    wjr->push_back(CharToQStr(q->c_str()),CharToQStr(p->c_str()),ReadFile->len);
                    ++cnt2;
                }
                ReadFile->close();
            }

        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
    delete p;
    delete q;
}


void QuickSearchThread::thread_get_Files_1() {
    if(*NowThread)return;
    QuickString*p,*q;
    p=new QuickString,q=new QuickString;
    int len=strlen(setting->root_path->toLocal8Bit())+1;
    p->append(setting->root_path->toLocal8Bit())->append("\\*");
    _finddata_t fileinfo;
    long hFile;
    int step(0);
    if (~(hFile=_findfirst(p->c_str(), &fileinfo))) {
        do {
            if(fileinfo.attrib&6)continue;
            ++step;
            if (!(step & 1)) continue;
            ++SearchFile;
            q->resize(0);
            p->resize(len);
            q->append(fileinfo.name);
            p->append(q->c_str());
            if (setting->fun_1&&FileName->search(q->c_str(), q->nowlength)) {
                wjr->push_back(CharToQStr(q->c_str()),CharToQStr(p->c_str()));
                ++cnt1;
            }
            if (fileinfo.attrib & _A_SUBDIR) {
                if (strcmp(q->c_str(), ".")!=0  && strcmp(q->c_str(), "..")!=0)
                     getFiles(p->c_str());
            }
            else {
                if (!setting->fun_2 || setting->__exclude.count(path_extension(fileinfo.name)))
                     continue;
                ReadFile->QuickFileRead(p->c_str(),setting->MinSize,setting->MaxSize);
                SearchSize+=ReadFile->len;
                if (FileContent->search(ReadFile->ptr, ReadFile->len)) {
                    wjr->push_back(CharToQStr(q->c_str()),CharToQStr(p->c_str()),ReadFile->len);
                    ++cnt2;
                }
                ReadFile->close();
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
    delete p;
    delete q;
}

void QuickSearchThread::thread_get_Files_2() {
    if(*NowThread)return;
    QuickString*p,*q;
    p=new QuickString,q=new QuickString;
    int len=strlen(setting->root_path->toLocal8Bit())+1;
    p->append(setting->root_path->toLocal8Bit())->append("\\*");
    _finddata_t fileinfo;
    long hFile;
    int step(0);
    if (~(hFile=_findfirst(p->c_str(), &fileinfo))) {
        do {
            if(fileinfo.attrib&6)continue;
            ++step;
            if (step & 1) continue;
            ++SearchFile;
            q->resize(0);
            p->resize(len);
            q->append(fileinfo.name);
            p->append(q->c_str());
            if (setting->fun_1&&FileName->search(q->c_str(), q->nowlength)) {
                wjr->push_back(CharToQStr(q->c_str()),CharToQStr(p->c_str()));
                ++cnt1;
            }
            if (fileinfo.attrib & _A_SUBDIR) {
                if (strcmp(q->c_str(), ".")!=0  && strcmp(q->c_str(), "..")!=0)
                    getFiles(p->c_str());
            }
            else {
                if (!setting->fun_2|| setting->__exclude.count(path_extension(fileinfo.name)))
                    continue;
                ReadFile->QuickFileRead(p->c_str(),setting->MinSize,setting->MaxSize);
                SearchSize+=ReadFile->len;
                if (FileContent->search(ReadFile->ptr, ReadFile->len)) {
                    wjr->push_back(CharToQStr(q->c_str()),CharToQStr(p->c_str()),ReadFile->len);
                    ++cnt2;
                }
                ReadFile->close();
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
    delete p;
    delete q;
}

