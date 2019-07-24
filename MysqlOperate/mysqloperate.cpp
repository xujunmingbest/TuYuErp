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
    if( !MysqlTableConfig::getInstance()->is_legal_table(tableName)){
        QMessageBox::about(NULL, SS("错误提示"), SS("该表没有配置文件:")+tableName);
        return false;
    }
    QSet<QString> columns =  MysqlTableConfig::getInstance()->get_table_columns(tableName);
    QString sq1 = "insert into ";
    sq1 += tableName + " (";
    QString sq2 = " values( ";
    QMap<QString, QString>::const_iterator it;
    for (it = data.constBegin(); it != data.constEnd(); ++it) {
        if( columns.find(it.key()) == columns.constEnd()) {
             QMessageBox::about(NULL, SS("错误提示"), SS("表字段有误:")+tableName);
             return false;
        }
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

bool MysqlOperate::ConditionHasData(QMap<QString,QString> *conditions,QMap<QString,QString> *likeconditions){
    if(conditions != nullptr){
        if( conditions->size() > 0) return true;
    }
    if( likeconditions != nullptr){
        if( likeconditions->size() > 0) return true;
    }
    return false;
}
int MysqlOperate::Count(QString tableName,QMap<QString,QString> *conditions,QMap<QString,QString> *likeconditions){
    if( !MysqlTableConfig::getInstance()->is_legal_table(tableName)){
        QMessageBox::about(NULL, SS("错误提示"), SS("该表没有配置文件:")+tableName);
        return false;
    }
    QSet<QString> columns =  MysqlTableConfig::getInstance()->get_table_columns(tableName);
    QString sq1 = "select count(*) from " + tableName;
    if( ConditionHasData(conditions,likeconditions) ){
        sq1 += " where ";
    }
    if(conditions !=nullptr){
        QMap<QString, QString>::const_iterator it;
        for (it = conditions->constBegin(); it != conditions->constEnd(); ++it) {
            if( columns.find(it.key()) == columns.constEnd()) {
                QMessageBox::about(NULL, SS("错误提示"), SS("表字段有误:")+tableName);
                return false;
           }
            sq1 += it.key() + "= '" + it.value() + "' and ";
        }
    }
    if(  likeconditions != nullptr){
        QMap<QString, QString>::const_iterator it;
        for (it = likeconditions->constBegin(); it != likeconditions->constEnd(); ++it) {
            if( columns.find(it.key()) == columns.constEnd()) {
                QMessageBox::about(NULL, SS("错误提示"), SS("表字段有误:")+tableName);
                return false;
           }
            sq1 += it.key() + " like '%" + it.value() + "%' and ";
        }
    }
    if(  ConditionHasData(conditions,likeconditions) ){
         sq1 = sq1.left(sq1.length()-5);
    }
    qDebug() << sq1;
    QSqlQuery query(sq1);
    if( query.isActive()){
        while(query.next()){
            return query.value(0).toInt();
        }
    }
    return 0;
}



bool MysqlOperate::Find(QString tableName,QMap<QString,QString> *conditions,QMap<QString,QString> *likeconditions,QVector<QMap<QString,QString>> &data,int page){
    data.clear();
    if( !MysqlTableConfig::getInstance()->is_legal_table(tableName)){
        QMessageBox::about(NULL, SS("错误提示"), SS("该表没有配置文件:")+tableName);
        return false;
    }
    QSet<QString> columns =  MysqlTableConfig::getInstance()->get_table_columns(tableName);
    QString sq1 = "select ";
    QSet<QString>::const_iterator itset;
    for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
        sq1 += *itset + ",";
    }
    sq1[sq1.length()-1] = ' ';
    sq1 += "from " + tableName ;
    if( ConditionHasData(conditions,likeconditions) ){
        sq1 += " where ";
    }

    if(conditions !=nullptr){
        QMap<QString, QString>::const_iterator it;
        for (it = conditions->constBegin(); it != conditions->constEnd(); ++it) {
            if( columns.find(it.key()) == columns.constEnd()) {
                QMessageBox::about(NULL, SS("错误提示"), SS("表字段有误:")+tableName);
                return false;
           }
            sq1 += it.key() + "= '" + it.value() + "' and ";
        }
    }
    if(  likeconditions != nullptr){
        QMap<QString, QString>::const_iterator it;
        for (it = likeconditions->constBegin(); it != likeconditions->constEnd(); ++it) {
            if( columns.find(it.key()) == columns.constEnd()) {
                QMessageBox::about(NULL, SS("错误提示"), SS("表字段有误:")+tableName);
                return false;
           }
            sq1 += it.key() + " like '%" + it.value() + "%' and ";
        }
    }
    if( ConditionHasData(conditions,likeconditions) ){
         sq1 = sq1.left(sq1.length()-5);
    }

    sq1 +=QString(" limit %1 offset %2").arg(PAGESIZE).arg(PAGESIZE*(page-1));


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
   if( !MysqlTableConfig::getInstance()->is_legal_table(tableName)){
       QMessageBox::about(NULL, SS("错误提示"), SS("该表没有配置文件:")+tableName);
       return false;
   }
   QSet<QString> columns =  MysqlTableConfig::getInstance()->get_table_columns(tableName);
   QString sq = "delete from ";
   sq += tableName;
   if(refer.size() > 0){
        sq += " where ";
        QMap<QString, QString>::const_iterator it;
        for (it = refer.constBegin(); it != refer.constEnd(); ++it) {
            if( columns.find(it.key()) == columns.constEnd()) {
                 QMessageBox::about(NULL, SS("错误提示"), SS("表字段有误:")+tableName);
                 return false;
            }
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
    if( !MysqlTableConfig::getInstance()->is_legal_table(tableName)) {
        QMessageBox::about(NULL, SS("错误提示"), SS("该表没有配置文件:")+tableName);
        return false;
    }
    QSet<QString> columns =  MysqlTableConfig::getInstance()->get_table_columns(tableName);
    if(data.size() == 0 ) return false;
    QString sq = "update ";
    sq += tableName + " set ";
    QMap<QString, QString>::const_iterator it;
    for (it = data.constBegin(); it != data.constEnd(); ++it) {
        if( columns.find(it.key()) == columns.constEnd()) {
             QMessageBox::about(NULL, SS("错误提示"), SS("表字段有误:")+tableName);
             return false;
        }
        sq += it.key() + "= '" + it.value() + "',";
    }
    sq[sq.length()-1] = ' ';


    if(refer.size() > 0){
         sq += " where ";
         QMap<QString, QString>::const_iterator it;
         for (it = refer.constBegin(); it != refer.constEnd(); ++it) {
             if( columns.find(it.key()) == columns.constEnd()) {
                  QMessageBox::about(NULL, SS("错误提示"), SS("表字段有误:")+tableName);
                  return false;
             }
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
