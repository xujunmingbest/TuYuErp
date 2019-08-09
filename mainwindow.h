#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QMdiArea>
#include "MysqlOperate/mysqloperate.h"

#include "FactoryContract/addfactorycontract.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QMdiArea *mdiArea;
    QMdiSubWindow *GetSubWindow(QString title);
    void DeleteSubWindow(QString title);
    void ShowFactoryContract(QString Contract_id,e_mode mode);
    void ShowZhixiangContract(QString Contract_id,e_mode mode);
    void ShowDingdanContract(QString Contract_id,e_mode mode);
    void ShowHeHuContract(QString Contract_id,e_mode mode);

    void SetWelcome(QString text);

private:
    Ui::MainWindow *ui;
    QLabel *welcome_label;

private slots:

    //工厂订单
    void ShowDingdanContractSlot();
    void ShowFindDingdanContractSlot();


    //面料合同
    void ShowAddFactoryContractSlot();
    void ShowFindFactoryContractSlot();
    //纸箱合同
    void ShowAddZhiXiangContractSlot();
    void ShowFindZhiXiangContractSlot();

    //面料
    void ShowMianLiaoChuRuKuSlot();
    void ShowMianLiaoMingXiSlot();
    //辅料
    void ShowFuLiaoChuRuKuSlot();
    void ShowFuLiaoMingXiSlot();
    //成品
    void ShowChengPinChuRuKuSlot();
    void ShowChengPinMingXiSlot();

    //显示客户合同
    void ShowKeHuContractSlot();
    void ShowFindKeHuContractSlot();
};

#endif // MAINWINDOW_H
