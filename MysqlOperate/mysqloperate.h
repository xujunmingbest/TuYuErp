#ifndef MYSQLOPERATE_H
#define MYSQLOPERATE_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>


#include "mysqlstruct.h"

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

};


#define SS(str) QString::fromLocal8Bit(str)
#define MAX(x,y) (x)>(y)?(x):(y)
#define MIN(x,y) (x)>(y)?(y):(x)
#endif // MYSQLOPERATE_H
