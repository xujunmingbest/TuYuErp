#ifndef FINDDINGDANCONTRACT_H
#define FINDDINGDANCONTRACT_H

#include <QWidget>
#include "MysqlOperate/mysqloperate.h"


namespace Ui {
class FindDingdanContract;
}

class FindDingdanContract : public QWidget
{
    Q_OBJECT

public:
    explicit FindDingdanContract(QWidget *parent = 0);
    ~FindDingdanContract();

private:
    Ui::FindDingdanContract *ui;

    MysqlOperate* m_MysqlOperate;
    QMap<QString,int> ContractTableIndex;
    void TiaoZhuang(int page);
private slots:
    void ShaiXuan();
    void ContractDetailSlot();
    void ContractEditSlot();
    void ContractDeleteSlot();
    void PageSlot();
};

#endif // FINDDINGDANCONTRACT_H
