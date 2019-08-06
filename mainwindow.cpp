#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "global.h"
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mdiArea = new QMdiArea(this);
    this->setCentralWidget(mdiArea);

    {//添加Statusbar

         welcome_label = new QLabel(SS("您好:"));
         QToolBar *toolBar = ui->toolBar;
         toolBar->addWidget(welcome_label);
         toolBar->setStyleSheet("background-color:rgb(204,204,204);color:red");
    }
    //注册菜单
    {//系统管理
        QMenu* Q =  ui->menu;
        {
            QAction* action = Q->addAction(SS("添加面料合同"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowAddFactoryContractSlot()));
        }
        {
            QAction* action = Q->addAction(SS("查看面料合同汇总"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowFindFactoryContractSlot()));
        }
        {
            QAction* action = Q->addAction(SS("添加纸箱合同"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowAddZhiXiangContractSlot()));
        }
        {
            QAction* action = Q->addAction(SS("查看纸箱合同汇总"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowFindZhiXiangContractSlot()));
        }
    }
    {//系统管理
        QMenu* Q =  ui->menu_cangku;
        {
            QAction* action = Q->addAction(SS("面料出入库"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowMianLiaoChuRuKuSlot()));
        }
        {
            QAction* action = Q->addAction(SS("面料明细"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowMianLiaoMingXiSlot()));
        }

        {
            QAction* action = Q->addAction(SS("辅料出入库"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowFuLiaoChuRuKuSlot()));
        }
        {
            QAction* action = Q->addAction(SS("辅料明细"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowFuLiaoMingXiSlot()));
        }
        {
            QAction* action = Q->addAction(SS("成品出入库"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowChengPinChuRuKuSlot()));
        }
        {
            QAction* action = Q->addAction(SS("成品明细"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowChengPinMingXiSlot()));
        }
    }
    {//订单管理
        QMenu* Q =  ui->menu_dingdan;
        {
            QAction* action = Q->addAction(SS("添加订单合同"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowDingdanContractSlot()));
        }
        {
            QAction* action = Q->addAction(SS("查看订单合同汇总"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowFindDingdanContractSlot()));
        }
    }
    {//订单管理
        QMenu* Q =  ui->menu_yewu;
        {
            QAction* action = Q->addAction(SS("添加客户合同"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowKeHuContractSlot()));
        }
        {
            QAction* action = Q->addAction(SS("查看客户合同汇总"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowFindKeHuContractSlot()));
        }
    }





}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::SetWelcome(QString text){
    welcome_label->setText(text);
}

QMdiSubWindow * MainWindow::GetSubWindow(QString title){

    foreach (QMdiSubWindow *window, mdiArea->subWindowList())
    {
        if ( window->windowTitle() == title)
        {   //如果已经打开过，则获得焦点
            window->showMaximized();
            return window;
        }
    }
    return Q_NULLPTR;
}
#include "FactoryContract/addfactorycontract.h"

void MainWindow::ShowAddFactoryContractSlot(){
    QString Title = SS("添加工厂合同");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        AddFactoryContract *c1 = new AddFactoryContract;
        c1->SetMode(e_mode::ADD);
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }

}
#include "FactoryContract/findfactorycontract.h"
void MainWindow::ShowFindFactoryContractSlot(){
    QString Title = SS("查看工厂合同汇总");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        FindFactoryContract *c1 = new FindFactoryContract;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }

}
#include "DindanContract/finddingdancontract.h"
 void MainWindow::ShowFindDingdanContractSlot(){
     QString Title = SS("查看订单合同汇总");
     QMdiSubWindow *wnd =GetSubWindow(Title);
     if( wnd == Q_NULLPTR){
         //mdiarea添加窗体
         FindDingdanContract *c1 = new FindDingdanContract;
         mdiArea->addSubWindow(c1);
         c1->setWindowState(Qt::WindowMaximized);
         c1->setWindowTitle(Title);
     }

 }

#include "ZhiXiangContract/zhixiangcontract.h"
void MainWindow::ShowAddZhiXiangContractSlot(){
    QString Title = SS("添加纸箱合同");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        ZhiXiangContract *c1 = new ZhiXiangContract;
        c1->SetMode(e_mode::ADD);
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }
}

#include "ZhiXiangContract/findzhixiangcontract.h"
void MainWindow::ShowFindZhiXiangContractSlot(){
    QString Title = SS("查看纸箱合同汇总");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        FindZhiXiangContract *c1 = new FindZhiXiangContract;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }
}


#include "FactoryContract/lookfactorycontract.h"
void MainWindow::ShowFactoryContract(QString Contract_id,e_mode mode){
    QString Title = SS("添加工厂合同");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        AddFactoryContract *c1 = new AddFactoryContract;
        if( !c1->LoadData(Contract_id) ){
              QMessageBox::about(NULL, SS("错误提示"), SS("数据加载失败"));
              delete c1;
              return ;
        }
        c1->SetMode(mode);
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }else{
        AddFactoryContract *c1 = ((AddFactoryContract *)wnd->widget());
        if( !c1->LoadData(Contract_id) ){
              QMessageBox::about(NULL, SS("错误提示"), SS("数据加载失败"));
              return ;
        }
        c1->SetMode(mode);
    }
}

void MainWindow::ShowZhixiangContract(QString Contract_id,e_mode mode){
    QString Title = SS("添加纸箱合同");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        ZhiXiangContract *c1 = new ZhiXiangContract;
        if( !c1->LoadData(Contract_id) ){
              QMessageBox::about(NULL, SS("错误提示"), SS("数据加载失败"));
              delete c1;
              return ;
        }
        c1->SetMode(mode);
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }else{
        ZhiXiangContract *c1 = ((ZhiXiangContract *)wnd->widget());
        if( !c1->LoadData(Contract_id) ){
              QMessageBox::about(NULL, SS("错误提示"), SS("数据加载失败"));
              return ;
        }
        c1->SetMode(mode);
    }

}
#include "DindanContract/dindancontract.h"

void MainWindow::ShowDingdanContract(QString Contract_id,e_mode mode){
    QString Title = SS("添加订单合同");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        DindanContract *c1 = new DindanContract;
        if( !c1->LoadData(Contract_id) ){
              QMessageBox::about(NULL, SS("错误提示"), SS("数据加载失败"));
              delete c1;
              return ;
        }
        c1->SetMode(mode);
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }else{
        DindanContract *c1 = ((DindanContract *)wnd->widget());
        if( !c1->LoadData(Contract_id) ){
              QMessageBox::about(NULL, SS("错误提示"), SS("数据加载失败"));
              return ;
        }
        c1->SetMode(mode);
    }


}


#include "Mianliao/miaoliaoruku.h"

void MainWindow::ShowMianLiaoChuRuKuSlot(){
    QString Title = SS("面料出入库");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        MiaoliaoRuku *c1 = new MiaoliaoRuku;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }
}
#include "Mianliao/findmianliao.h"
void MainWindow::ShowMianLiaoMingXiSlot(){
    QString Title = SS("面料明细");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        FindMianLiao *c1 = new FindMianLiao;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }

}


#include "FuLiao/fuliaochuruku.h"
void MainWindow::ShowFuLiaoChuRuKuSlot(){
    QString Title = SS("辅料出入库");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        FuLiaoChuRuKu *c1 = new FuLiaoChuRuKu;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }

}

#include "FuLiao/findfuliao.h"
void MainWindow::ShowFuLiaoMingXiSlot(){
    QString Title = SS("辅料明细");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        findfuliao *c1 = new findfuliao;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }


}


#include "ChengPin/chengpinchuruku.h"
void MainWindow::ShowChengPinChuRuKuSlot(){
    QString Title = SS("成品出入库");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        ChengPinChuRuKu *c1 = new ChengPinChuRuKu;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }
}

#include "ChengPin/findchengpin.h"
void MainWindow::ShowChengPinMingXiSlot(){
    QString Title = SS("成品明细");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        findchengpin *c1 = new findchengpin;
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }

}
#include "DindanContract/dindancontract.h"


void MainWindow::ShowDingdanContractSlot(){
    QString Title = SS("添加订单合同");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        DindanContract *c1 = new DindanContract;
        c1->SetMode(e_mode::ADD);
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }
}


#include "YeWuManage/kehucontract.h"
//显示客户合同
void MainWindow::ShowKeHuContractSlot(){
    QString Title = SS("客户合同");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        KeHuContract *c1 = new KeHuContract;
        c1->SetMode(e_mode::ADD);
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }

}

