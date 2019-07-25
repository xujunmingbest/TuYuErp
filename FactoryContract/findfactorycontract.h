#ifndef FINDFACTORYCONTRACT_H
#define FINDFACTORYCONTRACT_H

#include <QWidget>
#include "MysqlOperate/mysqloperate.h"
namespace Ui {
class FindFactoryContract;
}

class FindFactoryContract : public QWidget
{
    Q_OBJECT

public:
    explicit FindFactoryContract(QWidget *parent = 0);
    ~FindFactoryContract();

private:
    Ui::FindFactoryContract *ui;
    QMap<QString,int> ContractTableIndex;
    int page;
private slots:
    void ShaiXuan();
    void ContractDetailSlot();
    void TiaoZhuang();
    void leftSlot();
    void rightSlot();
    void PageSlot();
};

#endif // FINDFACTORYCONTRACT_H
