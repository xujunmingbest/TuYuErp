#include "mianliao_chuku_edit.h"
#include "ui_mianliao_chuku_edit.h"

mianliao_chuku_edit::mianliao_chuku_edit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mianliao_chuku_edit)
{
    ui->setupUi(this);

    QIntValidator *IntValidator =new QIntValidator(0, 10000000);

    ui->LineEdit_mishu_2->setValidator(IntValidator);
    ui->LineEdit_juanshu_2->setValidator(IntValidator);
    ui->LineEdit_gongjin_2->setValidator(IntValidator);
    m_MysqlOperate = MysqlOperate::getInstance();
}

mianliao_chuku_edit::~mianliao_chuku_edit()
{
    delete ui;
}
bool mianliao_chuku_edit::ChukuSlotValider(){
    QString RuKuId = ui->LineEdit_ruku_id_2->text().trimmed();

    int MiShuRead =ui->LineEdit_mishu_2->text().trimmed().toInt();
    int JuanShuRead =ui->LineEdit_juanshu_2->text().trimmed().toInt();
    int GongjingRead =ui->LineEdit_gongjin_2->text().trimmed().toInt();
    if( MiShuRead == 0 && JuanShuRead == 0 && GongjingRead == 0){
        QMessageBox::about(NULL, SS("错误提示"), SS("面数 卷数 公斤数 不能全为0"));
        return false;
    }

    if( MiShuRead > jieyu_mishu) {
        QMessageBox::about(NULL, SS("错误提示"), SS("米数超过订单库存,当前入库单号米数库存:")+QString::number(jieyu_mishu));
        return false;
    }

    if( GongjingRead > jieyu_gongjin) {
        QMessageBox::about(NULL, SS("错误提示"), SS("公斤数超过订单库存,当前入库单号公斤数库存:")+QString::number(jieyu_gongjin));
        return false;
    }

    if( JuanShuRead > jieyu_juanshu) {
        QMessageBox::about(NULL, SS("错误提示"), SS("卷数超过订单库存,当前入库单号卷数库存:")+QString::number(jieyu_juanshu));
        return false;
    }

    jieyu_mishu = jieyu_mishu - MiShuRead;
    jieyu_gongjin = jieyu_gongjin - GongjingRead;
    jieyu_juanshu = jieyu_juanshu - JuanShuRead;
    return true;
}
#include "MysqlTableConfig/MysqlTableConfig.h"

void mianliao_chuku_edit::MianLiaoChukuSlot(){
    if( !ChukuSlotValider()) return ;
    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("mianliao_chuku");
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
        {"jieyu_mishu",QString::number(jieyu_mishu)},
        {"jieyu_gongjin",QString::number(jieyu_gongjin)},
        {"jieyu_juanshu",QString::number(jieyu_juanshu)},
    };

    MakeConditions c_ruku("mianliao_ruku");
    c_ruku.AddEqual("ruku_id",ui->LineEdit_ruku_id_2->text().trimmed()).AddEqual("version",QString::number(version_ruku));

    MakeConditions c_chuku("mianliao_chuku");
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

bool mianliao_chuku_edit::LoadData(int Id){
    id = Id;
    MakeConditions cd("mianliao_chuku");
    cd.AddEqual("id",QString::number(Id));
    QMap<QString,QString> data_chuku;
    if( !m_MysqlOperate->Get(cd,data_chuku) ) return false;

    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("mianliao_chuku");

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
                object->setDate(QDate::fromString(data_chuku.value(*itset)));
                continue;
            }
        }
    }
    MakeConditions cd_ruku("mianliao_ruku");
    cd_ruku.AddEqual("ruku_id",data_chuku.value("ruku_id"));
    QMap<QString,QString> data_ruku;
    if( !m_MysqlOperate->Get(cd_ruku,data_ruku) ) return false;

    mishu = data_chuku.value("mishu").toInt();
    gongjin = data_chuku.value("gongjin").toInt();
    juanshu = data_chuku.value("juanshu").toInt();
    version_chuku = data_chuku.value("version").toInt();

    jieyu_mishu = data_ruku.value("jieyu_mishu").toInt() + mishu;
    jieyu_gongjin = data_ruku.value("jieyu_gongjin").toInt() + gongjin;
    jieyu_juanshu =data_ruku.value("jieyu_juanshu").toInt() + juanshu;
    version_ruku = data_ruku.value("version").toInt();
    return true;

}
