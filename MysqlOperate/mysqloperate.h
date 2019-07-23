#ifndef MYSQLOPERATE_H
#define MYSQLOPERATE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <vector>

#include "mysqlstruct.h"
#include <QJsonObject>
#include <QVector>

class MysqlOperate
{
private:
  static MysqlOperate *local_instance;
public:
  MysqlOperate();
  static MysqlOperate *getInstance();
public:
    bool MysqlOpen();  //数据库打开连接
private:
  QSqlDatabase defaultDB;

public:
  bool Insert(QString tableName,QMap<QString,QString> &data);
  bool Find(QString tableName,QMap<QString,QString> &refer,QVector<QMap<QString,QString>> &data);
  bool Delete(QString tableName,QMap<QString,QString> &refer);
  bool Update(QString tableName,QMap<QString,QString> &refer ,QMap<QString,QString> &data);


  bool find_user(QString name,QString password);
  bool find_auth(QString role,QJsonObject &jo);


  bool add_factory_contract(S_factory_contract &data);
  bool add_factory_product(S_factory_product &data);
  bool find_factory_contract(QString contract_id,S_factory_contract &data);
  bool find_factory_product(QString contract_id,std::vector<S_factory_product> &data);


  bool add_mianliao_ruku(S_mianliao_ruku &data);
  bool add_mianliao_chuku(S_mianliao_chuku &data);
  bool find_mianliao_ruku(QString ruku_id,S_mianliao_ruku &data);
  bool find_mianliao_chuku(QString ruku_id,std::vector<S_mianliao_chuku> &data);

};


#define SS(str) QString::fromLocal8Bit(str)
#define MAX(x,y) (x)>(y)?(x):(y)
#define MIN(x,y) (x)>(y)?(y):(x)
#endif // MYSQLOPERATE_H
