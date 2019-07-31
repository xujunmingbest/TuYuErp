#ifndef ADDFACTORYCONTRACT_H
#define ADDFACTORYCONTRACT_H

#include <QWidget>
#include <QMap>
#include "MysqlOperate/mysqloperate.h"


namespace Ui {
class AddFactoryContract;
}


class AddFactoryContract : public QWidget
{
    Q_OBJECT

public:
    explicit AddFactoryContract(QWidget *parent = 0);
    ~AddFactoryContract();
    void SetMode(e_mode mode);
    bool LoadData(QString ContractId);
private:
    Ui::AddFactoryContract *ui;
    QMap<QString,int> ProductTableIndex;
    MysqlOperate * m_MysqlOperate;
    e_mode m_mode;
    void AddContract();
    void EditContract();

    QString version;
private slots:
    //void TianJiaProduct();
    void ButtonSlot();


    void DeleteProduct();
    void AddLine();
};

#endif // ADDFACTORYCONTRACT_H
