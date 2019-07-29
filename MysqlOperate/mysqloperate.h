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
#include <QStringList>
#include <QMessageBox>
#define PAGESIZE 30

class MakeConditions{
public:
    MakeConditions(QString TableName);
    MakeConditions& AddEqual(QString column,QString value);
    MakeConditions& AddBetween(QString column,QString begin,QString end);
    MakeConditions& AddLike(QString column,QString value);
    QString& AddPage(int page);
    MakeConditions& AddIn(QString mycolumn,QString targetcolumn,MakeConditions &target_condition);
    QString &Get();
    operator QString&(){return Get();}
private:
    bool IsColumnLegal(QString column);
    void DeleteLastAnd();
    bool hasDelete;
    QString m_Conditions;
    QString m_TableName;
    QSet<QString> m_columns;

    friend class MysqlOperate;
};


class MysqlOperate
{
private:
  static MysqlOperate *local_instance;
public:
  MysqlOperate();
  static MysqlOperate *getInstance();
    bool MysqlOpen();  //数据库打开连接
  void Transaction();
  void Commit();
  void Rollback();
private:
  QSqlDatabase defaultDB;

public:
  bool Get(MakeConditions &Conditions,QMap<QString,QString> &data);
  bool Find(MakeConditions &Conditions,QVector<QMap<QString,QString>> &data);
  int Count(MakeConditions &Conditions);
  bool Delete(MakeConditions &Conditions);
  qlonglong Sum(QString columnName,MakeConditions &Conditions);
  bool Update(MakeConditions &Conditions,QMap<QString,QString> &data);
  bool Insert(QString tableName,QMap<QString,QString> &data);

};


#define SS(str) QString::fromLocal8Bit(str)
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)>(y)?(y):(x))
#endif // MYSQLOPERATE_H
