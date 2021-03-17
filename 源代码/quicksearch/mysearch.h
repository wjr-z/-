#ifndef MYSEARCH_H
#define MYSEARCH_H
#include <QDialog>
#include "myfile.h"
class MySearch:public QDialog{
    Q_OBJECT
public:
    MySearch();
    ~MySearch();
signals:
    void print_file_list(QString s);
};
#endif // MYSEARCH_H
