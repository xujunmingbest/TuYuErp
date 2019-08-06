#include "findkehucontract.h"
#include "ui_findkehucontract.h"

FindKeHuContract::FindKeHuContract(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindKeHuContract)
{
    ui->setupUi(this);
}

FindKeHuContract::~FindKeHuContract()
{
    delete ui;
}
