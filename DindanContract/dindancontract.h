#ifndef DINDANCONTRACT_H
#define DINDANCONTRACT_H

#include <QWidget>
#include "MysqlOperate/mysqloperate.h"
namespace Ui {
class DindanContract;
}

#include <QLabel>

class ClickQlabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickQlabel(QWidget *parent=0, Qt::WindowFlags f=0);
    explicit ClickQlabel(const QString &text, QWidget *parent=0, Qt::WindowFlags f=0);
protected:
    /*!
      由Qt自己调用的鼠标事件处理函数,在此发射clicked信号
    */
    virtual void mouseReleaseEvent(QMouseEvent * ev);
signals:
    void clicked(void);
};


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
