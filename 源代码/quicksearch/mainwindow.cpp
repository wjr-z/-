#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "function.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <windows.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,myout(nullptr),now_thread(0)
{
    ui->setupUi(this);

    this->setAcceptDrops(true);//设置可以拖拽

    createActions();//创建动作
    createMenus();//创建菜单

    //初始化
    setting=new _setting;
    FileName=new Sunday;
    FileContent=new Sunday;
    thread1=new QuickSearchThread(setting,FileName,FileContent,&now_thread);
    thread2=new QuickSearchThread(setting,FileName,FileContent,&now_thread);
    for(int i=0;i<3;++i)
        List[i]=nullptr;

    show_size=MaxPage=list_size=0;
    ListPage=1;
    sortmode=1;

    ui->file_list->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->file_list->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为时每次选择一行
    ui->file_list->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
    ui->file_list->horizontalHeader()
            ->setStyleSheet("QHeaderView::section{background:#fff;outline:none;"
                            "color:#413e3e;font-weight:500;font-family:Microsoft Yahei;"
                            "}"); //设置表头背景色
    //设置表头
    QStringList str;
    str<<"文件名字"<<"路径"<<"大小";
    ui->file_list->setRowCount(0);
    ui->file_list->setColumnCount(3);
    ui->file_list->setHorizontalHeaderLabels(str);
    ui->file_list->setColumnWidth(0,380);
    ui->file_list->setColumnWidth(1,220);
    ui->file_list->verticalHeader()->setHidden(true);
    ui->file_list->horizontalHeader()->setSortIndicatorShown(true);

    search=new QTimer(this);//搜索时的定时器，用于显示信息
    setdeltatime(1000);

    connect(ui->file_list,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(statusBarShow(QTableWidgetItem*)));
    connect(ui->file_list,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(my_open_file(QTableWidgetItem*)));
    connect(this,SIGNAL(ShowInfo(const QString&)),this,SLOT(show_string(const QString&)));
    connect(this,SIGNAL(slow_add_file_list(mytableitem*)),this,SLOT(add_file_list(mytableitem*)));
    connect(ui->file_list->horizontalHeader(),SIGNAL(sectionClicked(int )),this, SLOT(sortTable(int)));

    connect(search,SIGNAL(timeout()),this,SLOT(UpdateInfo()));
    connect(this,SIGNAL(StartTimerSignal(QTimer*,const int&)),this,SLOT(StartTimer(QTimer*,const int&)));
    connect(this,SIGNAL(StopTimerSignal(QTimer*)),this,SLOT(StopTimer(QTimer*)));

    connect(ui->path_input,SIGNAL(returnPressed()),ui->file_name_input,SLOT(setFocus()));
    connect(ui->file_name_input,SIGNAL(returnPressed()),ui->file_content_input,SLOT(setFocus()));
    connect(ui->file_content_input,SIGNAL(returnPressed()),ui->path_input,SLOT(setFocus()));
}

MainWindow::~MainWindow()
{
    delete ui,ui=nullptr;
    delete setting,setting=nullptr;
    delete FileName,FileName=nullptr;
    delete FileContent,FileContent=nullptr;
    delete thread1,thread1=nullptr;
    delete thread2,thread2=nullptr;
    for(int i=0;i<3;++i)
        if(List[i]!=nullptr)
            delete [] List[i],List[i]=nullptr;
}

void MainWindow::on_searchButton_clicked()
{
    QString s=ui->path_input->text();
    MainTainPath(s);
    ui->path_input->setText(s);
    use_thread();
}

void MainWindow::on_path_input_textChanged(){
    ui->searchButton->setEnabled(!ui->path_input->text().isEmpty());
}

void MainWindow::on_fileview_button_clicked()
{
    QString file_path;
    QString old=ui->path_input->text();
    if(!FileExists(ui->path_input->text()))
        file_path=QFileDialog::getExistingDirectory(this,"请选择搜索根目录...","C:");
    else file_path=QFileDialog::getExistingDirectory(this,"请选择搜索根目录...",old+"\\");
    if(file_path.length())ui->path_input->setText(file_path);
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

void MainWindow::sortTable(int p){//第p列
    ((sortmode>>1)==p)?sortmode^=1:sortmode=p<<1;
    ListClear();//然后清空当前列表
    ListShow();//然后展示即可
}

void MainWindow::on_PageSize_currentTextChanged(const QString &arg1)
{
    PageSize=arg1.toInt();
    MaxPage=(list_size-1)/PageSize+1;
    ListClear();
    ListShow();
}
void MainWindow::openSeniorSettingDialog(){
    SeniorSettingDialog=new SeniorSetting(this);
    SeniorSettingDialog->show();
}

void MainWindow::dragEnterEvent(QDragEnterEvent* e){
    e->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent* e){
    QList<QUrl> urls = e->mimeData()->urls();
    if(urls.isEmpty())
        return ;
    QString fileName = urls.first().toLocalFile();
    if(fileName.isEmpty())
        return;
    ui->path_input->setText(fileName);
}

