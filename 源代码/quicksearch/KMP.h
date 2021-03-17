#ifndef KMP_H
#define KMP_H

#include <QString>
class KMP{
private:
    int *nxt;
    QString*s;
    int length;
    bool*is_case_sensitive;
public:
    KMP(bool*is=nullptr);
    ~KMP();
    void get_nxt();
    bool find(std::string&g, const int&n);
    void clear();
    void init(QString*p);
};

#endif // KMP_H
