#include "mainwindow.h"
#include <QApplication>
#include "MysqlTableConfig/MysqlTableConfig.h"
#include "MysqlOperate/mysqloperate.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MysqlTableConfig::getInstance();
    QMap<QString,QString> userData;
    QString tableName = "user";
    userData.insert("id","2");
    userData.insert("role",SS("总经理"));
    userData.insert("name",SS("张三"));
    userData.insert("password",SS("123456"));
    MysqlOperate::getInstance()->Insert(tableName,userData);
    QMap<QString,QString> ref = {{"name",SS("张三")}};
    QVector<QMap<QString,QString>> datauser;
    MysqlOperate::getInstance()->Find(tableName,ref,datauser);
    //MysqlOperate::getInstance()->Delete(tableName,ref);
    QMap<QString,QString> updateData= {{"password",SS("aaaa")}};
    MysqlOperate::getInstance()->Update(tableName,ref,updateData);
    MainWindow w;
    w.show();

    return a.exec();
}
