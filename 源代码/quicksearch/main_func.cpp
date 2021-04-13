#include <QProcess>
#include <QFileInfo>
#include <mainwindow.h>
#include <QDesktopServices>
#include <io.h>
#include "ui_mainwindow.h"
#include "SearchThread.h"
void MainWindow::show_string(const QString&s){
    ui->InforOutput->setText(s);
}
void MainWindow::end_work(){
    fclose(myout);
    mutex_number.unlock();
}
void MainWindow::searchinit(){
    ++now_thread;
    mutex_number.lock();
    --now_thread;
    if(now_thread)
        return;
    thread1->clear();
    thread2->clear();
    setting->clear();
    thread1->SearchFile=thread1->SearchSize
    =thread2->SearchFile=thread2->SearchSize
    =0;
    errno_t err;
    if (_access("output", 0) == -1)    //如果文件夹不存在
           system("mkdir output");
    err=fopen_s(&myout,"output\\out.txt","wb");
    if (err){
         end_work();
         return;
    }
}

#include <windows.h>
void MainWindow::work(){

    searchinit();
    if(now_thread){
        end_work();
        return;
    }
    emit StartTimerSignal(search,1000);

    QString tempStr(ui->path_input->text());

    *setting->root_path=tempStr;

    tempStr=ui->file_name_input->text();
    *setting->FileName=tempStr;

    tempStr=ui->file_content_input->text();
    *setting->FileContent=tempStr;

    tempStr=ui->max_size_input->text();
    setting->MaxSize=tempStr.toInt();

    tempStr=ui->min_size_input->text();
    setting->MinSize=tempStr.toInt();

    setting->is_thread=ui->is_thread->isChecked()==true;

    setting->is_case_sensitive=ui->is_case_sensetive->isChecked()==true;

    start_time=clock();
    emit ShowInfo("正在搜索!");

    FileName->setpattern(*setting->FileName);
    FileName->initial();
    FileName->setmode(setting->is_case_sensitive);
    FileContent->setpattern(*setting->FileContent);
    FileContent->initial();
    FileContent->setmode(setting->is_case_sensitive);
    if (setting->FileName->length())setting->fun_1 = true;
    if (setting->FileContent->length())setting->fun_2 = true;

    setting->is_thread?quick_search():normal_search();

    emit StopTimerSignal(search);

    print();
}

void MainWindow::print(){
    if(now_thread){
        end_work();
        return;
    }
    end_time=clock();
    QString str;
    str=QString("在%1s内共找到 %2个包含文件名的文件 %3个包含文件内容的文件").arg(QString::number((end_time-start_time)/1000.0,'f',2))
            .arg(thread1->cnt1+thread2->cnt1).arg(thread1->cnt2+thread2->cnt2);
    emit ShowInfo(str);
    Merge(thread1->wjr,thread2->wjr);
    ListShow();
    return end_work();
}

void MainWindow::add_file_list(mytableitem*p){
    QTableWidgetItem*a=new QTableWidgetItem;
    a->setText(*p->FileName);
    a->setIcon(icon_provider.icon(*p->Path));
    ui->file_list->setItem(show_size,0,a);
    QTableWidgetItem*b=new QTableWidgetItem;
    b->setText(*p->Path);
    ui->file_list->setItem(show_size,1,b);
    QTableWidgetItem*c=new QTableWidgetItem;
    c->setText(getSize(p->Size));
    ui->file_list->setItem(show_size,2,c);
    ++show_size;
}

void MainWindow::use_thread(){
    std::thread t(&MainWindow::work,this);
    t.detach();
}


void MainWindow::my_open_file(QTableWidgetItem*p){
    QDesktopServices::openUrl("file:///"+getPath(p));
}
void MainWindow::normal_search(){
    string s=setting->root_path->toStdString();
    thread1->getFiles(s.c_str());
}
void MainWindow::quick_search(){
    std::thread t1(&QuickSearchThread::thread_get_Files_1,thread1);
    std::thread t2(&QuickSearchThread::thread_get_Files_2,thread2);
    t1.join();
    t2.join();
}

void MainWindow::ListClear(){
    ui->file_list->reset();
    show_size=0;
}

void MainWindow::ListSort(mytableitem*head){
    switch(sortmode){
    case 0:
        return sort(head,head+list_size,sort0);
    case 1:
        return sort(head,head+list_size,sort1);
    case 2:
        return sort(head,head+list_size,sort2);
    default:
        return sort(head,head+list_size,sort0);
    }
}

void MainWindow::Merge(MyQStringList*p,MyQStringList*q){

    ListClear();
    for(int i=0;i<3;++i)
        if(List[i]!=nullptr)
            delete [] List[i],List[i]=nullptr;
    ListPage=1;
    p->initial(),q->initial();
    int n=p->Size,m=q->Size;
    int i(0),j(0);

    for(int i=0;i<3;++i)
        List[i]=new mytableitem [n+m];
    list_size=n+m;
    MaxPage=(list_size-1)/PageSize+1;

    mytableitem*(head[3]);
    for(int i=0;i<3;++i)
        head[i]=List[i];

    while(i<n){
        for(int k=0;k<3;++k)
            *(head[k]++)=p->wjr[i];
        ++i;
    }
    while(j<m){
        for(int k=0;k<3;++k)
            *(head[k]++)=q->wjr[j];
        ++j;
    }
    int pre=sortmode;
    for(int i=0;i<3;++i)
        sortmode=i,ListSort(List[i]);//初始化所有升序
    sortmode=pre;
}
void MainWindow::ListShow(){
    if(!list_size)return;
    mytableitem*L,*R;
    int l=(ListPage-1)*PageSize;
    int r=min(list_size,ListPage*PageSize)-1;
    if(sortmode&1){//降序，在升序基础上略微修改一点
        l=antirow(l);
        r=antirow(r);
        QuickSwap(l,r);
    }
    L=List[sortmode>>1]+l;
    R=List[sortmode>>1]+r;
    ui->file_list->setRowCount(R-L+1);
    if(sortmode&1){
        while(R>=L)
            emit slow_add_file_list(R--);
    }else{
        while(L<=R)
            emit slow_add_file_list(L++);
    }
    PageShow();
}

void MainWindow::PageShow(){
    ui->TurnToPage->setText(QString("%1").arg(ListPage));
    ui->MaxPage->setText(QString("/%1").arg(MaxPage));
}

void MainWindow::statusBarShow(QTableWidgetItem*p){
    ui->statusBar->clearMessage();
    ui->statusBar->showMessage("路径:"+getPath(p));
}

void MainWindow::UpdateInfo(){
    show_string(QString("用时%1s,已搜索%2个文件夹,"+getSize(thread1->SearchSize+thread2->SearchSize)+"的文件")
                .arg(QString::number((UsedTime+=deltatime)/1000.0,'f',2))
                .arg(thread1->SearchFile+thread2->SearchFile));
}

void MainWindow::StartTimer(QTimer*w,const int&ctime){
    UsedTime=0;
    setdeltatime(ctime);
    w->start(ctime);
}
void MainWindow::StopTimer(QTimer*w){
    w->stop();
}
