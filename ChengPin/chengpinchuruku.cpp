#include "chengpinchuruku.h"
#include "ui_chengpinchuruku.h"

ChengPinChuRuKu::ChengPinChuRuKu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChengPinChuRuKu)
{
    ui->setupUi(this);
}

ChengPinChuRuKu::~ChengPinChuRuKu()
{
    delete ui;
}
