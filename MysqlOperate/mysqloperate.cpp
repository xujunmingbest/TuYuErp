#include "mysqloperate.h"
#include "QMessageBox.h"
#include <QCryptographicHash>
MysqlOperate *MysqlOperate::local_instance = nullptr;



MysqlOperate::MysqlOperate()
{
}


MysqlOperate *MysqlOperate::getInstance(){
    if (local_instance == nullptr)
    {
        local_instance = new MysqlOperate();
        local_instance->MysqlOpen();
    }
    return local_instance;

}

bool MysqlOperate::MysqlOpen(){
    defaultDB = QSqlDatabase::addDatabase("QMYSQL");
    {
        defaultDB.setDatabaseName( "tuyuerp" );
        defaultDB.setUserName( "root" );
        defaultDB.setPassword( "123456" );
        defaultDB.setHostName( "47.94.219.135" );

        if ( defaultDB.open() ) {
            defaultDB.exec("SET NAMES 'gbk'");
            return true;
        }else{
              QMessageBox::about(NULL, QString::fromLocal8Bit("错误提示"), defaultDB.lastError().databaseText());
              return false;
        }
    }
}


//bool MysqlOperate::UserLogin(QString name,QString password){
//    QByteArray passwordmd5;
//    passwordmd5 = QCryptographicHash::hash(password.toLatin1(),QCryptographicHash::Md5);
//
//    QString sq=QStringLiteral("select * from user where username = '%1' and password = '%2'").arg(name,QString(passwordmd5.toHex()));
//    qDebug() << sq;
//    QSqlQuery query(sq);
//    if( query.isActive()){
//        while(query.next()){
//            g_Auth = query.value(1).toString();
//            g_ChargeName = query.value(2).toString();
//            g_Name = query.value(3).toString();
//            qDebug() << g_Auth;
//            qDebug() << g_ChargeName;
//            qDebug() << g_Name;
//            return true;
//        }
//    }
//    return false;
//}
//
//#include <QJsonObject>
//#include <QJsonDocument>
//QJsonObject MysqlOperate::GetAuth(){
//    QJsonObject jo;
//            QString AuthType;
//
//    {
//        QString sq=QStringLiteral("select module from user_authority where role = '%1'").arg(g_Auth);
//        qDebug() << sq;
//        QSqlQuery query(sq);
//        if( query.isActive()){
//            while(query.next()){
//               QString jsonString = query.value(0).toString();
//
//                QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toStdString().data());
//                if( jsonDocument.isNull() ){
//                    qDebug()<< "===> please check the string "<< jsonString;
//                }
//                jo = jsonDocument.object();
//               return jo;
//
//            }
//        }else{
//            QMessageBox::about(NULL, QString::fromLocal8Bit("错误提示"),QString::fromLocal8Bit("权限获取失败"));
//            return jo;
//        }
//    }
//
//}

