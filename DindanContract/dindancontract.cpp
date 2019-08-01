#include "dindancontract.h"
#include "ui_dindancontract.h"

DindanContract::DindanContract(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DindanContract)
{
    ui->setupUi(this);
}

DindanContract::~DindanContract()
{
    delete ui;
}
