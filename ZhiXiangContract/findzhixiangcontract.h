#ifndef FINDZHIXIANGCONTRACT_H
#define FINDZHIXIANGCONTRACT_H

#include <QWidget>
#include "MysqlOperate/mysqloperate.h"
namespace Ui {
class FindZhiXiangContract;
}

class FindZhiXiangContract : public QWidget
{
    Q_OBJECT

public:
    explicit FindZhiXiangContract(QWidget *parent = 0);
    ~FindZhiXiangContract();

private:
    Ui::FindZhiXiangContract *ui;
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

#endif // FINDZHIXIANGCONTRACT_H
