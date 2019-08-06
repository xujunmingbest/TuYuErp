#include "kehucontract.h"
#include "ui_kehucontract.h"

KeHuContract::KeHuContract(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeHuContract)
{
    ui->setupUi(this);

    m_MysqlOperate = MysqlOperate::getInstance();
    //tablewidget提添加字段
    QTableWidget *m_tw = ui->table_product;
    m_tw->setColumnCount(9);
    m_tw->setRowCount(0);
    QStringList headers;
    headers << SS("产品名称") << SS("出口货号") << SS("规格尺寸")<< SS("图片")
            << SS("数量") <<  SS("单位") <<SS("单价") << SS("金额")
            << SS("删除");
    m_tw->setHorizontalHeaderLabels(headers);

    ProductTableIndex.insert("name",0);
    ProductTableIndex.insert("chukouhuohao",1);
    ProductTableIndex.insert("spac",2);
    ProductTableIndex.insert("imageurl",3);
    ProductTableIndex.insert("shuliang",4);
    ProductTableIndex.insert("danwei",5);
    ProductTableIndex.insert("danjia",6);
    ProductTableIndex.insert("jine",7);

    m_tw->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;}");

}

KeHuContract::~KeHuContract()
{
    delete ui;
}


void KeHuContract::SetMode(e_mode mode){
    m_mode = mode;
    switch(m_mode){
     case e_mode::ADD:
        ui->pushButton->setText(SS("添加合同"));
        ui->pushButton->setVisible(true);
        ui->LineEdit_contract_id->setReadOnly(false);
        break;
     case e_mode::EDIT:
        ui->pushButton->setText(SS("修改合同"));
        ui->pushButton->setVisible(true);
        ui->LineEdit_contract_id->setReadOnly(true);
        break;
     case e_mode::LOOK:
        ui->pushButton->setVisible(false);
        ui->LineEdit_contract_id->setReadOnly(true);
        break;
    }
}



void KeHuContract::DeleteProduct(){
    QTableWidget *m_tw = ui->table_product;
    QPushButton *btn = (QPushButton *)sender();
    m_tw->removeRow(btn->property("Row").toInt());
    for(int i=0;i<m_tw->rowCount();i++){
        m_tw->cellWidget(i,8)->setProperty("Row",i);
    }
}

void KeHuContract::ButtonSlot(){
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
#include "global.h"

#include "MysqlTableConfig/MysqlTableConfig.h"
void KeHuContract::AddContract(){
    if( ui->LineEdit_contract_id->text().trimmed().length() == 0) {
        QMessageBox::about(NULL, SS("错误提示"), SS("合同编号不能为空"));
        return ;
    }
    MysqlTableConfig * m_TableConfig  = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("kehu_contract");
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
    if( !m_MysqlOperate->Insert("kehu_contract",Contractdata)) Success = false;

    for(int i=0;i<row;i++){
        QMap<QString,QString> mapdata;
        QMap<QString,int>::const_iterator itmap;
        QString name = m_tw->item(i,0)->text().trimmed();
        if(name.length() == 0 ) continue;

        mapdata.insert("contract_id",ui->LineEdit_contract_id->text().trimmed());
        for (itmap = ProductTableIndex.constBegin(); itmap != ProductTableIndex.constEnd(); ++itmap) {
            if(itmap.key() == "imageurl"){
                ClickQlabel *label =  (ClickQlabel *)m_tw->cellWidget(i,3);
                QString UUid  =label->property("imageurl").toString();
                mapdata.insert(itmap.key(),UUid);
                continue;
            }else if(itmap.key() == "shuliang"){
                QLineEdit *LineEdit =  (QLineEdit *)m_tw->cellWidget(i,4);
                mapdata.insert(itmap.key(),LineEdit->text());
                continue;
            }else if(itmap.key() == "jine"){
                QLineEdit *LineEdit =  (QLineEdit *)m_tw->cellWidget(i,7);
                mapdata.insert(itmap.key(),LineEdit->text());
                continue;
            }
            mapdata.insert(itmap.key(),m_tw->item(i,itmap.value())->text().trimmed());
        }
        if( !m_MysqlOperate->Insert("kehu_product",mapdata) ) Success = false;
    }

    if( Success == false){
        m_MysqlOperate->Rollback();
        QMessageBox::about(NULL, SS("错误提示"), SS("合同添加失败"));
    }else{
        m_MysqlOperate->Commit();
        QMessageBox::about(NULL, SS("提示"), SS("合同添加成功"));
    }
}
#include <QMdiArea>
#include <QMdiSubWindow>
void KeHuContract::EditContract(){
    m_MysqlOperate->Transaction();
    MysqlTableConfig * m_TableConfig  = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("zhixiang_contract");
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

    //删除全部产品
    QString contract_id = ui->LineEdit_contract_id->text().trimmed();
    bool success = true;
    MakeConditions cds_product("zhixiang_product");
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
        if( !m_MysqlOperate->Insert("zhixiang_product",mapdata) ) success = false;
    }

    MakeConditions cds_contract("zhixiang_contract");
    cds_contract.AddEqual("contract_id",contract_id).AddEqual("version",version);
    Contractdata.insert("version",version);
    if( ! m_MysqlOperate->Update(cds_contract,Contractdata) ) success = false;

    if( success == false){
        m_MysqlOperate->Rollback();
        QMessageBox::about(NULL, SS("错误提示"), SS("合同更新失败"));
    }else{
        m_MysqlOperate->Commit();
        QMessageBox::about(NULL, SS("提示"), SS("合同更新成功"));
        QMdiArea *mdiArea  = ((QMdiSubWindow*)this->parent())->mdiArea();
        mdiArea->closeActiveSubWindow();
    }

}

#include "global.h"
void KeHuContract::AddLine(){
    QTableWidget *m_tw = ui->table_product;
    int rowCount = m_tw->rowCount();
    m_tw->setRowCount(rowCount+1);

    QPushButton *pBtn = new QPushButton(SS("删除"));
    pBtn->setProperty("Row",rowCount);
    connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteProduct()));
    // 在QTableWidget中添加控件
    m_tw->setCellWidget(rowCount,8,pBtn);
    for(int i=0;i<8;i++){
        if( i == 3){//图片
            ClickQlabel *  label = new ClickQlabel(SS("点击导入图片"));
            connect(label, SIGNAL(clicked()), this, SLOT(LoadImage()));
            m_tw->setCellWidget(rowCount,i,label);
            m_tw->setColumnWidth(i,150);
            continue;
        }
        if( i == 4){//数量
            QLineEdit *LineEdit = new QLineEdit("");
            m_tw->setCellWidget(rowCount,i,LineEdit);
            QIntValidator *Validator =new QIntValidator(0, 10000000);
            LineEdit->setValidator(Validator);
            continue;
        }
        if( i == 7){//数量
            QLineEdit *LineEdit = new QLineEdit("");
            m_tw->setCellWidget(rowCount,i,LineEdit);
            QDoubleValidator *Validator =new QDoubleValidator(0, 10000000,3);
            LineEdit->setValidator(Validator);
            continue;
        }
       m_tw->setItem(rowCount,i,new QTableWidgetItem(""));
    }
}

#include <QFileDialog>
#include "picture_client/qtclient.h"

void KeHuContract::LoadImage(){

     if( m_mode == e_mode::LOOK) return;
     ClickQlabel *label = (ClickQlabel *)sender();

     QStringList    fileNameList;
     QString fileName0;
     QFileDialog* fd = new QFileDialog(this);//创建对话框
     fd->resize(240,320);    //设置显示的大小
     fd->setNameFilter( ("png(*.png);;jpg(*.jpg);;jpeg(*.jpeg)")); //设置文件过滤器
     fd->setViewMode(QFileDialog::List);  //设置浏览模式,有列表（list） 模式和 详细信息（detail）两种方式
     if ( fd->exec() == QDialog::Accepted )   //如果成功的执行
     {
         fileNameList = fd->selectedFiles();      //返回文件列表的名称
         QString fileName0 = fileNameList[0];            //取第一个文件名
         //上传图片
         QtClient client;
         client.UploadData(fileName0);
         if(client.Success){
             //删除原来的
             //QString oldUUid  =label->property("imageurl").toString();
             //if( oldUUid.length() > 0){
             //       client.DeleteData(oldUUid);
             //}
             label->setProperty("imageurl",client.uuid);
             QPixmap fitpixmap = QPixmap(fileName0).scaled(150, 120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
             label->setPixmap(fitpixmap);
         }else{
             QMessageBox::about(NULL, SS("错误提示"), SS("图片上传失败"));

         }
     }
     else fd->close();
}

bool KeHuContract::LoadData(QString ContractId){
    MakeConditions cd_contract("zhixiang_contract");
    cd_contract.AddEqual("contract_id",ContractId);
    MakeConditions cd_product("zhixiang_product");
    cd_product.AddEqual("contract_id",ContractId);


    QMap<QString,QString> data_contract;
    if( !m_MysqlOperate->Get(cd_contract,data_contract) ) return false;

    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("zhixiang_contract");

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
        m_tw->setCellWidget(i,7,pBtn);

    }
    return true;
}



void KeHuContract::HeJiShuLiang(){
    QTableWidget *m_tw = ui->table_product;
    int row = m_tw->rowCount();

    for(int i=0;i<row;i++){
        QMap<QString,QString> mapdata;
        QMap<QString,int>::const_iterator itmap;
        QString name = m_tw->item(i,0)->text().trimmed();
        if(name.length() == 0 ) continue;


    }

}
void KeHuContract::HeJiJine(){


}

