#include "mysqloperate.h"
#include <QCryptographicHash>
#include "MysqlTableConfig/MysqlTableConfig.h"
MysqlOperate *MysqlOperate::local_instance = nullptr;

   MakeConditions::MakeConditions(QString TableName){
       m_TableName =TableName;
       if( !MysqlTableConfig::getInstance()->is_legal_table(m_TableName)){
           QMessageBox::about(NULL, SS("错误提示"), SS("该表没有配置文件:")+m_TableName);
       }
       m_columns = MysqlTableConfig::getInstance()->get_table_columns(m_TableName);
       hasDelete = true;
   }

   MakeConditions& MakeConditions::AddEqual(QString column,QString value){
       if( m_Conditions.length() == 0) m_Conditions += " where " ;
       if( !IsColumnLegal(column)) return *this;
       m_Conditions += column + "= '" + value + "' and " ;
       hasDelete = false;
       return *this;
   }
   MakeConditions& MakeConditions::AddBetween(QString column,QString begin,QString end){
       if( m_Conditions.length() == 0) m_Conditions += " where " ;
       if( !IsColumnLegal(column)) return *this;
       m_Conditions += column + " between '" + begin + "' and '" +end + "' and "  ;
       hasDelete = false;
       return *this;
   }
   MakeConditions& MakeConditions::AddLike(QString column,QString value){
       if( m_Conditions.length() == 0) m_Conditions += " where " ;
       if( !IsColumnLegal(column)) return *this;
       m_Conditions += column + " like '%" + value + "%' and " ;
       hasDelete = false;
       return *this;
   }
   QString& MakeConditions::AddPage(int page){
       if( !hasDelete) DeleteLastAnd();
       m_Conditions += QString(" limit %1 offset %2").arg(PAGESIZE).arg(PAGESIZE*(page-1));
       return Get();
   }
   void MakeConditions::DeleteLastAnd(){
       if(m_Conditions.length() > 0){
           m_Conditions = m_Conditions.left(m_Conditions.length()-5);
           hasDelete = true;
       }
   }
   MakeConditions& MakeConditions::AddIn(QString mycolumn,QString targetcolumn,MakeConditions &target_condition){
       if( m_Conditions.length() == 0) m_Conditions += " where " ;
       if( !IsColumnLegal(mycolumn)) return *this;
       if( !target_condition.IsColumnLegal(targetcolumn)) return *this;
       m_Conditions += mycolumn + " in ( select " + targetcolumn + " from "+ target_condition.m_TableName + target_condition.Get() + ") and " ;
       hasDelete = false;
       return *this;
   }
   QString &MakeConditions::Get(){
       if( !hasDelete) DeleteLastAnd();
       return m_Conditions;
   }

   bool MakeConditions::IsColumnLegal(QString column){
       if( m_columns.find(column) == m_columns.constEnd()) {
            QMessageBox::about(NULL, SS("错误提示"), SS("表字段有误:")+m_TableName);
            return false;
       }
       return true;
   }


MysqlOperate::MysqlOperate()
{
}

#include <QSqlDriver>
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
            defaultDB.setConnectOptions("MYSQL_OPT_RECONNECT=1;");
            return true;
        }else{
              QMessageBox::about(NULL, QString::fromLocal8Bit("错误提示"), defaultDB.lastError().databaseText());
              return false;
        }
    }
}

bool MysqlOperate::Insert(QString tableName,QMap<QString,QString> &data){
    if(!defaultDB.isOpen() ) MysqlOpen();
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
        return true;
    }else{
        QMessageBox::about(NULL, SS("错误提示"), query.lastError().databaseText());
        return false;
    }
}

int MysqlOperate::Count(MakeConditions &Conditions){
    if(!defaultDB.isOpen() ) MysqlOpen();
   QString sq1 = "select count(*) from " + Conditions.m_TableName + Conditions.Get();
   qDebug() << sq1;
   QSqlQuery query(sq1);
   if( query.isActive()){
       while(query.next()){
           return query.value(0).toInt();
       }
   }
   if( query.lastError().nativeErrorCode() == "2006"){
       MysqlOpen();
   }
   QMessageBox::about(NULL, SS("提示"), SS("数据count失败")+ Conditions.m_TableName);
   return -1;
}

bool MysqlOperate::Find(MakeConditions &Conditions,QVector<QMap<QString,QString>> &data){
    if(!defaultDB.isOpen() ) MysqlOpen();
    data.clear();
    QString sq1 = "select ";
    QSet<QString>::const_iterator itset;
    QSet<QString> &columns = Conditions.m_columns;
    for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
        sq1 += *itset + ",";
    }
    sq1[sq1.length()-1] = ' ';
    sq1 += "from " + Conditions.m_TableName + Conditions;
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
    if( query.lastError().nativeErrorCode() == "2006"){
        MysqlOpen();
    }
    return false;
}


bool MysqlOperate::Get(MakeConditions &Conditions,QMap<QString,QString> &data){
    if(!defaultDB.isOpen() ) MysqlOpen();
    data.clear();
    QString sq1 = "select ";
    QSet<QString>::const_iterator itset;
    QSet<QString> &columns = Conditions.m_columns;
    for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
        sq1 += *itset + ",";
    }
    sq1[sq1.length()-1] = ' ';
    sq1 += "from " + Conditions.m_TableName + Conditions;
    qDebug() << sq1;
    QSqlQuery query(sq1);
    if( query.isActive()){
        while(query.next()){
            int i=0;
            for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset,i++) {
               data.insert(*itset,query.value(i).toString());
            }
            qDebug() << data;
            return true;
        }
    }
    if( query.lastError().nativeErrorCode() == "2006"){
        MysqlOpen();
    }
    return false;

}


qlonglong MysqlOperate::Sum(QString columnName,MakeConditions &Conditions){
    if(!defaultDB.isOpen() ) MysqlOpen();
    QString sq = "select sum(" + columnName + ") from ";
    sq += Conditions.m_TableName + Conditions.Get();
    qDebug() << sq;
    QSqlQuery query(sq);
    if( query.isActive()){
        while(query.next()){
            qlonglong sum = query.value(0).toLongLong();
            return sum;
        }
    }
    QMessageBox::about(NULL, SS("提示"), SS("数据sum失败")+ Conditions.m_TableName);
    return -1;
}


bool MysqlOperate::Delete(MakeConditions &Conditions){
    if(!defaultDB.isOpen() ) MysqlOpen();
    QString sq = "delete from ";
    sq += Conditions.m_TableName + Conditions;
    QSqlQuery query;
    query.exec(sq);
    qDebug() << sq;
    if( query.isActive()){
        return true;
    }else{
        QMessageBox::about(NULL, SS("错误提示"), query.lastError().databaseText());
        if( query.lastError().nativeErrorCode() == "2006"){
            MysqlOpen();
        }
        return false;
    }
}


bool MysqlOperate::Update(MakeConditions &Conditions,QMap<QString,QString> &data){
    if(!defaultDB.isOpen() ) MysqlOpen();
    if( data.count("version") == 0) {
        QMessageBox::about(NULL, SS("错误提示"), SS("更新没有version字段,不安全:")+Conditions.m_TableName);
        return false;
    }
    int version = data.value("version").toInt() + 1;
    data.insert("version",QString::number(version));
    QString sq = "update ";
    sq += Conditions.m_TableName + " set ";
    QMap<QString, QString>::const_iterator it;
    for (it = data.constBegin(); it != data.constEnd(); ++it) {
        if( !Conditions.IsColumnLegal(it.key())) {
             QMessageBox::about(NULL, SS("错误提示"), SS("表字段有误:")+Conditions.m_TableName + it.key());
             return false;
        }
        sq += it.key() + "= '" + it.value() + "',";
    }

    sq[sq.length()-1] = ' ';
    sq += Conditions;

    QSqlQuery query;
    query.exec(sq);
    qDebug() << sq;
    if( query.isActive()){
        return true;
    }else{
        QMessageBox::about(NULL, SS("错误提示"), query.lastError().databaseText());
        if( query.lastError().nativeErrorCode() == "2006"){
            MysqlOpen();
        }
        return false;
    }
   return true;
}

void MysqlOperate::Transaction(){
    if(!defaultDB.isOpen() ) MysqlOpen();
   QSqlQuery query ;
   query.exec("START TRANSACTION");//
   if( query.lastError().nativeErrorCode() == "2006"){
       MysqlOpen();
       query.exec("START TRANSACTION");
   }
}
void MysqlOperate::Commit(){
    if(!defaultDB.isOpen() ) MysqlOpen();
    QSqlQuery query ;
    query.exec("COMMIT");//
    if( query.lastError().nativeErrorCode() == "2006"){
        MysqlOpen();
        query.exec("COMMIT");
    }
}

void MysqlOperate::Rollback(){
    if(!defaultDB.isOpen() ) MysqlOpen();
    QSqlQuery query ;
    query.exec("ROLLBACK");
    if( query.lastError().nativeErrorCode() == "2006"){
        MysqlOpen();
        query.exec("ROLLBACK");
    }

}

