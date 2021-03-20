#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "function.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,myout(nullptr),now_thread(0),res_thread(0)
{
    ui->setupUi(this);



    setting=new _setting;
    FileName=new KMP(&setting->is_case_sensitive);
    FileContent=new KMP(&setting->is_case_sensitive);
    thread1=new QuickSearchThread(setting,FileName,FileContent,&res_thread,&now_thread);
    thread2=new QuickSearchThread(setting,FileName,FileContent,&res_thread,&now_thread);
    List=nullptr;

    show_size=MaxPage=ListPage=list_size=0;

    connect(ui->file_list,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(my_open_file(QListWidgetItem*)));
    connect(this,SIGNAL(show_end(const QString&)),this,SLOT(show_string(const QString&)));
    connect(this,SIGNAL(slow_add_file_list(QListWidgetItem*)),this,SLOT(add_file_list(QListWidgetItem*)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete setting;
    delete FileName;
    delete FileContent;
    delete thread1;
    delete thread2;
    if(List!=nullptr)delete [] List;
}

void MainWindow::on_searchButton_clicked()
{
    use_thread();
}

void MainWindow::on_path_input_textChanged(){
    ui->searchButton->setEnabled(!ui->path_input->text().isEmpty());
}

void MainWindow::on_fileview_button_clicked()
{
    QString file_path(QFileDialog::getExistingDirectory(this,"请选择搜索根目录...","C:"));
    ui->path_input->setText(file_path);
}

void MainWindow::on_PageUp_clicked()
{
    if(ListPage==1||!list_size)return;
    ListClear();
    --ListPage;
    ListShow();
}

void MainWindow::on_PageDown_clicked()
{
    if(ListPage>=MaxPage||!list_size)return;
    ListClear();
    ++ListPage;
    ListShow();
}

void MainWindow::on_TurnTo_clicked()
{
    if(!list_size)return;
    ListClear();
    ListPage=ui->TurnToPage->text().toInt();
    if(ListPage<=0)ListPage=1;
    if(ListPage>MaxPage)ListPage=MaxPage;
    ListShow();
}

