#include "miaoliaoruku.h"
#include "ui_miaoliaoruku.h"
#include "MysqlOperate/mysqloperate.h"
#include <QMessageBox>
MiaoliaoRuku::MiaoliaoRuku(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MiaoliaoRuku)
{
    ui->setupUi(this);
    QIntValidator *IntValidator =new QIntValidator(0, 10000000);

    ui->LineEdit_mishu->setValidator(IntValidator);
    ui->LineEdit_juanshu->setValidator(IntValidator);
    ui->LineEdit_gongjin->setValidator(IntValidator);
    ui->LineEdit_mishu_2->setValidator(IntValidator);
    ui->LineEdit_juanshu_2->setValidator(IntValidator);
    ui->LineEdit_gongjin_2->setValidator(IntValidator);

    QTableWidget *m_tb = ui->tableWidget_jieyu;
    m_tb->setColumnCount(7);
    m_tb->setRowCount(0);
    m_tb->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList head ;
    head << SS("入库单号") << SS("剩余米数") << SS("剩余公斤数") <<SS("剩余卷数")
         << SS("面料名称") << SS("门幅") << SS("克重") ;
    m_tb->setHorizontalHeaderLabels(head);
    m_tb->horizontalHeader()->setMinimumHeight(40);
    m_tb->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;font:13pt}");
}

MiaoliaoRuku::~MiaoliaoRuku()
{
    delete ui;
}

#include "MysqlTableConfig/MysqlTableConfig.h"
bool MiaoliaoRuku::RukuSlotValider(){
    if( ui->LineEdit_ruku_id->text().trimmed().length() == 0){
        QMessageBox::about(NULL, SS("错误提示"), SS("请填写入库单号"));
        return false;
    }
    if( ui->LineEdit_name->text().trimmed().length() == 0){
        QMessageBox::about(NULL, SS("错误提示"), SS("请填写面料名称"));
        return false;
    }
    return true;
}
void MiaoliaoRuku::MianLiaoRukuSlot(){
    FormatIntlineEdit();
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
    RukuData.insert("jieyu_mishu",RukuData.value("mishu"));
    RukuData.insert("jieyu_gongjin",RukuData.value("gongjin"));
    RukuData.insert("jieyu_juanshu",RukuData.value("juanshu"));
    if( m_MysqlOperate->Insert("mianliao_ruku",RukuData)){
       QMessageBox::about(NULL, SS("提示"), SS("面料入库成功"));
    }
}

void MiaoliaoRuku::FormatIntlineEdit(){
    if( ui->LineEdit_gongjin->text().trimmed().length() == 0) ui->LineEdit_gongjin->setText("0");
    if( ui->LineEdit_gongjin_2->text().trimmed().length() == 0) ui->LineEdit_gongjin_2->setText("0");

    if( ui->LineEdit_mishu->text().trimmed().length() == 0) ui->LineEdit_mishu->setText("0");
    if( ui->LineEdit_mishu_2->text().trimmed().length() == 0) ui->LineEdit_mishu_2->setText("0");

    if( ui->LineEdit_juanshu->text().trimmed().length() == 0) ui->LineEdit_juanshu->setText("0");
    if( ui->LineEdit_juanshu_2->text().trimmed().length() == 0) ui->LineEdit_juanshu_2->setText("0");

}


bool MiaoliaoRuku::ChukuSlotValider(QVector<QMap<QString,QString>> &RuKudata){
    MysqlOperate * m_MysqlOperate = MysqlOperate::getInstance();
    QString RuKuId = ui->LineEdit_ruku_id_2->text().trimmed();
    if( RuKuId.length() == 0){
        QMessageBox::about(NULL, SS("错误提示"), SS("请填写入库单号"));
        return false;
    }
    MakeConditions ruku_conditions("mianliao_ruku");
    MakeConditions chuku_conditions("mianliao_chuku");
    ruku_conditions.AddEqual("ruku_id",RuKuId);
    chuku_conditions.AddEqual("ruku_id",RuKuId);

    int MiShuRead =ui->LineEdit_mishu_2->text().trimmed().toInt();
    int JuanShuRead =ui->LineEdit_juanshu_2->text().trimmed().toInt();
    int GongjingRead =ui->LineEdit_gongjin_2->text().trimmed().toInt();
    if( MiShuRead == 0 && JuanShuRead == 0 && GongjingRead == 0){
        QMessageBox::about(NULL, SS("错误提示"), SS("面数 卷数 公斤数 不能全为0"));
        return false;
    }

    m_MysqlOperate->Find(ruku_conditions,RuKudata);
    if(RuKudata.size() == 0 ){
        QMessageBox::about(NULL, SS("错误提示"), SS("查询不到面料入库单号"));
        return false;
    }

    int JieYuMiShu = RuKudata[0].value("jieyu_mishu").toInt();
    if( MiShuRead > JieYuMiShu) {
        QMessageBox::about(NULL, SS("错误提示"), SS("米数超过订单库存,当前入库单号米数库存:")+QString::number(JieYuMiShu));
        return false;
    }
     RuKudata[0].insert("jieyu_mishu",QString::number(JieYuMiShu-MiShuRead));

    int JieYuGongjin = RuKudata[0].value("jieyu_gongjin").toInt();
    if( GongjingRead > JieYuGongjin) {
        QMessageBox::about(NULL, SS("错误提示"), SS("公斤数超过订单库存,当前入库单号公斤数库存:")+QString::number(JieYuGongjin));
        return false;
    }
    RuKudata[0].insert("jieyu_gongjin",QString::number(JieYuGongjin-GongjingRead));

    int JieYuJuanShu = RuKudata[0].value("jieyu_juanshu").toInt();
    if( JuanShuRead > JieYuJuanShu) {
        QMessageBox::about(NULL, SS("错误提示"), SS("卷数超过订单库存,当前入库单号卷数库存:")+QString::number(JieYuJuanShu));
        return false;
    }
    RuKudata[0].insert("jieyu_juanshu",QString::number(JieYuJuanShu-JuanShuRead));
    return true;
}

void MiaoliaoRuku::MianLiaoChukuSlot(){
    QVector<QMap<QString,QString>> RuKudata;
    if( !ChukuSlotValider(RuKudata)) return ;
    MysqlOperate * m_MysqlOperate = MysqlOperate::getInstance();
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
        {"version",RuKudata[0].value("version")},
        {"jieyu_mishu",RuKudata[0].value("jieyu_mishu")},
        {"jieyu_gongjin",RuKudata[0].value("jieyu_gongjin")},
        {"jieyu_juanshu",RuKudata[0].value("jieyu_juanshu")},
    };

    MakeConditions c_ruku("mianliao_ruku");
    c_ruku.AddEqual("ruku_id",ui->LineEdit_ruku_id_2->text().trimmed()).AddEqual("version",RuKudata[0].value("version"));


    m_MysqlOperate->Transaction();
    bool s1 = m_MysqlOperate->Insert("mianliao_chuku",ChukuData);
    bool s2 = m_MysqlOperate->Update(c_ruku,RukuUpdateData);

    if(s1 && s2 ){
        QMessageBox::about(NULL, SS("提示"), SS("面料出库成功"));
        m_MysqlOperate->Commit();
    }else{
        QMessageBox::about(NULL, SS("提示"), SS("面料出库失败"));
        m_MysqlOperate->Rollback();
    }

}

void MiaoliaoRuku::MianLiaoJieYuSlot(){
    QString Name = ui->LineEdit_name_3->text().trimmed();
    QString MenFu = ui->LineEdit_menfu_3->text().trimmed();
    QString KeZhong = ui->LineEdit_kezhong_3->text().trimmed();
    MakeConditions ruku_conditions("mianliao_ruku") ;

    if(Name.length() != 0 ) ruku_conditions.AddEqual("name",Name);
    if(MenFu.length() != 0 ) ruku_conditions.AddEqual("menfu",MenFu);
    if(KeZhong.length() != 0 ) ruku_conditions.AddEqual("kezhong",KeZhong);
    if( ruku_conditions.Get().length() == 0)  {
        QMessageBox::about(NULL, SS("错误提示"), SS("无筛选条件"));
        return;
    }

    MysqlOperate * m_MysqlOperate = MysqlOperate::getInstance();
    //获取总结余
    {
      qlonglong MiShujieyu = m_MysqlOperate->Sum("jieyu_mishu",ruku_conditions);
      ui->LineEdit_mishu_3->setText(QString::number(MiShujieyu));
    }
    {
         qlonglong JuanShujieyu = m_MysqlOperate->Sum("jieyu_juanshu",ruku_conditions);
         ui->LineEdit_juanshu_3->setText(QString::number(JuanShujieyu));
    }
    {
        qlonglong Gongjinjieyu = m_MysqlOperate->Sum("jieyu_gongjin",ruku_conditions);
        ui->LineEdit_gongjin_3->setText(QString::number(Gongjinjieyu));
    }

    TiaoZhuang(1);
}

void MiaoliaoRuku::TiaoZhuang(int page){
    MysqlOperate * m_MysqlOperate = MysqlOperate::getInstance();

    MakeConditions ruku_cd("mianliao_ruku");
    QString Name = ui->LineEdit_name_3->text().trimmed();
    QString MenFu = ui->LineEdit_menfu_3->text().trimmed();
    QString KeZhong = ui->LineEdit_kezhong_3->text().trimmed();

    if(Name.length() != 0 ) ruku_cd.AddEqual("name",Name);
    if(MenFu.length() != 0 ) ruku_cd.AddEqual("menfu",MenFu);
    if(KeZhong.length() != 0 ) ruku_cd.AddEqual("kezhong",KeZhong);
    if( ruku_cd.Get().length() == 0)  {
        QMessageBox::about(NULL, SS("错误提示"), SS("无筛选条件"));
        return;
    }
    //int page_read = ui->lineEdit_page->text().trimmed().toInt();
    int data_count = m_MysqlOperate->Count(ruku_cd);
    int total_page = data_count/PAGESIZE;
    if(data_count%PAGESIZE >0 ) total_page++;
    if(page < 1 ) page = 1;
    else if(page > total_page) page = total_page;
    ruku_cd.AddPage(page);
    QString pagetext = QStringLiteral("第 %1 页/ %2 页").arg(page).arg(total_page);
    ui->label_page->setText(pagetext);

    //获取符合条件的data
    QVector<QMap<QString,QString>> data;
    m_MysqlOperate->Find(ruku_cd,data);

    QTableWidget *m_tb = ui->tableWidget_jieyu;
    m_tb->clearContents();
    m_tb->setRowCount(data.size());
    if(data.size() == 0){
        QMessageBox::about(NULL, SS("错误提示"), SS("无数据"));
        return;
    }

    for(int i=0;i<data.size();i++){
        m_tb->setItem(i,0,new QTableWidgetItem(data[i].value("ruku_id")));
        m_tb->setItem(i,1,new QTableWidgetItem( data[i].value("jieyu_mishu")));
        m_tb->setItem(i,2,new QTableWidgetItem(data[i].value("jieyu_gongjin")));
        m_tb->setItem(i,3,new QTableWidgetItem(data[i].value("jieyu_juanshu")));
        m_tb->setItem(i,4,new QTableWidgetItem(data[i].value("name")));
        m_tb->setItem(i,5,new QTableWidgetItem(data[i].value("menfu")));
        m_tb->setItem(i,6,new QTableWidgetItem(data[i].value("kezhong")));
    }
}

void MiaoliaoRuku::TiaoZhuangSlot(){
    int page = ui->lineEdit_page->text().trimmed().toInt();
    TiaoZhuang(page);
}
