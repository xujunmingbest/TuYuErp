#ifndef MYSQLTABLECONFIG_H
#define MYSQLTABLECONFIG_H

#include <QSet>
#include <QMap>

class MysqlTableConfig //»®œﬁ¿‡
{
private:
        static MysqlTableConfig *local_instance;
  public:
    static MysqlTableConfig *getInstance();
    void LoadConfig();
    bool is_legal_table(QString TableName);
    QSet<QString> get_table_columns(QString TableName);
  private:
    QMap<QString,QSet<QString>> TableConfigdata;
};







#endif // MYSQLTABLECONFIG_H
