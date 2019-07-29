#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mdiArea = new QMdiArea(this);
    this->setCentralWidget(mdiArea);
    //注册菜单
    {//系统管理
        QMenu* Q =  ui->menu;
        {
            QAction* action = Q->addAction(SS("添加工厂合同"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowAddFactoryContractSlot()));
        }
        {
            QAction* action = Q->addAction(SS("查看工厂合同汇总"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowFindFactoryContractSlot()));
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
            QAction* action = Q->addAction(SS("成品结余统计"));
            connect(action, SIGNAL(triggered()), this, SLOT(ShowChengPinJieYuSlot()));
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
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

#include "FactoryContract/lookfactorycontract.h"
void MainWindow::ShowLookFactoryContract(QString Contract_id){
    QString Title = SS("工厂合同明细");
    QMdiSubWindow *wnd =GetSubWindow(Title);
    if( wnd == Q_NULLPTR){
        //mdiarea添加窗体
        LookFactoryContract *c1 = new LookFactoryContract;
        c1->LoadData(Contract_id);
        mdiArea->addSubWindow(c1);
        c1->setWindowState(Qt::WindowMaximized);
        c1->setWindowTitle(Title);
    }else{
        ((LookFactoryContract *)wnd->widget())->LoadData(Contract_id);
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
void MainWindow::ShowChengPinJieYuSlot(){


}



