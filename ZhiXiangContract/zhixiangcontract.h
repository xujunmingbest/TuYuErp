#ifndef ZHIXIANGCONTRACT_H
#define ZHIXIANGCONTRACT_H

#include <QWidget>
#include "MysqlOperate/mysqloperate.h"
namespace Ui {
class ZhiXiangContract;
}

class ZhiXiangContract : public QWidget
{
    Q_OBJECT

public:
    explicit ZhiXiangContract(QWidget *parent = 0);
    ~ZhiXiangContract();
    void SetMode(e_mode mode);
    bool LoadData(QString ContractId);
private:
    Ui::ZhiXiangContract *ui;

private:
    QMap<QString,int> ProductTableIndex;
    MysqlOperate * m_MysqlOperate;
    e_mode m_mode;
    void AddContract();
    void EditContract();

    QString version;
private slots:
    void ButtonSlot();


    void DeleteProduct();
    void AddLine();
};

#endif // ZHIXIANGCONTRACT_H
