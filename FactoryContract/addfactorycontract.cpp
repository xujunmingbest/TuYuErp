#include "addfactorycontract.h"
#include "ui_addfactorycontract.h"
#include "MysqlOperate/mysqloperate.h"
#include <QMessageBox>

AddFactoryContract::AddFactoryContract(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFactoryContract)
{
    ui->setupUi(this);



    m_MysqlOperate = MysqlOperate::getInstance();
    //tablewidget提添加字段
    QTableWidget *m_tw = ui->table_product;
    m_tw->setColumnCount(11);
    m_tw->setRowCount(0);
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

void AddFactoryContract::SetMode(e_mode mode){
    m_mode = mode;
    switch(m_mode){
     case e_mode::ADD:
        ui->pushButton->setText(SS("添加面料合同"));
        ui->pushButton->setVisible(true);
        ui->LineEdit_contract_id->setReadOnly(false);
        break;
     case e_mode::EDIT:
        ui->pushButton->setText(SS("修改面料合同"));
        ui->pushButton->setVisible(true);
        ui->LineEdit_contract_id->setReadOnly(true);
        break;
     case e_mode::LOOK:
        ui->pushButton->setVisible(false);
        ui->LineEdit_contract_id->setReadOnly(true);
        break;
    }
}


//void AddFactoryContract::TianJiaProduct(){
//    QTableWidget *m_tw = ui->table_product;
//    int row = m_tw->rowCount();
//    m_tw->setRowCount(row+1);
//    m_tw->setItem(row,0,new QTableWidgetItem(ui->LineEdit_name->text().trimmed()));
//    m_tw->setItem(row,1,new QTableWidgetItem(ui->LineEdit_color->text().trimmed()));
//    m_tw->setItem(row,2,new QTableWidgetItem(ui->LineEdit_spec->text().trimmed()));
//    m_tw->setItem(row,3,new QTableWidgetItem(ui->LineEdit_maogao->text().trimmed()));
//    m_tw->setItem(row,4,new QTableWidgetItem(ui->LineEdit_menfu->text().trimmed()));
//    m_tw->setItem(row,5,new QTableWidgetItem(ui->LineEdit_danwei->text().trimmed()));
//    m_tw->setItem(row,6,new QTableWidgetItem(ui->LineEdit_shuliang->text().trimmed()));
//    m_tw->setItem(row,7,new QTableWidgetItem(ui->LineEdit_danjia->text().trimmed()));
//    m_tw->setItem(row,8,new QTableWidgetItem(ui->LineEdit_jine->text().trimmed()));
//    m_tw->setItem(row,9,new QTableWidgetItem(ui->LineEdit_comment->text().trimmed()));
//    QPushButton *pBtn = new QPushButton(SS("删除"));
//    pBtn->setProperty("Row",row);
//    connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteProduct()));
//    // 在QTableWidget中添加控件
//    m_tw->setCellWidget(row,10,pBtn);
//
//}

void AddFactoryContract::DeleteProduct(){
    QTableWidget *m_tw = ui->table_product;
    QPushButton *btn = (QPushButton *)sender();
    m_tw->removeRow(btn->property("Row").toInt());
    for(int i=0;i<m_tw->rowCount();i++){
        m_tw->cellWidget(i,10)->setProperty("Row",i);
    }
}

void AddFactoryContract::ButtonSlot(){
    switch(m_mode){
    case e_mode::ADD:
        AddContract();
        break;
    case e_mode::EDIT:
        EditContract();
        break;
    default :
        break;
    }

}


#include "MysqlTableConfig/MysqlTableConfig.h"
void AddFactoryContract::AddContract(){
    if( ui->LineEdit_contract_id->text().trimmed().length() == 0) {
        QMessageBox::about(NULL, SS("错误提示"), SS("合同编号不能为空"));
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

    //添加product
    QTableWidget *m_tw = ui->table_product;
    int row = m_tw->rowCount();
    bool Success = true;
    m_MysqlOperate->Transaction();
    if( !m_MysqlOperate->Insert("factory_contract",Contractdata)) Success = false;

    for(int i=0;i<row;i++){
        QMap<QString,QString> mapdata;
        QMap<QString,int>::const_iterator itmap;
        QString name = m_tw->item(i,0)->text().trimmed();
        if(name.length() == 0 ) continue;
        mapdata.insert("contract_id",ui->LineEdit_contract_id->text().trimmed());
        for (itmap = ProductTableIndex.constBegin(); itmap != ProductTableIndex.constEnd(); ++itmap) {
            mapdata.insert(itmap.key(),m_tw->item(i,itmap.value())->text().trimmed());
        }
        if( !m_MysqlOperate->Insert("factory_product",mapdata) ) Success = false;
    }

    if( Success == false){
        m_MysqlOperate->Rollback();
        QMessageBox::about(NULL, SS("错误提示"), SS("面料合同添加失败"));
    }else{
        m_MysqlOperate->Commit();
        QMessageBox::about(NULL, SS("提示"), SS("面料合同添加成功"));
    }
}
#include <QMdiArea>
#include <QMdiSubWindow>
void AddFactoryContract::EditContract(){

    MysqlTableConfig * m_TableConfig  = MysqlTableConfig::getInstance();
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


    //添加product
    QTableWidget *m_tw = ui->table_product;
    int row = m_tw->rowCount();

    m_MysqlOperate->Transaction();
    //删除全部产品
    QString contract_id = ui->LineEdit_contract_id->text().trimmed();
    bool success = true;
    MakeConditions cds_product("factory_product");
    cds_product.AddEqual("contract_id",contract_id);
    if( !m_MysqlOperate->Delete(cds_product) ) success = false;
    for(int i=0;i<row;i++){
        QMap<QString,QString> mapdata;
        QMap<QString,int>::const_iterator itmap;
        QString name = m_tw->item(i,0)->text().trimmed();
        if(name.length() == 0 ) continue;
        mapdata.insert("contract_id",ui->LineEdit_contract_id->text().trimmed());
        for (itmap = ProductTableIndex.constBegin(); itmap != ProductTableIndex.constEnd(); ++itmap) {
            mapdata.insert(itmap.key(),m_tw->item(i,itmap.value())->text().trimmed());
        }
        if( !m_MysqlOperate->Insert("factory_product",mapdata) ) success = false;
    }

    MakeConditions cds_contract("factory_contract");
    cds_contract.AddEqual("contract_id",contract_id).AddEqual("version",version);
    Contractdata.insert("version",version);
    if( ! m_MysqlOperate->Update(cds_contract,Contractdata) ) success = false;

    if( success == false){
        m_MysqlOperate->Rollback();
        QMessageBox::about(NULL, SS("错误提示"), SS("面料合同更新失败"));
    }else{
        m_MysqlOperate->Commit();
        QMessageBox::about(NULL, SS("提示"), SS("面料合同更新成功"));
        QMdiArea *mdiArea  = ((QMdiSubWindow*)this->parent())->mdiArea();
        mdiArea->closeActiveSubWindow();
    }

}


void AddFactoryContract::AddLine(){
    QTableWidget *m_tw = ui->table_product;
    int rowCount = m_tw->rowCount();
    m_tw->setRowCount(rowCount+1);

    QPushButton *pBtn = new QPushButton(SS("删除"));
    pBtn->setProperty("Row",rowCount);
    connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteProduct()));
    // 在QTableWidget中添加控件
    m_tw->setCellWidget(rowCount,10,pBtn);
    for(int i=0;i<10;i++){
       m_tw->setItem(rowCount,i,new QTableWidgetItem(""));
    }
}


bool AddFactoryContract::LoadData(QString ContractId){
    MakeConditions cd_contract("factory_contract");
    cd_contract.AddEqual("contract_id",ContractId);
    MakeConditions cd_product("factory_product");
    cd_product.AddEqual("contract_id",ContractId);


    QMap<QString,QString> data_contract;
    if( !m_MysqlOperate->Get(cd_contract,data_contract) ) return false;

    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("factory_contract");

    QSet<QString>::const_iterator itset;
    for (itset = columns.constBegin(); itset != columns.constEnd(); ++itset) {
        {
            QString objectName = "LineEdit_" + *itset;
            QLineEdit *object = this->findChild<QLineEdit *>(objectName);
            if( object != nullptr){
                object->setText(data_contract.value(*itset));
                continue;
            }
        }
        {
            QString objectName = "dateEdit_" + *itset;
            QDateEdit *object = this->findChild<QDateEdit *>(objectName);
            if( object != nullptr){
                object->setDate(QDate::fromString(data_contract.value(*itset),"yyyy-MM-dd"));
                continue;
            }
        }
    }
    version = data_contract.value("version");
    ui->LineEdit_jia_name_2->setText(ui->LineEdit_jia_name->text());
    ui->LineEdit_yi_name_2->setText(ui->LineEdit_yi_name->text());

    QVector<QMap<QString,QString>> data_product;
    if( !m_MysqlOperate->Find(cd_product,data_product) ) return false;

    QTableWidget *m_tw = ui->table_product;
    m_tw->clearContents();
    m_tw->setRowCount(data_product.size());
    for(int i = 0;i<data_product.size();i++){
        QMap<QString,int>::const_iterator itmap;
        QMap<QString,QString> &rowdata = data_product[i];
        for (itmap = ProductTableIndex.constBegin(); itmap != ProductTableIndex.constEnd(); ++itmap) {
            m_tw->setItem(i, itmap.value(),new QTableWidgetItem(rowdata.value(itmap.key())));
        }
        QPushButton *pBtn = new QPushButton(SS("删除"));
        pBtn->setProperty("Row",i);
        connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteProduct()));
        // 在QTableWidget中添加控件
        m_tw->setCellWidget(i,10,pBtn);
    }
    return true;
}
