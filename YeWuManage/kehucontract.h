#ifndef KEHUCONTRACT_H
#define KEHUCONTRACT_H

#include <QWidget>
#include "MysqlOperate/mysqloperate.h"
namespace Ui {
class KeHuContract;
}

class KeHuContract : public QWidget
{
    Q_OBJECT

public:
    explicit KeHuContract(QWidget *parent = 0);
    ~KeHuContract();
    void SetMode(e_mode mode);
    bool LoadData(QString ContractId);
private:
    Ui::KeHuContract *ui;

private:
    QMap<QString,int> ProductTableIndex;
    MysqlOperate * m_MysqlOperate;
    e_mode m_mode;
    void AddContract();
    void EditContract();

    QString version;
private slots:
    void ButtonSlot();
    void HeJiShuLiang();
    void HeJiJine();

    void DeleteProduct();
    void AddLine();
    void LoadImage();
};

#endif // KEHUCONTRACT_H
