#include "mianliaojieyu.h"
#include "ui_mianliaojieyu.h"

MianLiaoJieYU::MianLiaoJieYU(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MianLiaoJieYU)
{
    ui->setupUi(this);
}

MianLiaoJieYU::~MianLiaoJieYU()
{
    delete ui;
}
