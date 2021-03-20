#include <io.h>
#include "SearchThread.h"
#include "function.h"
#include "mainwindow.h"
#include <iostream>

QuickSearchThread::QuickSearchThread(_setting*SETTING,KMP*FILENAME,KMP*FILECONTENT,int*RESTHREAD,int*NOWTHREAD)
    :setting(SETTING),FileName(FILENAME),FileContent(FILECONTENT),ResThread(RESTHREAD),NowThread(NOWTHREAD)
{
    ReadFile=new my_file;
    wjr=new MyQStringList;
    ReadFile->ResThread=ResThread;
}
QuickSearchThread::~QuickSearchThread(){
    delete ReadFile;
    delete wjr;
}

void QuickSearchThread::clear(){
    ReadFile->clear();
    wjr->clear();
    cnt1=cnt2=0;
}

void QuickSearchThread::getFiles(string& path) {
    if(*NowThread)return;
    string p(path+"\\*") ,q;
    struct _finddata_t fileinfo;
    long hFile;
    if ((hFile=_findfirst(p.c_str(), &fileinfo))!=-1) {
        do {
             q = fileinfo.name;
             p = path + "\\" + q;
             if (setting->fun_1&&FileName->find(q, q.length())) {
                 wjr->push_back(string_to_QString(p));
                 ++cnt1;
             }
             if ((fileinfo.attrib & _A_SUBDIR)) {
                 if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
                     getFiles(p);
                 }
             }
             else {
                 if (!setting->fun_2 || (fileinfo.attrib & _A_SYSTEM) || setting->__exclude.count(path_extension(fileinfo.name))) {
                     continue;
                 }
                 ReadFile->ReadFilesIntoString(p,setting->MAX_SIZE);
                 if (FileContent->find(ReadFile->__file, ReadFile->siz)) {
                     wjr->push_back(string_to_QString(p));
                     ++cnt2;
                 }
             }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}


void QuickSearchThread::thread_get_Files_1() {
    ++*ResThread;
    if(*NowThread){--*ResThread;return;}
    string p(setting->root_path->toLocal8Bit() + "\\*"), q;
    struct _finddata_t fileinfo;
    long hFile;
    int step(0);
    if ((hFile=_findfirst(p.c_str(), &fileinfo))!=-1) {
        do {
            ++step;
            if (!(step & 1)) continue;
            q = fileinfo.name;
            p = string(setting->root_path->toLocal8Bit().data()) + "\\" + q;
            if (setting->fun_1&&FileName->find(q, q.length())) {
                wjr->push_back(string_to_QString(p));
                ++cnt1;
            }
            if ((fileinfo.attrib & _A_SUBDIR)) {
                if (strcmp(fileinfo.name, ".")!=0  && strcmp(fileinfo.name, "..")!=0)
                     getFiles(p);
            }
            else {
                if (!setting->fun_2 || (fileinfo.attrib & _A_SYSTEM) || setting->__exclude.count(path_extension(fileinfo.name)))
                     continue;
                ReadFile->ReadFilesIntoString(p,setting->MAX_SIZE);
                if (FileContent->find(ReadFile->__file, ReadFile->siz)) {
                    wjr->push_back(string_to_QString(p));
                    ++cnt2;
                }
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
    --*ResThread;
}

void QuickSearchThread::thread_get_Files_2() {
    ++*ResThread;
    if(*NowThread){--*ResThread;return;}
    string p(setting->root_path->toLocal8Bit() + "\\*"), q;
    struct _finddata_t fileinfo;
    long hFile;
    int step(0);
    if ((hFile=_findfirst(p.c_str(), &fileinfo))!=-1) {
        do {
            ++step;
            if (step & 1) continue;
            q = fileinfo.name;
            p = string(setting->root_path->toLocal8Bit().data()) + "\\" + q;
            if (setting->fun_1&&FileName->find(q, q.length())) {
                wjr->push_back(string_to_QString(p));
                ++cnt1;
            }
            if ((fileinfo.attrib & _A_SUBDIR)) {
                if (strcmp(fileinfo.name, ".")!=0  && strcmp(fileinfo.name, "..")!=0)
                    getFiles(p);
            }
            else {
                if (!setting->fun_2 || (fileinfo.attrib & _A_SYSTEM) || setting->__exclude.count(path_extension(fileinfo.name)))
                    continue;
                ReadFile->ReadFilesIntoString(p,setting->MAX_SIZE);
                if (FileContent->find(ReadFile->__file, ReadFile->siz)) {
                    wjr->push_back(string_to_QString(p));
                    ++cnt2;
                }
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
    --*ResThread;
}

