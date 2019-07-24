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

#define PAGESIZE 30

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
  bool Find(QString tableName,QMap<QString,QString> *conditions,QMap<QString,QString> *likeconditions,QVector<QMap<QString,QString>> &data,int page=1);
  bool Delete(QString tableName,QMap<QString,QString> &conditions);
  bool Update(QString tableName,QMap<QString,QString> &conditions ,QMap<QString,QString> &data);
  int Count(QString tableName,QMap<QString,QString> *conditions,QMap<QString,QString> *likeconditions);
private:
  bool ConditionHasData(QMap<QString,QString> *conditions,QMap<QString,QString> *likeconditions);

};


#define SS(str) QString::fromLocal8Bit(str)
#define MAX(x,y) (x)>(y)?(x):(y)
#define MIN(x,y) (x)>(y)?(y):(x)
#endif // MYSQLOPERATE_H
