#ifndef MYLIST_H
#define MYLIST_H
#include <QString>
struct MyQStringListItem{
    MyQStringListItem*nxt;
    QString*s;
    MyQStringListItem(const QString&path);
    ~MyQStringListItem();
};

struct MyQStringList{
    MyQStringListItem*head,*tail;
    QString*qwq;
    int size;
    MyQStringList();
    ~MyQStringList();
    void push_back(const QString&path);
    void clear();
    void sort();
};

#endif // MYLIST_H
