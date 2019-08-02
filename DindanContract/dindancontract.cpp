#include "dindancontract.h"
#include "ui_dindancontract.h"

ClickQlabel::ClickQlabel(QWidget *parent, Qt::WindowFlags f):
    QLabel(parent,f)
{
}
ClickQlabel::ClickQlabel(const QString &text, QWidget *parent, Qt::WindowFlags f):
    QLabel(text,parent,f)
{
}
void ClickQlabel::mouseReleaseEvent(QMouseEvent * ev)
{
    Q_UNUSED(ev)
    emit clicked();
}


DindanContract::DindanContract(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DindanContract)
{
    ui->setupUi(this);
    m_MysqlOperate = MysqlOperate::getInstance();

    {
        //tablewidget提添加字段
        QTableWidget *m_tw = ui->table_1;
        m_tw->setColumnCount(9);
        m_tw->setRowCount(0);
        QStringList headers;
        headers << SS("品名") << SS("面料") << SS("出口货号")<< SS("图片")
                <<SS("数量") <<  SS("尺寸") <<SS("用料") << SS("印刷方式及确认情况")
                 <<SS("删除");
        m_tw->setHorizontalHeaderLabels(headers);
        TableIndex_1.insert("name",0);
        TableIndex_1.insert("mianliao",1);
        TableIndex_1.insert("chukouhuohao",2);
        TableIndex_1.insert("imageurl",3);
        TableIndex_1.insert("shuliang",4);
        TableIndex_1.insert("chicun",5);
        TableIndex_1.insert("yongliao",6);
        TableIndex_1.insert("confirm",7);
        m_tw->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;}");
    }

    {
        //tablewidget提添加字段
        QTableWidget *m_tw = ui->table_2;
        m_tw->setColumnCount(11);
        m_tw->setRowCount(0);
        QStringList headers;
        headers << SS("编号") << SS("面辅料名称") << SS("规格")<< SS("裁片")
                <<SS("单件用料") <<  SS("损耗") <<SS("合计用料") << SS("订单数")
                <<SS("总数") << SS("单位") <<SS("删除");
        m_tw->setHorizontalHeaderLabels(headers);
        TableIndex_2.insert("bianhao",0);
        TableIndex_2.insert("name",1);
        TableIndex_2.insert("spec",2);
        TableIndex_2.insert("caipian",3);
        TableIndex_2.insert("single_use",4);
        TableIndex_2.insert("loss",5);
        TableIndex_2.insert("total_use",6);
        TableIndex_2.insert("order_number",7);
        TableIndex_2.insert("total_number",8);
        TableIndex_2.insert("danwei",9);
        m_tw->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;}");
    }

    {
        //tablewidget提添加字段
        QTableWidget *m_tw = ui->table_3;
        m_tw->setColumnCount(3);
        m_tw->setRowCount(0);
        QStringList headers;
        headers << SS("款式图：") << SS("数量")<<SS("删除");
        m_tw->setHorizontalHeaderLabels(headers);
        TableIndex_3.insert("imageurl",0);
        TableIndex_3.insert("shuliang",1);
        m_tw->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;}");
    }
}

DindanContract::~DindanContract()
{
    delete ui;
}


void DindanContract::AddLine_1(){
    QTableWidget *m_tw = ui->table_1;
    int rowCount = m_tw->rowCount();
    m_tw->setRowCount(rowCount+1);

    int columncount = m_tw->columnCount();

    QPushButton *pBtn = new QPushButton(SS("删除"));
    pBtn->setProperty("Row",rowCount);
    connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteProduct_1()));
     //在QTableWidget中添加控件
    m_tw->setCellWidget(rowCount,columncount-1,pBtn);

    for(int i=0;i<columncount-1;i++){
        if(i == 3 ){
           ClickQlabel *  label = new ClickQlabel(SS("点击导入图片"));
           connect(label, SIGNAL(clicked()), this, SLOT(Loadimage_1()));
           m_tw->setCellWidget(rowCount,i,label);
           m_tw->setColumnWidth(i,150);
        }else{
           m_tw->setItem(rowCount,i,new QTableWidgetItem(""));
        }
    }
}

void DindanContract::AddLine_2(){
    QTableWidget *m_tw = ui->table_2;
    int rowCount = m_tw->rowCount();
    m_tw->setRowCount(rowCount+1);

    int columncount = m_tw->columnCount();

    QPushButton *pBtn = new QPushButton(SS("删除"));
    pBtn->setProperty("Row",rowCount);
    connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteProduct_2()));
     //在QTableWidget中添加控件
    m_tw->setCellWidget(rowCount,columncount-1,pBtn);

    for(int i=0;i<columncount-1;i++){
           m_tw->setItem(rowCount,i,new QTableWidgetItem(""));
    }

}
void DindanContract::AddLine_3(){
    QTableWidget *m_tw = ui->table_3;
    int rowCount = m_tw->rowCount();
    m_tw->setRowCount(rowCount+1);

    int columncount = m_tw->columnCount();

    QPushButton *pBtn = new QPushButton(SS("删除"));
    pBtn->setProperty("Row",rowCount);
    connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteProduct_3()));
     //在QTableWidget中添加控件
    m_tw->setCellWidget(rowCount,columncount-1,pBtn);

    for(int i=0;i<columncount-1;i++){
        if(i == 0 ){
           ClickQlabel *  label = new ClickQlabel(SS("点击导入图片"));
           connect(label, SIGNAL(clicked()), this, SLOT(Loadimage_1()));
           m_tw->setCellWidget(rowCount,i,label);
           m_tw->setColumnWidth(i,150);
        }else{
           m_tw->setItem(rowCount,i,new QTableWidgetItem(""));
        }
    }

}

#include <QFileDialog>
#include "picture_client/qtclient.h"
void DindanContract::Loadimage_1(){
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


bool DindanContract::add_table_1(){

    QTableWidget *m_tw = ui->table_1;
    int row = m_tw->rowCount();

    for(int i=0;i<row;i++){
        QMap<QString,QString> mapdata;
        QMap<QString,int>::const_iterator itmap;
        QString name = m_tw->item(i,0)->text().trimmed();
        if(name.length() == 0 ) continue;
        mapdata.insert("contract_id",ui->LineEdit_contract_id->text().trimmed());
        for (itmap = TableIndex_1.constBegin(); itmap != TableIndex_1.constEnd(); ++itmap) {
            if(itmap.key() == "imageurl"){
                ClickQlabel *label =  (ClickQlabel *)m_tw->cellWidget(i,3);
                QString UUid  =label->property("imageurl").toString();
                mapdata.insert(itmap.key(),UUid);
                continue;
            }
            mapdata.insert(itmap.key(),m_tw->item(i,itmap.value())->text().trimmed());

        }
        if( !m_MysqlOperate->Insert("dingdan_table_1",mapdata) ) return false;
    }
    return true;
}

bool DindanContract::add_table_2(){

    QTableWidget *m_tw = ui->table_2;
    int row = m_tw->rowCount();

    for(int i=0;i<row;i++){
        QMap<QString,QString> mapdata;
        QMap<QString,int>::const_iterator itmap;
        QString name = m_tw->item(i,1)->text().trimmed();
        if(name.length() == 0 ) continue;
        mapdata.insert("contract_id",ui->LineEdit_contract_id->text().trimmed());
        for (itmap = TableIndex_2.constBegin(); itmap != TableIndex_2.constEnd(); ++itmap) {
            mapdata.insert(itmap.key(),m_tw->item(i,itmap.value())->text().trimmed());

        }
        if( !m_MysqlOperate->Insert("dingdan_table_2",mapdata) ) return false;
    }

    return true;
}
bool DindanContract::add_table_3(){
    QTableWidget *m_tw = ui->table_3;
    int row = m_tw->rowCount();

    for(int i=0;i<row;i++){
        QMap<QString,QString> mapdata;
        QMap<QString,int>::const_iterator itmap;
        mapdata.insert("contract_id",ui->LineEdit_contract_id->text().trimmed());
        for (itmap = TableIndex_3.constBegin(); itmap != TableIndex_3.constEnd(); ++itmap) {
            if(itmap.key() == "imageurl"){
                ClickQlabel *label =  (ClickQlabel *)m_tw->cellWidget(i,0);
                QString UUid  =label->property("imageurl").toString();
                mapdata.insert(itmap.key(),UUid);
                continue;
            }
            mapdata.insert(itmap.key(),m_tw->item(i,itmap.value())->text().trimmed());

        }
        if( !m_MysqlOperate->Insert("dingdan_table_3",mapdata) ) return false;
    }


    return true;
}

#include "MysqlTableConfig/MysqlTableConfig.h"
bool DindanContract::add_contract(){
    if( ui->LineEdit_contract_id->text().trimmed().length() == 0) {
        QMessageBox::about(NULL, SS("错误提示"), SS("合同编号不能为空"));
        return false;
    }
    MysqlTableConfig * m_TableConfig  = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("dingdan_contract");
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
        { //检测是不是textEdit
            QString objectName = "textEdit_" + *itset;
            QTextEdit *object = this->findChild<QTextEdit *>(objectName);
            if( object != nullptr){
                Contractdata.insert(*itset,object->toPlainText().trimmed());
                continue;
            }
        }

    }

    if( !m_MysqlOperate->Insert("dingdan_contract",Contractdata)) return false;

    return true;
}



void DindanContract::SetMode(e_mode mode){
    m_mode = mode;
    switch(m_mode){
     case e_mode::ADD:
        ui->pushButton->setText(SS("添加订单"));
        ui->pushButton->setVisible(true);
        ui->LineEdit_contract_id->setReadOnly(false);
        break;
     case e_mode::EDIT:
        ui->pushButton->setText(SS("修改订单"));
        ui->pushButton->setVisible(true);
        ui->LineEdit_contract_id->setReadOnly(true);
        break;
     case e_mode::LOOK:
        ui->pushButton->setVisible(false);
        ui->LineEdit_contract_id->setReadOnly(true);
        break;
    }
}

void DindanContract::ButtonSlot(){
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
#include <QMdiArea>
#include <QMdiSubWindow>
void DindanContract::AddContract(){
    m_MysqlOperate->Transaction();
    bool Success = true;
    if( !add_contract() ) Success = false;
    if( !add_table_1() ) Success = false;
    if( !add_table_2() ) Success = false;
    if( !add_table_3() ) Success = false;


    if( Success == false){
        m_MysqlOperate->Rollback();
        QMap<int , QString>::const_iterator itmap;
        //图片撤销提交
        {
            QTableWidget *m_tw = ui->table_1;
            int row = m_tw->rowCount();

            for(int i=0;i<row;i++){
                ClickQlabel *label =  (ClickQlabel *)m_tw->cellWidget(i,3);
                QString UUid  =label->property("imageurl").toString();
                if(UUid.length() > 0 ){
                    QtClient client1;
                    client1.DeleteData(UUid);
                }
            }
        }
        {
            QTableWidget *m_tw = ui->table_3;
            int row = m_tw->rowCount();

            for(int i=0;i<row;i++){
                ClickQlabel *label =  (ClickQlabel *)m_tw->cellWidget(i,0);
                QString UUid  =label->property("imageurl").toString();
                if(UUid.length() > 0 ){
                    QtClient client1;
                    client1.DeleteData(UUid);
                }
            }
        }

        QMessageBox::about(NULL, SS("错误提示"), SS("订单添加失败"));
    }else{
        m_MysqlOperate->Commit();
        QMessageBox::about(NULL, SS("提示"), SS("订单添加成功"));
    }

}
void DindanContract::EditContract(){
    //总表更新


    //table1，2，3 重新删除并添加

    MysqlTableConfig * m_TableConfig  = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("dingdan_contract");
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
        { //检测是不是textEdit
            QString objectName = "textEdit_" + *itset;
            QTextEdit *object = this->findChild<QTextEdit *>(objectName);
            if( object != nullptr){
                Contractdata.insert(*itset,object->toPlainText().trimmed());
                continue;
            }
        }
    }

    m_MysqlOperate->Transaction();
    bool success = true;
    {
        //添加product
        QTableWidget *m_tw = ui->table_1;
        int row = m_tw->rowCount();

        //删除全部产品
        QString contract_id = ui->LineEdit_contract_id->text().trimmed();
        MakeConditions cds_product("dingdan_table_1");
        cds_product.AddEqual("contract_id",contract_id);
        if( !m_MysqlOperate->Delete(cds_product) ) success = false;
        for(int i=0;i<row;i++){
            QMap<QString,QString> mapdata;
            QMap<QString,int>::const_iterator itmap;
            QString name = m_tw->item(i,0)->text().trimmed();
            if(name.length() == 0 ) continue;
            mapdata.insert("contract_id",ui->LineEdit_contract_id->text().trimmed());
            for (itmap = TableIndex_1.constBegin(); itmap != TableIndex_1.constEnd(); ++itmap) {
                if(itmap.key() == "imageurl"){\
                    ClickQlabel *label =  (ClickQlabel *)m_tw->cellWidget(i,3);
                    QString UUid  =label->property("imageurl").toString();
                    mapdata.insert(itmap.key(),UUid);
                    continue;
                }

                mapdata.insert(itmap.key(),m_tw->item(i,itmap.value())->text().trimmed());
            }
            if( !m_MysqlOperate->Insert("dingdan_table_1",mapdata) ) success = false;
        }
    }

    {
        //添加product
        QTableWidget *m_tw = ui->table_2;
        int row = m_tw->rowCount();

        //删除全部产品
        QString contract_id = ui->LineEdit_contract_id->text().trimmed();
        MakeConditions cds_product("dingdan_table_2");
        cds_product.AddEqual("contract_id",contract_id);
        if( !m_MysqlOperate->Delete(cds_product) ) success = false;
        for(int i=0;i<row;i++){
            QMap<QString,QString> mapdata;
            QMap<QString,int>::const_iterator itmap;
            QString name = m_tw->item(i,1)->text().trimmed();
            if(name.length() == 0 ) continue;
            mapdata.insert("contract_id",ui->LineEdit_contract_id->text().trimmed());
            for (itmap = TableIndex_2.constBegin(); itmap != TableIndex_2.constEnd(); ++itmap) {
                mapdata.insert(itmap.key(),m_tw->item(i,itmap.value())->text().trimmed());
            }
            if( !m_MysqlOperate->Insert("dingdan_table_2",mapdata) ) success = false;
        }
    }

    {
        //添加product
        QTableWidget *m_tw = ui->table_3;
        int row = m_tw->rowCount();

        //删除全部产品
        QString contract_id = ui->LineEdit_contract_id->text().trimmed();
        MakeConditions cds_product("dingdan_table_3");
        cds_product.AddEqual("contract_id",contract_id);
        if( !m_MysqlOperate->Delete(cds_product) ) success = false;
        for(int i=0;i<row;i++){
            QMap<QString,QString> mapdata;
            QMap<QString,int>::const_iterator itmap;
            mapdata.insert("contract_id",ui->LineEdit_contract_id->text().trimmed());
            for (itmap = TableIndex_3.constBegin(); itmap != TableIndex_3.constEnd(); ++itmap) {
                if(itmap.key() == "imageurl"){\
                    ClickQlabel *label =  (ClickQlabel *)m_tw->cellWidget(i,0);
                    QString UUid  =label->property("imageurl").toString();
                    mapdata.insert(itmap.key(),UUid);
                    continue;
                }

                mapdata.insert(itmap.key(),m_tw->item(i,itmap.value())->text().trimmed());
            }
            if( !m_MysqlOperate->Insert("dingdan_table_3",mapdata) ) success = false;
        }
    }



    MakeConditions cds_contract("dingdan_contract");
    QString contract_id = ui->LineEdit_contract_id->text().trimmed();
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


void DindanContract::DeleteProduct_1(){
    QTableWidget *m_tw = ui->table_1;
    QPushButton *btn = (QPushButton *)sender();
    m_tw->removeRow(btn->property("Row").toInt());
    for(int i=0;i<m_tw->rowCount();i++){
        m_tw->cellWidget(i,m_tw->columnCount()-1)->setProperty("Row",i);
    }
}

void DindanContract::DeleteProduct_2(){
    QTableWidget *m_tw = ui->table_2;
    QPushButton *btn = (QPushButton *)sender();
    m_tw->removeRow(btn->property("Row").toInt());
    for(int i=0;i<m_tw->rowCount();i++){
        m_tw->cellWidget(i,m_tw->columnCount()-1)->setProperty("Row",i);
    }
}

void DindanContract::DeleteProduct_3(){
    QTableWidget *m_tw = ui->table_3;
    QPushButton *btn = (QPushButton *)sender();
    m_tw->removeRow(btn->property("Row").toInt());
    for(int i=0;i<m_tw->rowCount();i++){
        m_tw->cellWidget(i,m_tw->columnCount()-1)->setProperty("Row",i);
    }
}

#include "picture_client/msg_def.h"
bool DindanContract::LoadData(QString ContractId){
    MakeConditions cd_contract("dingdan_contract");
    cd_contract.AddEqual("contract_id",ContractId);
    MakeConditions cd_table_1("dingdan_table_1");
    cd_table_1.AddEqual("contract_id",ContractId);
    MakeConditions cd_table_2("dingdan_table_2");
    cd_table_2.AddEqual("contract_id",ContractId);
    MakeConditions cd_table_3("dingdan_table_3");
    cd_table_3.AddEqual("contract_id",ContractId);


    QMap<QString,QString> data_contract;
    if( !m_MysqlOperate->Get(cd_contract,data_contract) ) return false;

    MysqlTableConfig* m_TableConfig = MysqlTableConfig::getInstance();
    QSet<QString> columns = m_TableConfig->get_table_columns("dingdan_contract");

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
        { //检测是不是textEdit
            QString objectName = "textEdit_" + *itset;
            QTextEdit *object = this->findChild<QTextEdit *>(objectName);
            if( object != nullptr){
                object->setText(data_contract.value(*itset));
                continue;
            }
        }

    }

    version = data_contract.value("version");

    QVector<QMap<QString,QString>> data_table_1;
    if( !m_MysqlOperate->Find(cd_table_1,data_table_1) ) return false;

    QTableWidget *m_tw_1 = ui->table_1;
    m_tw_1->clearContents();
    m_tw_1->setRowCount(data_table_1.size());
    for(int i = 0;i<data_table_1.size();i++){
        QMap<QString,int>::const_iterator itmap;
        QMap<QString,QString> &rowdata = data_table_1[i];
        for (itmap = TableIndex_1.constBegin(); itmap != TableIndex_1.constEnd(); ++itmap) {
            if(itmap.key() == "imageurl"){
                 ClickQlabel *label = new ClickQlabel("点击添加图片");
                 connect(label, SIGNAL(clicked()), this, SLOT(Loadimage_1()));
                 m_tw_1->setCellWidget(i,3,label);
                 m_tw_1->setColumnWidth(3,150);
                 QtClient Client;
                 QString uuid=rowdata.value(itmap.key());
                 Client.DownData(uuid);
                 if(Client.Success ){
                       label->setProperty("imageurl",uuid);
                       QPixmap fitpixmap = QPixmap(save_dir + uuid).scaled(150, 120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                       label->setPixmap(fitpixmap);
                 }else{
                     return false;
                 }
                 continue;
            }
            m_tw_1->setItem(i, itmap.value(),new QTableWidgetItem(rowdata.value(itmap.key())));
        }
        QPushButton *pBtn = new QPushButton(SS("删除"));
        pBtn->setProperty("Row",i);
        connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteProduct_1()));
        // 在QTableWidget中添加控件
        m_tw_1->setCellWidget(i,8,pBtn);
    }


    QVector<QMap<QString,QString>> data_table_2;
    if( !m_MysqlOperate->Find(cd_table_2,data_table_2) ) return false;

    QTableWidget *m_tw_2 = ui->table_2;
    m_tw_2->clearContents();
    m_tw_2->setRowCount(data_table_2.size());
    for(int i = 0;i<data_table_2.size();i++){
        QMap<QString,int>::const_iterator itmap;
        QMap<QString,QString> &rowdata = data_table_2[i];
        for (itmap = TableIndex_2.constBegin(); itmap != TableIndex_2.constEnd(); ++itmap) {
            m_tw_2->setItem(i, itmap.value(),new QTableWidgetItem(rowdata.value(itmap.key())));
        }
        QPushButton *pBtn = new QPushButton(SS("删除"));
        pBtn->setProperty("Row",i);
        connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteProduct_2()));
        // 在QTableWidget中添加控件
        m_tw_2->setCellWidget(i,10,pBtn);
    }

    QVector<QMap<QString,QString>> data_table_3;
    if( !m_MysqlOperate->Find(cd_table_3,data_table_3) ) return false;
    QTableWidget *m_tw_3 = ui->table_3;
    m_tw_3->clearContents();
    m_tw_3->setRowCount(data_table_3.size());
    for(int i = 0;i<data_table_3.size();i++){
        QMap<QString,int>::const_iterator itmap;
        QMap<QString,QString> &rowdata = data_table_3[i];
        for (itmap = TableIndex_3.constBegin(); itmap != TableIndex_3.constEnd(); ++itmap) {
            if(itmap.key() == "imageurl"){
                 ClickQlabel *label = new ClickQlabel("点击添加图片");
                 connect(label, SIGNAL(clicked()), this, SLOT(Loadimage_1()));
                 m_tw_3->setCellWidget(i,0,label);
                 m_tw_3->setColumnWidth(0,150);
                 QtClient Client;
                 QString uuid=rowdata.value(itmap.key());
                 Client.DownData(uuid);
                 if(Client.Success ){
                       label->setProperty("imageurl",uuid);
                       QPixmap fitpixmap = QPixmap(save_dir + uuid).scaled(150, 120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                       label->setPixmap(fitpixmap);
                 }else{
                     return false;
                 }
                 continue;
            }
            m_tw_3->setItem(i, itmap.value(),new QTableWidgetItem(rowdata.value(itmap.key())));
        }
        QPushButton *pBtn = new QPushButton(SS("删除"));
        pBtn->setProperty("Row",i);
        connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteProduct_3()));
        // 在QTableWidget中添加控件
        m_tw_3->setCellWidget(i,2,pBtn);
    }


    return true;

}
