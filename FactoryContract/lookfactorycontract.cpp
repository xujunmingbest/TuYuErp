#include "lookfactorycontract.h"
#include "ui_lookfactorycontract.h"
#include "MysqlOperate/mysqloperate.h"
#include "MysqlTableConfig/MysqlTableConfig.h"


LookFactoryContract::LookFactoryContract(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LookFactoryContract)
{
    ui->setupUi(this);
    //tablewidget提添加字段
    QTableWidget *m_tw = ui->table_product;
    m_tw->setColumnCount(11);
    m_tw->setRowCount(0);
    m_tw->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QStringList headers;
    headers << SS("产品名称") << SS("颜色") << SS("规格\n（平方克重）")<< SS("毛高")
            <<SS("有效门幅/cm") <<  SS("单位") <<SS("数量") <<  SS("单价/含税/足米\n（元/米）")
             <<SS("金额（元）") <<  SS("备注");
    m_tw->setHorizontalHeaderLabels(headers);

    ProductTableIndex.insert("name",0);
    ProductTableIndex.insert("color",1);
    ProductTableIndex.insert("spec",2);
    ProductTableIndex.insert("maogao",3);
    ProductTableIndex.insert("menfu",4);
    ProductTableIndex.insert("danwei",5);
    ProductTableIndex.insert("shuliang",6);
    ProductTableIndex.insert("danjia",7);
    ProductTableIndex.insert("jine",8);
    ProductTableIndex.insert("comment",9);

    m_tw->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;}");

}

LookFactoryContract::~LookFactoryContract()
{
    delete ui;
}


void LookFactoryContract::LoadData(QString Contract_id){
    MysqlOperate *m_MysqlOperate = MysqlOperate::getInstance();
    QVector<QMap<QString,QString>> data;
    MakeConditions makeconditions("factory_contract");
    makeconditions.AddEqual("contract_id",Contract_id);
    m_MysqlOperate->Find(makeconditions,data);
    if(data.size() == 0 ) return;
    MysqlTableConfig *m_TableConfig = MysqlTableConfig::getInstance();

    QSet<QString> columns = m_TableConfig->get_table_columns("factory_contract");
    QMap<QString,QString>::const_iterator itset;
    for (itset = data[0].constBegin(); itset != data[0].constEnd(); ++itset) {
        { //检测是不是lineedit
              QString objectName = "LineEdit_" + itset.key();
              QLineEdit *lineEdit = this->findChild<QLineEdit *>(objectName);
              if( lineEdit != nullptr){
                  lineEdit->setText(itset.value());
                  continue;
              }
        }
        { //检测是不是dateEdit_
              QString objectName = "dateEdit_" + itset.key();
              QDateEdit *object = this->findChild<QDateEdit *>(objectName);
              if( object != nullptr){
                  qDebug()<< itset.value();
                  object->setDate(QDate::fromString(itset.value(),"yyyy-MM-dd"));
                  continue;
              }
        }
    }
    ui->LineEdit_jia_name_2->setText( ui->LineEdit_jia_name->text());
    ui->LineEdit_yi_name_2->setText( ui->LineEdit_yi_name->text());

    // 加载 widget
    MakeConditions conditions_product("factory_product");
    conditions_product.AddEqual("contract_id",Contract_id);
    m_MysqlOperate->Find(conditions_product, data);
    QTableWidget *m_tw = ui->table_product;
    m_tw->clearContents();
    m_tw->setRowCount(data.size());
    for(int i = 0;i<data.size();i++){
        QMap<QString,int>::const_iterator itmap;
        QMap<QString,QString> &rowdata = data[i];
        for (itmap = ProductTableIndex.constBegin(); itmap != ProductTableIndex.constEnd(); ++itmap) {
            m_tw->setItem(i, itmap.value(),new QTableWidgetItem(rowdata.value(itmap.key())));
        }
    }
}
