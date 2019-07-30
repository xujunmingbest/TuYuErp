#include "chengpinchukuedit.h"
#include "ui_chengpinchukuedit.h"

ChengPinChuKuEdit::ChengPinChuKuEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChengPinChuKuEdit)
{
    ui->setupUi(this);
}

ChengPinChuKuEdit::~ChengPinChuKuEdit()
{
    delete ui;
}

bool ChengPinChuKuEdit::ChukuSlotValider(){
    QString RuKuId = ui->LineEdit_ruku_id_2->text().trimmed();

    int shuliangRead =ui->LineEdit_shuliang_2->text().trimmed().toInt();

    if( shuliangRead == 0 ){
        QMessageBox::about(NULL, SS("错误提示"), SS("数量不能全为0"));
        return false;
    }

    if( shuliangRead > jieyu_shuliang) {
        QMessageBox::about(NULL, SS("错误提示"), SS("数量超过订单库存,当前入库单号数量库存:")+QString::number(jieyu_shuliang));
        return false;
    }


    jieyu_shuliang = jieyu_shuliang - shuliangRead;
    return true;
}
#include "MysqlTableConfig/MysqlTableConfig.h"

void ChengPinChuKuEdit::ChengPinChukuSlot(){
    if( !ChukuSlotValider()) return ;
    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("chengpin_chuku");
    QMap<QString,QString> ChukuData;
    QSet<QString>::const_iterator itset;
    for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
        {
            QString objectName = "LineEdit_" + *itset + "_2"; //加2表示出库
            QLineEdit *object = this->findChild<QLineEdit *>(objectName);
            if( object != nullptr){
                ChukuData.insert(*itset,object->text().trimmed());
                continue;
            }
        }
        {
            QString objectName = "dateEdit_" + *itset + "_2"; //加2表示出库
            QDateEdit *object = this->findChild<QDateEdit *>(objectName);
            if( object != nullptr){
                ChukuData.insert(*itset,object->text().trimmed());
                continue;
            }
        }

    }

    QMap<QString,QString> RukuUpdateData={
        {"version",QString::number(version_ruku)},
        {"jieyu_shuliang",QString::number(jieyu_shuliang)},
    };

    MakeConditions c_ruku("chengpin_ruku");
    c_ruku.AddEqual("ruku_id",ui->LineEdit_ruku_id_2->text().trimmed()).AddEqual("version",QString::number(version_ruku));

    MakeConditions c_chuku("chengpin_chuku");
    c_chuku.AddEqual("id",QString::number(id)).AddEqual("version",QString::number(version_chuku));
    ChukuData.insert("version",QString::number(version_chuku));

    m_MysqlOperate->Transaction();
    bool s1 = m_MysqlOperate->Update(c_chuku,ChukuData);
    bool s2 = m_MysqlOperate->Update(c_ruku,RukuUpdateData);

    if(s1 && s2 ){
        QMessageBox::about(NULL, SS("提示"), SS("面料出库更新成功"));
        m_MysqlOperate->Commit();
    }else{
        QMessageBox::about(NULL, SS("提示"), SS("面料出库更新失败"));
        m_MysqlOperate->Rollback();
    }

}

bool ChengPinChuKuEdit::LoadData(int Id){
    id = Id;
    MakeConditions cd("chengpin_chuku");
    cd.AddEqual("id",QString::number(Id));
    QMap<QString,QString> data_chuku;
    if( !m_MysqlOperate->Get(cd,data_chuku) ) return false;

    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("chengpin_chuku");

    QSet<QString>::const_iterator itset;
    for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
        {
            QString objectName = "LineEdit_" + *itset + "_2";
            QLineEdit *object = this->findChild<QLineEdit *>(objectName);
            if( object != nullptr){
                object->setText(data_chuku.value(*itset));
                continue;
            }
        }
        {
            QString objectName = "dateEdit_" + *itset + "_2";
            QDateEdit *object = this->findChild<QDateEdit *>(objectName);
            if( object != nullptr){
                object->setDate(QDate::fromString(data_chuku.value(*itset),"yyyy-MM-dd"));
                continue;
            }
        }
    }
    MakeConditions cd_ruku("chengpin_ruku");
    cd_ruku.AddEqual("ruku_id",data_chuku.value("ruku_id"));
    QMap<QString,QString> data_ruku;
    if( !m_MysqlOperate->Get(cd_ruku,data_ruku) ) return false;

    shuliang = data_chuku.value("shuliang").toInt();
    version_chuku = data_chuku.value("version").toInt();

    jieyu_shuliang = data_ruku.value("jieyu_shuliang").toInt() + shuliang;
    version_ruku = data_ruku.value("version").toInt();
    return true;

}
