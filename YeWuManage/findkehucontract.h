#ifndef FINDKEHUCONTRACT_H
#define FINDKEHUCONTRACT_H

#include <QWidget>
#include "MysqlOperate/mysqloperate.h"
namespace Ui {
class FindKeHuContract;
}

class FindKeHuContract : public QWidget
{
    Q_OBJECT

public:
    explicit FindKeHuContract(QWidget *parent = 0);
    ~FindKeHuContract();

private:
    Ui::FindKeHuContract *ui;

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

#endif // FINDKEHUCONTRACT_H
