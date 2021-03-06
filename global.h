#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <QMap>
#include "MysqlOperate/mysqloperate.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
class CUser //权限类
{
  private:
    static CUser *local_instance;

  public:
    static CUser *getInstance();
    QString get_auth(QString MoKuaiName);
    bool LoadAuth(); //加载权限
    void set_role(QString role){m_role = role;}
    void set_name(QString name){m_name = name;}
    QString get_name(){return m_name;}
    QString get_role(){return m_role;}
  private:
    QString m_role; //用户角色
    QString m_name; //用户名字
    QMap<QString, QString> m_authmap;
};

QJsonObject QstringToJson(QString jsonString);

QString JsonToQstring(QJsonObject jsonObject);

#include <QWidget>
#include <QLabel>

class ClickQlabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickQlabel(QWidget *parent=0, Qt::WindowFlags f=0);
    explicit ClickQlabel(const QString &text, QWidget *parent=0, Qt::WindowFlags f=0);
protected:
    /*!
      由Qt自己调用的鼠标事件处理函数,在此发射clicked信号
    */
    virtual void mouseReleaseEvent(QMouseEvent * ev);
signals:
    void clicked(void);
};






#endif // GLOBAL_H
