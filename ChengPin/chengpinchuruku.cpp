#include "chengpinchuruku.h"
#include "ui_chengpinchuruku.h"

ChengPinChuRuKu::ChengPinChuRuKu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChengPinChuRuKu)
{
    ui->setupUi(this);

    QIntValidator *IntValidator =new QIntValidator(0, 10000000);

    ui->LineEdit_shuliang->setValidator(IntValidator);
    ui->LineEdit_shuliang_2->setValidator(IntValidator);

    m_MysqlOperate = MysqlOperate::getInstance();

    QTableWidget *m_tb = ui->tableWidget_jieyu;
    m_tb->setColumnCount(6);
    m_tb->setRowCount(0);
    m_tb->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList head ;
    head << SS("入库单号") << SS("成品名称") << SS("订单号")<< SS("剩余数量")
         <<SS("单位") <<SS("明细");

    m_tb->setHorizontalHeaderLabels(head);
    m_tb->horizontalHeader()->setMinimumHeight(40);
    m_tb->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;font:13pt}");

}

ChengPinChuRuKu::~ChengPinChuRuKu()
{
    delete ui;
}

#include "MysqlTableConfig/MysqlTableConfig.h"
void ChengPinChuRuKu::ChengPinRukuSlot(){
    if( !RukuSlotValider()) return ;

    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("chengpin_ruku");
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
    RukuData.insert("jieyu_shuliang",RukuData.value("shuliang"));

    if( m_MysqlOperate->Insert("chengpin_ruku",RukuData)){
       QMessageBox::about(NULL, SS("提示"), SS("成品入库成功"));
    }

}
void ChengPinChuRuKu::ChengPinChukuSlot(){
    QMap<QString,QString> RuKudata;
    if( !ChukuSlotValider(RuKudata)) return ;
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
        {"version",RuKudata.value("version")},
        {"jieyu_shuliang",RuKudata.value("jieyu_shuliang")},
    };

    MakeConditions c_ruku("chengpin_ruku");
    c_ruku.AddEqual("ruku_id",ui->LineEdit_ruku_id_2->text().trimmed()).AddEqual("version",RuKudata.value("version"));


    m_MysqlOperate->Transaction();
    bool s1 = m_MysqlOperate->Insert("chengpin_chuku",ChukuData);
    bool s2 = m_MysqlOperate->Update(c_ruku,RukuUpdateData);

    if(s1 && s2 ){
        QMessageBox::about(NULL, SS("提示"), SS("成品出库成功"));
        m_MysqlOperate->Commit();
    }else{
        QMessageBox::about(NULL, SS("提示"), SS("成品出库失败"));
        m_MysqlOperate->Rollback();
    }

}

bool ChengPinChuRuKu::RukuSlotValider(){
    if( ui->LineEdit_ruku_id->text().trimmed().length() == 0){
        QMessageBox::about(NULL, SS("错误提示"), SS("请填写入库单号"));
        return false;
    }
    if( ui->LineEdit_name->text().trimmed().length() == 0){
        QMessageBox::about(NULL, SS("错误提示"), SS("请填写成品名称"));
        return false;
    }
    if( ui->LineEdit_shuliang->text().trimmed().toInt() <= 0){
        QMessageBox::about(NULL, SS("错误提示"), SS("数量错误"));
        return false;
    }
    return true;
}

bool ChengPinChuRuKu::ChukuSlotValider(QMap<QString,QString> &RuKudata){
    QString RuKuId = ui->LineEdit_ruku_id_2->text().trimmed();
    if( RuKuId.length() == 0){
        QMessageBox::about(NULL, SS("错误提示"), SS("请填写入库单号"));
        return false;
    }
    MakeConditions ruku_conditions("chengpin_ruku");
    MakeConditions chuku_conditions("chengpin_chuku");
    ruku_conditions.AddEqual("ruku_id",RuKuId);
    chuku_conditions.AddEqual("ruku_id",RuKuId);

    int shuliangRead =ui->LineEdit_shuliang_2->text().trimmed().toInt();
    if( shuliangRead == 0 ){
        QMessageBox::about(NULL, SS("错误提示"), SS("数量 不能为0"));
        return false;
    }

    m_MysqlOperate->Get(ruku_conditions,RuKudata);
    if(RuKudata.size() == 0 ){
        QMessageBox::about(NULL, SS("错误提示"), SS("查询不到成品入库单号"));
        return false;
    }

    int jieyu_shuliang = RuKudata.value("jieyu_shuliang").toInt();
    if( shuliangRead > jieyu_shuliang) {
        QMessageBox::about(NULL, SS("错误提示"), SS("数量超过订单库存,当前入库单号数量库存:")+QString::number(jieyu_shuliang));
        return false;
    }
    RuKudata.insert("jieyu_shuliang",QString::number(jieyu_shuliang-shuliangRead));

    return true;
}

void ChengPinChuRuKu::ChengPinJieYuSlot(){
    QString Name = ui->LineEdit_name_3->text().trimmed();
    QString OrderId = ui->LineEdit_order_id_3->text().trimmed();
    MakeConditions ruku_conditions("chengpin_ruku") ;

    if(Name.length() != 0 ) ruku_conditions.AddEqual("name",Name);
    if(OrderId.length() != 0 ) ruku_conditions.AddEqual("order_id",OrderId);
    if( ruku_conditions.Get().length() == 0)  {
        QMessageBox::about(NULL, SS("错误提示"), SS("无筛选条件"));
        return;
    }

    MysqlOperate * m_MysqlOperate = MysqlOperate::getInstance();
    //获取总结余
    {
      qlonglong jieyu_shuliang = m_MysqlOperate->Sum("jieyu_shuliang",ruku_conditions);
      ui->LineEdit_shuliang_3->setText(QString::number(jieyu_shuliang));
    }

    TiaoZhuang(1);
}

void ChengPinChuRuKu::TiaoZhuang(int page){

    MakeConditions ruku_cd("chengpin_ruku");
    QString Name = ui->LineEdit_name_3->text().trimmed();
    QString OrderId = ui->LineEdit_order_id_3->text().trimmed();
    if(Name.length() != 0 ) ruku_cd.AddEqual("name",Name);
    if(OrderId.length() != 0 ) ruku_cd.AddEqual("order_id",OrderId);
    if( ruku_cd.Get().length() == 0)  {
        QMessageBox::about(NULL, SS("错误提示"), SS("无筛选条件"));
        return;
    }

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
        m_tb->setItem(i,1,new QTableWidgetItem( data[i].value("name")));
        m_tb->setItem(i,2,new QTableWidgetItem( data[i].value("order_id")));
        m_tb->setItem(i,3,new QTableWidgetItem(data[i].value("jieyu_shuliang")));
        m_tb->setItem(i,4,new QTableWidgetItem(data[i].value("danwei")));
        m_tb->setItem(i,5,new QTableWidgetItem(data[i].value("detail")));
    }
}

void ChengPinChuRuKu::TiaoZhuangSlot(){
    int page = ui->lineEdit_page->text().trimmed().toInt();
    TiaoZhuang(page);
}
