#ifndef DINDANCONTRACT_H
#define DINDANCONTRACT_H

#include <QWidget>
#include "MysqlOperate/mysqloperate.h"
#include "global.h"
namespace Ui {
class DindanContract;
}


class DindanContract : public QWidget
{
    Q_OBJECT
//imageurl 放到 ClickQlabel的 properity url这里
public:
    explicit DindanContract(QWidget *parent = 0);
    ~DindanContract();
    void SetMode(e_mode mode);
    bool LoadData(QString contract_id);
private:
    QMap<QString,int> TableIndex_1;
    QMap<QString,int> TableIndex_2;
    QMap<QString,int> TableIndex_3;
    Ui::DindanContract *ui;
    e_mode m_mode;
    //QMap<int , QString> imageUrls_1;
    //QMap<int , QString> imageUrls_3;
private:
    //处理表1
    bool add_table_1();
    bool add_table_2();
    bool add_table_3();
    bool add_contract();


    void AddContract();
    void EditContract();
    QString version;
    MysqlOperate * m_MysqlOperate;
private slots:
    void AddLine_1();
    void AddLine_2();
    void AddLine_3();
    void Loadimage_1();
    void ButtonSlot();
    void DeleteProduct_1();
    void DeleteProduct_2();
    void DeleteProduct_3();
};

#endif // DINDANCONTRACT_H
