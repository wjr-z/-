#include "seniorsetting.h"
#include "ui_seniorsetting.h"

SeniorSetting::SeniorSetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SeniorSetting)
{
    ui->setupUi(this);
}

SeniorSetting::~SeniorSetting()
{
    delete ui;
}
