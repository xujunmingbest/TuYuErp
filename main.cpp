#include "mainwindow.h"
#include <QApplication>
#include "MysqlTableConfig/MysqlTableConfig.h"
#include "MysqlOperate/mysqloperate.h"
#include "login.h"
#include "global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MysqlTableConfig::getInstance();
    MainWindow w;
    MysqlOperate::getInstance();

    //w.showMaximized();
    Login lo;
    lo.exec();
    if( lo.get_login_success()){
         w.SetWelcome(SS("ฤ๚บร:") + CUser::getInstance()->get_name());
         w.showMaximized();
    }else{
        return -1;
    }

    return a.exec();
}
