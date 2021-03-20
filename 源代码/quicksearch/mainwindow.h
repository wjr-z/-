#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QProcess>
#include <QFileIconProvider>
#include "myfile.h"
#include "SearchThread.h"

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
    void show_end(const QString&s);
    void slow_add_file_list(QListWidgetItem*p);
private slots:
    void on_path_input_textChanged();
    void on_searchButton_clicked();
    void show_string(const QString&s);
    void my_open_file(QListWidgetItem*p);
    void on_fileview_button_clicked();
    void add_file_list(QListWidgetItem*p);

    void on_PageUp_clicked();

    void on_PageDown_clicked();

    void on_TurnTo_clicked();


private:
    QFileIconProvider icon_provider;
    Ui::MainWindow *ui;

    QuickSearchThread*thread1,*thread2;
    _setting*setting;//通用设置
    KMP*FileName,*FileContent;//通用搜索关键字

    FILE*myout;
    int now_thread,res_thread;
    int start_time,end_time;
    mutex mutex_number;

    const int PageSize=100;
    QListWidgetItem*List;//存找到的所有文件、文件夹
    int list_size,ListPage;//个数和当前页数
    int show_size,MaxPage;//展示出的个数

    void init();
    void work();
    void print();
    void end_work();
    void use_thread();
    void normal_search();
    void quick_search();
    void Merge(MyQStringList*p,MyQStringList*q);
    void ListShow();
    void ListClear();
    void PageShow();

};
#endif // MAINWINDOW_H
