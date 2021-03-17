#include <QProcess>
#include <QFileInfo>
#include <mainwindow.h>
#include <QDesktopServices>
#include "io.h"
#include "ui_mainwindow.h"
#include "SearchThread.h"
void MainWindow::show_string(const QString&s){
    ui->end_output->setText(s);
}
void MainWindow::end_work(){
    fclose(myout);
    mutex_number.unlock();
}
void MainWindow::init(){
    ++now_thread;
    mutex_number.lock();
    --now_thread;
    if(now_thread)
        return;
    thread1->clear();
    thread2->clear();
    setting->clear();
    FileName->clear();
    FileContent->clear();

    errno_t err;
    if (_access("output", 0) == -1)    //如果文件夹不存在
           system("mkdir output");
    err=fopen_s(&myout,"output\\out.txt","wb");
    if (err){
         end_work();
         return;
    }
}

void MainWindow::work(){

    init();
    if(now_thread){
        end_work();
        return;
    }

    QString tempStr(ui->path_input->text());

    MainTainPath(tempStr);
    *setting->root_path=tempStr;

    tempStr=ui->file_name_input->text();
    *setting->FileName=tempStr;

    tempStr=ui->file_content_input->text();
    *setting->FileContent=tempStr;

    tempStr=ui->max_size_input->text();
    setting->MAX_SIZE=tempStr.toInt();

    setting->is_thread=ui->is_thread->isChecked()==true;

    setting->is_case_sensitive=ui->is_case_sensetive->isChecked()==true;

    start_time=clock();
    emit show_end("正在搜索!");

    QListWidgetItem*p;

    FileName->init(setting->FileName);
    FileContent->init(setting->FileContent);
    if (setting->FileName->length())setting->fun_1 = true;
    if (setting->FileContent->length())setting->fun_2 = true;

    if (!setting->is_thread)
        normal_search();
    else
        quick_search();
    print();
}

void MainWindow::print(){
    if(now_thread){
        end_work();
        return;
    }
    end_time=clock();
    emit show_end("no problem");
    QString str;
    str=QString("在%1ms内共找到 %2个包含文件名的文件 %3个包含文件内容的文件").arg(end_time-start_time)
            .arg(thread1->cnt1+thread2->cnt1).arg(thread1->cnt2+thread2->cnt2);
    emit show_end(str);
    Merge(thread1->wjr,thread2->wjr);
    ListShow();
//    MergePrint(thread1->wjr,thread2->wjr);
    return end_work();
}

void MainWindow::add_file_list(QListWidgetItem*p){
    ui->file_list->addItem(p);
    ++show_size;
}

void MainWindow::use_thread(){
    std::thread t(&MainWindow::work,this);
    t.detach();
}

void MainWindow::my_open_file(QListWidgetItem*p){
    QDesktopServices::openUrl("file:///"+p->text());
}
void MainWindow::normal_search(){
    string s=setting->root_path->toStdString();
    thread1->getFiles(s);
}
void MainWindow::quick_search(){
    std::thread t1(&QuickSearchThread::thread_get_Files_1,thread1);
    std::thread t2(&QuickSearchThread::thread_get_Files_2,thread2);
    t1.join();
    t2.join();
}

void MainWindow::ListClear(){
    int len=show_size;
    for(int i=len-1;~i;--i)
        ui->file_list->takeItem(i);
    show_size=0;
}
void MainWindow::Merge(MyQStringList*p,MyQStringList*q){

    ListClear();

    if(List!=nullptr)delete [] List,List=nullptr;
    ListPage=1;
    p->sort(),q->sort();
    int n=p->size,m=q->size;
    int i(0),j(0);

    List=new QListWidgetItem [n+m];
    list_size=n+m;
    MaxPage=(list_size-1)/200+1;

    QListWidgetItem*head=List;

    while(i<n&&j<m){
        if(p->qwq[i]<q->qwq[j]){
            head->setIcon(icon_provider.icon(QFileInfo(p->qwq[i])));
            head->setText(p->qwq[i]);
            ++head,++i;
        }else{
            head->setIcon(icon_provider.icon(QFileInfo(q->qwq[j])));
            head->setText(q->qwq[j]);
            ++head,++j;
        }
        if(now_thread)return;
    }
    while(i<n){
        head->setIcon(icon_provider.icon(QFileInfo(p->qwq[i])));
        head->setText(p->qwq[i]);
        ++head,++i;
        if(now_thread)return ;
    }
    while(j<m){
        head->setIcon(icon_provider.icon(QFileInfo(q->qwq[j])));
        head->setText(q->qwq[j]);
        ++head,++j;
        if(now_thread)return ;
    }
}
void MainWindow::ListShow(){
    QListWidgetItem*L=List+(ListPage-1)*200,*R=List+min(list_size,ListPage*200)-1;
    while(L<=R)
        emit slow_add_file_list(L++);
    PageShow();
}

void MainWindow::PageShow(){
    ui->TurnToPage->setText(QString("%1").arg(ListPage));
    ui->MaxPage->setText(QString("/%1").arg(MaxPage));
}
