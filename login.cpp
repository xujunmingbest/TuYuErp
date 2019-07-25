#include "login.h"
#include "ui_login.h"
#include "MysqlOperate/mysqloperate.h"
#include <QString>
#include "global.h"
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setFixedSize(559,393);
    is_login_success = false;

}

Login::~Login()
{
    delete ui;
}


void Login::LoginSlot(){
    QString Name = ui->lineEdit_name->text().trimmed();
    QString password = ui->lineEdit_password->text().trimmed();
    MysqlOperate * MysqlInstance = MysqlOperate::getInstance();
    QMap<QString,QString> conditions={{"name",Name},{"password",password}};
    QVector<QMap<QString,QString>> data;
    MysqlInstance->Find("user",&conditions,nullptr,data);
    if( data.size() == 0) {
        is_login_success = false;
         QMessageBox::about(NULL, SS("������ʾ"), SS("�û������������"));
        return ;
    }
    is_login_success = true;

    CUser *UserInstance =  CUser::getInstance();
    UserInstance->set_name(data[0].value("name"));
    UserInstance->set_role(data[0].value("role"));
    UserInstance->LoadAuth();
    this->close();
}