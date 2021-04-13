#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QProcess>
#include <QFileIconProvider>
#include <QTimer>
#include <QAction>
#include <QMenuBar>
#include "quickfile.h"
#include "SearchThread.h"
#include "seniorsetting.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void ShowInfo(const QString&s);//多线程内信息信号
    void slow_add_file_list(mytableitem*p);//展示列表添加一行列表 信号

    void StartTimerSignal(QTimer*w,const int&ctime);//多线程内定时器开始信号
    void StopTimerSignal(QTimer*w);//多线程内定时器结束信号
private slots:
    void on_path_input_textChanged();//路径内容不为空则可以搜索，路径内容改变更新一次
    void on_searchButton_clicked();//搜索
    void show_string(const QString&s);//在显示信息展示
    void my_open_file(QTableWidgetItem*p);//打开文件
    void on_fileview_button_clicked();//浏览搜索路径
    void add_file_list(mytableitem*p);//展示列表添加一行列表 槽
    void on_PageUp_clicked();//展示列表上一页
    void on_PageDown_clicked();//展示列表下一页
    void on_TurnTo_clicked();//展示列表跳转
    void statusBarShow(QTableWidgetItem*p);//状态栏更新
    void sortTable(int p);//更改排序方式
    void UpdateInfo();//更新当前搜索进度

    void StartTimer(QTimer*w,const int&ctime);
    void StopTimer(QTimer*w);

    void on_PageSize_currentTextChanged(const QString &arg1);//一页展示数目
    void openSeniorSettingDialog();

private:
    QFileIconProvider icon_provider;
    Ui::MainWindow *ui;

    QuickSearchThread*thread1,*thread2;
    _setting*setting;//通用设置
    Sunday*FileName,*FileContent;//通用搜索关键字

    QMenu *MenuSetting;
    QAction *SeniorSettingAction;
    SeniorSetting*SeniorSettingDialog;


    FILE*myout;
    int now_thread;
    int start_time,end_time;
    mutex mutex_number;

    int PageSize=100;
    mytableitem*(List[3]);//存找到的所有文件、文件夹，每种模式都存起来
    int sortmode;//排序模式
    int list_size,ListPage;//个数和当前页数
    int show_size,MaxPage;//展示出的个数

    QTimer*search;//每秒显示当前搜索信息
    int deltatime,UsedTime;//定时器间隔和用时

    void setdeltatime(int p){deltatime=p;}//设置定时器时间

    void work();
    void print();
    void end_work();
    void use_thread();
    void searchinit();//搜索初始化
    void quick_search();
    void normal_search();
    void Merge(MyQStringList*p,MyQStringList*q);
    inline int antirow(int row){return list_size-row-1;}//反向排序
    inline QString getPath(QTableWidgetItem*p){
        return (sortmode&1)?*List[sortmode>>1][antirow((ListPage-1)*PageSize+p->row())].Path:*List[sortmode>>1][(ListPage-1)*PageSize+p->row()].Path;
    }

    void ListSort(mytableitem*head);
    void ListShow();
    void ListClear();
    void PageShow();

    inline void createActions(){
        SeniorSettingAction=new QAction(QIcon("open.png"),tr("高级设置"),this);
        connect(SeniorSettingAction,SIGNAL(triggered()),this,SLOT(openSeniorSettingDialog()));
    }
    inline void createMenus(){
        MenuSetting=menuBar()->addMenu(tr("设置"));
        MenuSetting->addAction(SeniorSettingAction);
    }

protected:
    void dragEnterEvent(QDragEnterEvent* e);	//声明
    void dropEvent(QDropEvent* e);				//声明
};
#endif // MAINWINDOW_H
