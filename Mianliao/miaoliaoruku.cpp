#include "miaoliaoruku.h"
#include "ui_miaoliaoruku.h"

MiaoliaoRuku::MiaoliaoRuku(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MiaoliaoRuku)
{
    ui->setupUi(this);


}

MiaoliaoRuku::~MiaoliaoRuku()
{
    delete ui;
}
#include "MysqlOperate/mysqloperate.h"
#include "MysqlTableConfig/MysqlTableConfig.h"

void MiaoliaoRuku::MianLiaoRukuSlot(){
    MysqlOperate * m_MysqlOperate = MysqlOperate::getInstance();
    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("mianliao_ruku");
    QMap<QString,QString> RukuData;
    QSet<QString>::const_iterator itset;
    for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
        {
            QString objectName = "LineEdit_" + *itset;
            QLineEdit *object = this->findChild<QLineEdit *>(objectName);
            if( object != nullptr){
                RukuData.insert(*itset,object->text().trimmed());
                continue;
            }
        }
        {
            QString objectName = "dateEdit_" + *itset;
            QDateEdit *object = this->findChild<QDateEdit *>(objectName);
            if( object != nullptr){
                RukuData.insert(*itset,object->text().trimmed());
                continue;
            }
        }

    }
    m_MysqlOperate->Insert("mianliao_ruku",RukuData);
}


void MiaoliaoRuku::MianLiaoChukuSlot(){
    MysqlOperate * m_MysqlOperate = MysqlOperate::getInstance();
    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("mianliao_chuku");
    QMap<QString,QString> RukuData;
    QSet<QString>::const_iterator itset;
    for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
        {
            QString objectName = "LineEdit_" + *itset + "_2"; //加2表示出库
            QLineEdit *object = this->findChild<QLineEdit *>(objectName);
            if( object != nullptr){
                RukuData.insert(*itset,object->text().trimmed());
                continue;
            }
        }
        {
            QString objectName = "dateEdit_" + *itset + "_2"; //加2表示出库
            QDateEdit *object = this->findChild<QDateEdit *>(objectName);
            if( object != nullptr){
                RukuData.insert(*itset,object->text().trimmed());
                continue;
            }
        }

    }
    m_MysqlOperate->Insert("mianliao_chuku",RukuData);

}
