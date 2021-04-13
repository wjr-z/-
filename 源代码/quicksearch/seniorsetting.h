#ifndef SENIORSETTING_H
#define SENIORSETTING_H

#include <QDialog>

namespace Ui {
class SeniorSetting;
}

class SeniorSetting : public QDialog
{
    Q_OBJECT

public:
    explicit SeniorSetting(QWidget *parent = nullptr);
    ~SeniorSetting();

private:
    Ui::SeniorSetting *ui;
};

#endif // SENIORSETTING_H
