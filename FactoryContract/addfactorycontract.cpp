#include "addfactorycontract.h"
#include "ui_addfactorycontract.h"
#include "MysqlOperate/mysqloperate.h"
#include <QMessageBox>

AddFactoryContract::AddFactoryContract(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFactoryContract)
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
             <<SS("金额（元）") <<  SS("备注") <<SS("操作");
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

AddFactoryContract::~AddFactoryContract()
{
    delete ui;
}

void AddFactoryContract::TianJiaProduct(){
    QTableWidget *m_tw = ui->table_product;
    int row = m_tw->rowCount();
    m_tw->setRowCount(row+1);
    m_tw->setItem(row,0,new QTableWidgetItem(ui->LineEdit_name->text().trimmed()));
    m_tw->setItem(row,1,new QTableWidgetItem(ui->LineEdit_color->text().trimmed()));
    m_tw->setItem(row,2,new QTableWidgetItem(ui->LineEdit_spec->text().trimmed()));
    m_tw->setItem(row,3,new QTableWidgetItem(ui->LineEdit_maogao->text().trimmed()));
    m_tw->setItem(row,4,new QTableWidgetItem(ui->LineEdit_menfu->text().trimmed()));
    m_tw->setItem(row,5,new QTableWidgetItem(ui->LineEdit_danwei->text().trimmed()));
    m_tw->setItem(row,6,new QTableWidgetItem(ui->LineEdit_shuliang->text().trimmed()));
    m_tw->setItem(row,7,new QTableWidgetItem(ui->LineEdit_danjia->text().trimmed()));
    m_tw->setItem(row,8,new QTableWidgetItem(ui->LineEdit_jine->text().trimmed()));
    m_tw->setItem(row,9,new QTableWidgetItem(ui->LineEdit_comment->text().trimmed()));
    QPushButton *pBtn = new QPushButton(SS("删除"));
    pBtn->setProperty("Row",row);
    connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteProduct()));
    // 在QTableWidget中添加控件
    m_tw->setCellWidget(row,10,pBtn);

}

void AddFactoryContract::DeleteProduct(){
   QTableWidget *m_tw = ui->table_product;
   QPushButton *btn = (QPushButton *)sender();
   m_tw->removeRow(btn->property("Row").toInt());
   for(int i=0;i<m_tw->rowCount();i++){
       m_tw->cellWidget(i,10)->setProperty("Row",i);
   }
}

#include "MysqlTableConfig/MysqlTableConfig.h"
void AddFactoryContract::AddContract(){
   if( ui->LineEdit_contract_id->text().trimmed().length() == 0) {
       QMessageBox::about(NULL, SS("错误提示"), "合同编号不能为空");
       return ;
   }
   MysqlTableConfig * m_TableConfig  = MysqlTableConfig::getInstance();
   MysqlOperate * m_MysqlOperate = MysqlOperate::getInstance();
   QSet<QString> columns = m_TableConfig->get_table_columns("factory_contract");
   QMap<QString,QString> Contractdata;
   QSet<QString>::const_iterator itset;
   for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
       { //检测是不是lineedit
             QString objectName = "LineEdit_" + *itset;
             QLineEdit *lineEdit = this->findChild<QLineEdit *>(objectName);
             if( lineEdit != nullptr){
                 Contractdata.insert(*itset,lineEdit->text().trimmed());
                 continue;
             }
       }
       { //检测是不是datetime
             QString objectName = "dateEdit_" + *itset;
             QDateEdit *object = this->findChild<QDateEdit *>(objectName);
             if( object != nullptr){
                 Contractdata.insert(*itset,object->text().trimmed());
                 continue;
             }
       }
   }
   if( !m_MysqlOperate->Insert("factory_contract",Contractdata) ){
       return ;
   }

   //添加product
   QTableWidget *m_tw = ui->table_product;
   int row = m_tw->rowCount();

   for(int i=0;i<row;i++){
       QMap<QString,QString> mapdata;
       QMap<QString,int>::const_iterator itmap;
       mapdata.insert("contract_id",ui->LineEdit_contract_id->text().trimmed());
       for (itmap = ProductTableIndex.constBegin(); itmap != ProductTableIndex.constEnd(); ++itmap) {
           mapdata.insert(itmap.key(),m_tw->item(i,itmap.value())->text());
       }
       m_MysqlOperate->Insert("factory_product",mapdata);
   }

}
