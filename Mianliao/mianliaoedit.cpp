#include "mianliaoedit.h"
#include "ui_mianliaoedit.h"

MianliaoEdit::MianliaoEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MianliaoEdit)
{
    ui->setupUi(this);
    m_MysqlOperate = MysqlOperate::getInstance();
    jieyu_mishu = 0;
    jieyu_gongjin = 0;
    jieyu_juanshu = 0;
    mishu = 0;
    gongjin = 0;
    juanshu = 0;

    QIntValidator *IntValidator =new QIntValidator(0, 10000000);

    ui->LineEdit_mishu->setValidator(IntValidator);
    ui->LineEdit_juanshu->setValidator(IntValidator);
    ui->LineEdit_gongjin->setValidator(IntValidator);
}

MianliaoEdit::~MianliaoEdit()
{
    delete ui;
}


#include "MysqlTableConfig/MysqlTableConfig.h"
bool MianliaoEdit::RukuSlotValider(){
    if( ui->LineEdit_ruku_id->text().trimmed().length() == 0){
        QMessageBox::about(NULL, SS("错误提示"), SS("请填写入库单号"));
        return false;
    }
    if( ui->LineEdit_name->text().trimmed().length() == 0){
        QMessageBox::about(NULL, SS("错误提示"), SS("请填写面料名称"));
        return false;
    }
    int MiShuRead =ui->LineEdit_mishu->text().trimmed().toInt();
    int JuanShuRead =ui->LineEdit_juanshu->text().trimmed().toInt();
    int GongjingRead =ui->LineEdit_gongjin->text().trimmed().toInt();

    if( MiShuRead < (mishu-jieyu_mishu)) {
        QMessageBox::about(NULL, SS("错误提示"), SS("米数小于出库库存,当前入库单号米数已出库:")+QString::number(mishu-jieyu_mishu));
        return false;
    }

    if( GongjingRead < (gongjin-jieyu_gongjin)) {
        QMessageBox::about(NULL, SS("错误提示"), SS("公斤数小于出库库存,当前入库单号公斤数已出库:")+QString::number(gongjin-jieyu_gongjin));
        return false;
    }

    if( JuanShuRead < (juanshu-jieyu_juanshu)) {
        QMessageBox::about(NULL, SS("错误提示"), SS("卷数小于出库库存,当前入库单号卷数已出库:")+QString::number(juanshu-jieyu_juanshu));
        return false;
    }
    jieyu_mishu = MiShuRead -(mishu-jieyu_mishu);
    jieyu_juanshu = JuanShuRead -(juanshu-jieyu_juanshu);
    jieyu_gongjin = GongjingRead -(gongjin-jieyu_gongjin);
    juanshu = JuanShuRead;
    mishu = MiShuRead;
    gongjin   = GongjingRead;
    return true;
}

void MianliaoEdit::MianLiaoRukuEditSlot(){
    if( !RukuSlotValider()) return ;

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

    RukuData.insert("jieyu_mishu",QString::number(jieyu_mishu));
    RukuData.insert("jieyu_gongjin",QString::number(jieyu_gongjin));
    RukuData.insert("jieyu_juanshu",QString::number(jieyu_juanshu));

    MakeConditions cd("mianliao_ruku");
    cd.AddEqual("ruku_id",ruku_id);
    cd.AddEqual("version",QString::number(version));
    RukuData.insert("version",QString::number(version));

    if( m_MysqlOperate->Update(cd,RukuData)){
       QMessageBox::about(NULL, SS("提示"), SS("面料更新成功"));
       this->close();
    }
}


bool MianliaoEdit::LoadData(QString Ruku_id){
    ruku_id =Ruku_id;
    MakeConditions cd("mianliao_ruku");
    cd.AddEqual("ruku_id",ruku_id);
    QMap<QString,QString> data;
    if( !m_MysqlOperate->Get(cd,data) ) return false;

    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("mianliao_ruku");

    QSet<QString>::const_iterator itset;
    for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
        {
            QString objectName = "LineEdit_" + *itset;
            QLineEdit *object = this->findChild<QLineEdit *>(objectName);
            if( object != nullptr){
                object->setText(data.value(*itset));
                continue;
            }
        }
        {
            QString objectName = "dateEdit_" + *itset;
            QDateEdit *object = this->findChild<QDateEdit *>(objectName);
            if( object != nullptr){
                object->setDate(QDate::fromString(data.value(*itset)));
                continue;
            }
        }
    }
    jieyu_mishu = data.value("jieyu_mishu").toInt();
    jieyu_gongjin = data.value("jieyu_gongjin").toInt();
    jieyu_juanshu =data.value("jieyu_juanshu").toInt();
    mishu = data.value("mishu").toInt();
    gongjin = data.value("gongjin").toInt();
    juanshu = data.value("juanshu").toInt();
    version = data.value("version").toInt();
    return true;
}
