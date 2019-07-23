#include "mysqloperate.h"
#include "QMessageBox.h"
#include <QCryptographicHash>
#include "MysqlTableConfig/MysqlTableConfig.h"
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

bool MysqlOperate::Insert(QString tableName,QMap<QString,QString> &data){
    if( !MysqlTableConfig::getInstance()->is_legal_table(tableName)) return false;

    QString sq1 = "insert into ";
    sq1 += tableName + " (";
    QString sq2 = " values( ";
    QMap<QString, QString>::const_iterator it;
    for (it = data.constBegin(); it != data.constEnd(); ++it) {
        sq1 += it.key() + ",";
        sq2 += "'" + it.value() + "'," ;
    }
    sq1[sq1.length()-1] = ')';
    sq2[sq2.length()-1] = ')';
    QString sq = sq1 + sq2;

    qDebug() << sq;
    QSqlQuery query;
    query.exec(sq);
    if( query.isActive()){
        QMessageBox::about(NULL, SS("提示"), SS("添加数据成功"));
        return true;
    }else{
        QMessageBox::about(NULL, SS("错误提示"), query.lastError().databaseText());
        QMessageBox::about(NULL, SS("错误提示"), SS("添加数据失败")+tableName);
        return false;
    }
}

bool MysqlOperate::Find(QString tableName,QMap<QString,QString> &refer,QVector<QMap<QString,QString>> &data){
    data.clear();
    if( !MysqlTableConfig::getInstance()->is_legal_table(tableName)) return false;
    QSet<QString> columns =  MysqlTableConfig::getInstance()->get_table_columns(tableName);
    QString sq1 = "select ";
    QSet<QString>::const_iterator itset;
    for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
        sq1 += *itset + ",";
    }
    sq1[sq1.length()-1] = ' ';
    sq1 += "from " + tableName ;
    if(refer.size() > 0){
        sq1 += " where ";
        QMap<QString, QString>::const_iterator it;
        for (it = refer.constBegin(); it != refer.constEnd(); ++it) {
            sq1 += it.key() + "= '" + it.value() + "' and ";
        }
        sq1 = sq1.left(sq1.length()-5);
    }

    qDebug() << sq1;
    QSqlQuery query(sq1);
    if( query.isActive()){
        while(query.next()){
            QMap<QString,QString> maptemp;
            int i=0;
            for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset,i++) {
               maptemp.insert(*itset,query.value(i).toString());
            }
            data.push_back(maptemp);
        }
        qDebug() << data;
        return true;
    }
    return false;
}


bool MysqlOperate::Delete(QString tableName,QMap<QString,QString> &refer){
   if( !MysqlTableConfig::getInstance()->is_legal_table(tableName)) return false;
   QString sq = "delete from ";
   sq += tableName;
   if(refer.size() > 0){
        sq += " where ";
        QMap<QString, QString>::const_iterator it;
        for (it = refer.constBegin(); it != refer.constEnd(); ++it) {
            sq += it.key() + "= '" + it.value() + "' and ";
        }
        sq = sq.left(sq.length()-5);
   }
   QSqlQuery query;
   query.exec(sq);
   qDebug() << sq;
   if( query.isActive()){
       QMessageBox::about(NULL, SS("提示"), SS("删除数据成功"));
       return true;
   }else{
       QMessageBox::about(NULL, SS("错误提示"), query.lastError().databaseText());
       QMessageBox::about(NULL, SS("错误提示"), SS("删除数据失败:")+tableName);
       return false;
   }
}
bool MysqlOperate::Update(QString tableName,QMap<QString,QString> &refer ,QMap<QString,QString> &data){
    if( !MysqlTableConfig::getInstance()->is_legal_table(tableName)) return false;
    if(data.size() == 0 ) return false;
    QString sq = "update ";
    sq += tableName + " set ";
    QMap<QString, QString>::const_iterator it;
    for (it = data.constBegin(); it != data.constEnd(); ++it) {
        sq += it.key() + "= '" + it.value() + "',";
    }
    sq[sq.length()-1] = ' ';


    if(refer.size() > 0){
         sq += " where ";
         QMap<QString, QString>::const_iterator it;
         for (it = refer.constBegin(); it != refer.constEnd(); ++it) {
             sq += it.key() + "= '" + it.value() + "' and ";
         }
         sq = sq.left(sq.length()-5);
    }

    QSqlQuery query;
    query.exec(sq);
    qDebug() << sq;
    if( query.isActive()){
        QMessageBox::about(NULL, SS("提示"), SS("修改数据成功"));
        return true;
    }else{
        QMessageBox::about(NULL, SS("错误提示"), query.lastError().databaseText());
        QMessageBox::about(NULL, SS("错误提示"), SS("修改数据失败:")+tableName);
        return false;
    }
   return true;
}






bool MysqlOperate::find_user(QString name,QString password){

    QString sq=QStringLiteral("select * from user where username = '%1' and password = '%2'").arg(name).arg(password);
    qDebug() << sq;
    QSqlQuery query(sq);
    if( query.isActive()){
        while(query.next()){
            return true;
        }
    }
    return false;

}
#include <QJsonObject>

bool MysqlOperate::find_auth(QString role,QJsonObject &jo){
    QString sq=QStringLiteral("select auth from auth where role = '%1'").arg(role);
    qDebug() << sq;
    QSqlQuery query(sq);
    if( query.isActive()){
        while(query.next()){
            jo = query.value(0).toJsonObject();
            return true;
        }
    }
    return false;

}


bool MysqlOperate::MysqlOperate::add_factory_contract(S_factory_contract &data){
    QString sq=QStringLiteral("insert into factory_contract ("
                              "contract_address,contract_id,jia_account,jia_address,"
                              "jia_agency,jia_bank,jia_bank,jia_faren,"
                              "jia_fax,jia_name,jia_telephone,jiesuan_daxie,"
                              "jiesuan_xiaoxie,yi_account,yi_address,yi_agency,"
                              "yi_bank,yi_date,yi_faren,yi_fax,"
                              "yi_name,yi_telephone ) values("
                              "'%1','%2','%3','%4','%5','%6','%7','%8','%9','%10',"
                              "'%11','%12','%13','%14','%15','%16','%17','%18','%19','%20',"
                              "'%21','%22')")
            .arg(data.contract_address).arg(data.contract_id).arg(data.jia_account).arg(data.jia_address)
            .arg(data.jia_agency).arg(data.jia_bank).arg(data.jia_date).arg(data.jia_faren)
            .arg(data.jia_fax).arg(data.jia_name).arg(data.jia_telephone).arg(data.jiesuan_daxie)
            .arg(data.jiesuan_xiaoxie).arg(data.yi_account).arg(data.yi_address).arg(data.yi_agency)
            .arg(data.yi_bank).arg(data.yi_date).arg(data.yi_faren).arg(data.yi_fax)
            .arg(data.yi_name).arg(data.yi_telephone);
    qDebug() << sq;
    QSqlQuery query;
    query.exec(sq);
    if( query.isActive()){
        QMessageBox::about(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("添加流水账成功"));
        return true;
    }else{
        QMessageBox::about(NULL, QString::fromLocal8Bit("错误提示"), query.lastError().databaseText());
        QMessageBox::about(NULL, QString::fromLocal8Bit("错误提示"), QString::fromLocal8Bit("添加流水账失败"));
        return false;
    }

}
bool MysqlOperate::MysqlOperate::add_factory_product(S_factory_product &data){
    QString sq=QStringLiteral("insert into factory_product ("
                              "color,comment,contract_id,danjia,"
                              "danwei,jine,maogao,menfu,"
                              "name,shuliang,spec ) values("
                              "'%1','%2','%3','%4','%5','%6','%7','%8','%9','%10',"
                              "'%11')")
            .arg(data.color).arg(data.comment).arg(data.contract_id).arg(data.danjia)
            .arg(data.danwei).arg(data.jine).arg(data.maogao).arg(data.menfu)
            .arg(data.name).arg(data.shuliang).arg(data.spec);
    qDebug() << sq;
    QSqlQuery query;
    query.exec(sq);
    if( query.isActive()){
        QMessageBox::about(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("添加流水账成功"));
        return true;
    }else{
        QMessageBox::about(NULL, QString::fromLocal8Bit("错误提示"), query.lastError().databaseText());
        QMessageBox::about(NULL, QString::fromLocal8Bit("错误提示"), QString::fromLocal8Bit("添加流水账失败"));
        return false;
    }

}
bool MysqlOperate::MysqlOperate::find_factory_contract(QString contract_id,S_factory_contract &data){
    QString sq=QStringLiteral("select "
                              "contract_address,contract_id,jia_account,jia_address,"
                              "jia_agency,jia_bank,jia_bank,jia_faren,"
                              "jia_fax,jia_name,jia_telephone,jiesuan_daxie,"
                              "jiesuan_xiaoxie,yi_account,yi_address,yi_agency,"
                              "yi_bank,yi_date,yi_faren,yi_fax,"
                              "yi_name,yi_telephone from factory_contract where contract_id = '%1'").arg(contract_id);
    qDebug() << sq;
    QSqlQuery query(sq);
    if( query.isActive()){
        while(query.next()){
            data.contract_address = query.value(0).toString();
            data.contract_id = query.value(1).toString();
            data.jia_account = query.value(2).toString();
            data.jia_address = query.value(3).toString();
            data.jia_agency = query.value(3).toString();

            return true;
        }
    }
    return false;

}
//bool MysqlOperate::MysqlOperate::find_factory_product(QString contract_id,std::vector<S_factory_product> &data);
//
//
//bool MysqlOperate::MysqlOperate::add_mianliao_ruku(S_mianliao_ruku &data);
//bool MysqlOperate::MysqlOperate::add_mianliao_chuku(S_mianliao_chuku &data);
//bool MysqlOperate::MysqlOperate::find_mianliao_ruku(QString ruku_id,S_mianliao_ruku &data);
//bool MysqlOperate::MysqlOperate::find_mianliao_chuku(QString ruku_id,std::vector<S_mianliao_chuku> &data);



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

