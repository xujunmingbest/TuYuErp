#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <QMap>

class CUser //权限类
{
  private:
    static CUser *local_instance;

  public:
    static CUser *getInstance();
    QString get_auth(QString MoKuaiName);
    QString get_name(){return m_name;};
    bool LoadAuth(); //加载权限
    void set_role(QString role){m_role = role;}
  private:
    QString m_role; //用户角色
    QString m_name; //用户名字
    QMap<QString, QString> m_authmap;
};
CUser *CUser::local_instance = nullptr;



#endif // GLOBAL_H
