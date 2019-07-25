#include "findmianliao.h"
#include "ui_findmianliao.h"

FindMianLiao::FindMianLiao(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindMianLiao)
{
    ui->setupUi(this);
}

FindMianLiao::~FindMianLiao()
{
    delete ui;
}
