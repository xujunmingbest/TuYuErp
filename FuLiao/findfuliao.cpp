#include "findfuliao.h"
#include "ui_findfuliao.h"

findfuliao::findfuliao(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::findfuliao)
{
    ui->setupUi(this);
    m_MysqlOperate = MysqlOperate::getInstance();
    {
        QTableWidget *m_tb = ui->tableWidget_ruku;
        m_tb->setColumnCount(14+2);
        m_tb->setRowCount(0);
        m_tb->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QStringList head ;
        head << SS("入库单号") << SS("日期") << SS("辅料名称") <<SS("数量")<<
                SS("单位") << SS("明细") << SS("颜色") <<SS("规格")<<
                SS("其他费用") << SS("单价") << SS("总金额") <<SS("供应商")<<
                SS("结余数量") << SS("备注") << SS("编辑") << SS("删除");

        m_tb->setHorizontalHeaderLabels(head);
        m_tb->horizontalHeader()->setMinimumHeight(25);
        m_tb->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;font:11pt}");

        RukuTableIndex.insert("ruku_id",0);
        RukuTableIndex.insert("date",1);
        RukuTableIndex.insert("name",2);
        RukuTableIndex.insert("shuliang",3);
        RukuTableIndex.insert("danwei",4);
        RukuTableIndex.insert("detail",5);
        RukuTableIndex.insert("color",6);
        RukuTableIndex.insert("spec",7);
        RukuTableIndex.insert("otherfee",8);
        RukuTableIndex.insert("danjia",9);
        RukuTableIndex.insert("zongjine",10);
        RukuTableIndex.insert("gongyingshang",11);
        RukuTableIndex.insert("jieyu_shuliang",12);
        RukuTableIndex.insert("comment",13);
    }
    {
        QTableWidget *m_tb = ui->tableWidget_chuku;
        m_tb->setColumnCount(11);
        m_tb->setRowCount(0);
        m_tb->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QStringList head ;
        head << SS("入库单号") << SS("日期") << SS("数量") << SS("单位")
             << SS("明细") << SS("订单号") << SS("领取人") <<SS("目标去向")
             << SS("备注") << SS("编辑") << SS("删除");

        m_tb->setHorizontalHeaderLabels(head);
        m_tb->horizontalHeader()->setMinimumHeight(25);
        m_tb->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;font:11pt}");

        ChukuTableIndex.insert("ruku_id",0);
        ChukuTableIndex.insert("date",1);
        ChukuTableIndex.insert("shuliang",2);
        ChukuTableIndex.insert("danwei",3);
        ChukuTableIndex.insert("detail",4);
        ChukuTableIndex.insert("order_id",5);
        ChukuTableIndex.insert("lingquren",6);
        ChukuTableIndex.insert("targetto",7);
        ChukuTableIndex.insert("comment",8);
    }
}

findfuliao::~findfuliao()
{
    delete ui;
}

bool findfuliao::getConditions(MakeConditions &conditions){
    QString name = ui->lineEdit_name->text().trimmed();
    if(name.length() != 0){
        conditions.AddEqual("name",name);
    }
    QString BeginTime = ui->dateEdit_begindate->text();
    QString EndTime = ui->dateEdit_enddate->text();
    conditions.AddBetween("date",BeginTime,EndTime);
    return true;
}

bool findfuliao::getConditions_2(MakeConditions &conditions){

    QString name = ui->lineEdit_name_2->text().trimmed();
    if(name.length() != 0){
        conditions.AddEqual("name",name);
    }
    return true;
}


void findfuliao::shaixuanSlot(){
   TiaoZhuang(1);
}

void findfuliao::shaixuanSlot_2(){
   TiaoZhuang_2(1);
}

void findfuliao::TiaoZhuangSlot(){
    int page = ui->lineEdit_page->text().trimmed().toInt();
    TiaoZhuang(page);
}

void findfuliao::TiaoZhuangSlot_2(){
    int page = ui->lineEdit_page_2->text().trimmed().toInt();
    TiaoZhuang_2(page);
}

  void findfuliao::TiaoZhuang(int page){
      MakeConditions cd_ruku("fuliao_ruku");
      if( !getConditions(cd_ruku) ) return ;

      MysqlOperate* m_MysqlOperate = MysqlOperate::getInstance();
      int data_count = m_MysqlOperate->Count(cd_ruku);
      int total_page = data_count/PAGESIZE;
      if(data_count%PAGESIZE >0 ) total_page++;
      if(page < 1 ) page = 1;
      else if(page > total_page) page = total_page;
      QString pagetext = QStringLiteral("第 %1 页/ %2 页").arg(page).arg(total_page);
      ui->label_page->setText(pagetext);


      cd_ruku.AddPage(page);
      MysqlOperate * m_mysqlins= MysqlOperate::getInstance();
      QVector<QMap<QString,QString>> RuKuDatas;
      m_mysqlins->Find(cd_ruku,RuKuDatas);

      QTableWidget *m_tb = ui->tableWidget_ruku;
      m_tb->setRowCount(RuKuDatas.size());
      m_tb->clearContents();
      for(int i = 0;i<RuKuDatas.size();i++){
          QMap<QString,int>::const_iterator itmap;
          QMap<QString,QString> &rowdata = RuKuDatas[i];
          for (itmap = RukuTableIndex.constBegin(); itmap != RukuTableIndex.constEnd(); ++itmap) {
              m_tb->setItem(i, itmap.value(),new QTableWidgetItem(rowdata.value(itmap.key())));
          }
          {
              QPushButton *pBtn = new QPushButton(SS("编辑"));
              pBtn->setProperty("ruku_id",rowdata.value("ruku_id"));
              connect(pBtn, SIGNAL(clicked()), this, SLOT(EditRuKu()));
              m_tb->setCellWidget(i,14,pBtn);
          }
          {
              QPushButton *pBtn = new QPushButton(SS("删除"));
              pBtn->setProperty("ruku_id",rowdata.value("ruku_id"));
              pBtn->setProperty("row",i);
              connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteRuKu()));
              m_tb->setCellWidget(i,15,pBtn);
          }
      }

  }


  void findfuliao::TiaoZhuang_2(int page){
      MakeConditions cd_ruku("fuliao_ruku");
      if( !getConditions_2(cd_ruku) ) return ;

      MakeConditions cd_chuku("fuliao_chuku");
      QString order_id = ui->lineEdit_order_id_2->text().trimmed();
      QString BeginTime = ui->dateEdit_begindate_2->text();
      QString EndTime = ui->dateEdit_enddate_2->text();
      cd_chuku.AddBetween("date",BeginTime,EndTime);
      cd_chuku.AddIn("ruku_id","ruku_id",cd_ruku);
      QString Order_id = ui->lineEdit_order_id_2->text().trimmed();
      if( Order_id.length() > 0) cd_chuku.AddEqual("order_id",Order_id);


      MysqlOperate* m_MysqlOperate = MysqlOperate::getInstance();
      int data_count = m_MysqlOperate->Count(cd_chuku);
      int total_page = data_count/PAGESIZE;
      if(data_count%PAGESIZE >0 ) total_page++;
      if(page < 1 ) page = 1;
      else if(page > total_page) page = total_page;
      QString pagetext = QStringLiteral("第 %1 页/ %2 页").arg(page).arg(total_page);
      ui->label_page_2->setText(pagetext);


      cd_chuku.AddPage(page);
      MysqlOperate * m_mysqlins= MysqlOperate::getInstance();
      QVector<QMap<QString,QString>> ChuKuDatas;
      m_mysqlins->Find(cd_chuku,ChuKuDatas);

      QTableWidget *m_tb = ui->tableWidget_chuku;
      m_tb->setRowCount(ChuKuDatas.size());
      m_tb->clearContents();
      for(int i = 0;i<ChuKuDatas.size();i++){
          QMap<QString,int>::const_iterator itmap;
          QMap<QString,QString> &rowdata = ChuKuDatas[i];
          for (itmap = ChukuTableIndex.constBegin(); itmap != ChukuTableIndex.constEnd(); ++itmap) {
              m_tb->setItem(i, itmap.value(),new QTableWidgetItem(rowdata.value(itmap.key())));
          }
          {
             QPushButton *pBtn = new QPushButton(SS("编辑"));
             pBtn->setProperty("ruku_id",rowdata.value("ruku_id"));
             pBtn->setProperty("id",rowdata.value("id"));
             connect(pBtn, SIGNAL(clicked()), this, SLOT(EditChuKu()));
             m_tb->setCellWidget(i,9,pBtn);
          }
          {
             QPushButton *pBtn = new QPushButton(SS("删除"));
             pBtn->setProperty("ruku_id",rowdata.value("ruku_id"));
             pBtn->setProperty("id",rowdata.value("id"));
             pBtn->setProperty("row",i);
             connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteChuKu()));
             m_tb->setCellWidget(i,10,pBtn);
          }
      }

  }



  void findfuliao::DeleteRuKu(){
      QTableWidget *m_tw = ui->tableWidget_ruku;
      QPushButton *btn = (QPushButton *)sender();
      QString ruku_id = btn->property("ruku_id").toString();
      MakeConditions mc_ruku("fuliao_ruku");
      mc_ruku.AddEqual("ruku_id",ruku_id);
      MakeConditions mc_chuku("fuliao_chuku");
      mc_chuku.AddEqual("ruku_id",ruku_id);

      switch( QMessageBox::warning(NULL, SS("警告"),SS("确定要删除该入库单吗?对应的出库单也会被删除"),
                                   QMessageBox::Yes | QMessageBox::Default,
                                   QMessageBox::No | QMessageBox::Escape ))
      {
      case QMessageBox::Yes:
          m_MysqlOperate->Transaction();
          bool ok2 = m_MysqlOperate->Delete(mc_chuku);
          bool ok1 = m_MysqlOperate->Delete(mc_ruku);
          if( ok1 && ok2){
              m_MysqlOperate->Commit();
              m_tw->removeRow(btn->property("row").toInt());
              for(int i=0;i<m_tw->rowCount();i++){
                  m_tw->cellWidget(i,15)->setProperty("row",i);
              }
              QMessageBox::about(NULL, SS("提示"), SS("删除成功"));
          }else{
              m_MysqlOperate->Rollback();
              QMessageBox::about(NULL, SS("提示"), SS("删除失败"));
          }
      }

  }

#include "rukuedit.h"
  //调用删除面料接口
 void findfuliao::EditRuKu(){
    RuKuEdit wnd ;
    QPushButton *btn = (QPushButton *)sender();
    QString ruku_id = btn->property("ruku_id").toString();
    if( !wnd.LoadData(ruku_id)){
        QMessageBox::about(NULL, SS("错误提示"), SS("加载入库数据失败:")+ruku_id);
        return;
    }
    wnd.exec();

 }


 void findfuliao::DeleteChuKu(){
     QTableWidget *m_tw = ui->tableWidget_chuku;
     QPushButton *btn = (QPushButton *)sender();
     QString ruku_id = btn->property("ruku_id").toString();
     QString id = btn->property("id").toString();

     MakeConditions mc_ruku("fuliao_ruku");
     mc_ruku.AddEqual("ruku_id",ruku_id);
     MakeConditions mc_chuku("fuliao_chuku");
     mc_chuku.AddEqual("id",id);

     switch( QMessageBox::warning(NULL, SS("警告"),SS("确定要删除该入出库单吗"),
                                  QMessageBox::Yes | QMessageBox::Default,
                                  QMessageBox::No | QMessageBox::Escape ))
     {
     case QMessageBox::Yes:
         m_MysqlOperate->Transaction();
         QMap<QString,QString> ruku_data;
         QMap<QString,QString> chuku_data;
         bool ok1 = m_MysqlOperate->Get(mc_ruku,ruku_data);
         bool ok2 = m_MysqlOperate->Get(mc_chuku,chuku_data);
         bool ok3 = m_MysqlOperate->Delete(mc_chuku);
         int chukushuliang = chuku_data.value("shuliang").toInt();
         int jieyu_shuliang = chukushuliang + ruku_data.value("jieyu_shuliang").toInt();

         QMap<QString,QString> updatedata ={
             {"jieyu_shuliang",QString::number(jieyu_shuliang)},
             {"version",ruku_data.value("version")},
         };
         bool ok4 = m_MysqlOperate->Update(mc_ruku,updatedata);

         if( ok1 && ok2 && ok3 && ok4){
             m_MysqlOperate->Commit();
             m_tw->removeRow(btn->property("row").toInt());
             for(int i=0;i<m_tw->rowCount();i++){
                 m_tw->cellWidget(i,10)->setProperty("row",i);
             }
             QMessageBox::about(NULL, SS("提示"), SS("删除成功"));
         }else{
             m_MysqlOperate->Rollback();
             QMessageBox::about(NULL, SS("提示"), SS("删除失败"));
         }
     }
 }

#include "chukuedit.h"

 void findfuliao::EditChuKu(){
     ChuKuEdit wnd;
     QPushButton *btn = (QPushButton *)sender();
     int id = btn->property("id").toInt();
     if( !wnd.LoadData(id)){
         QMessageBox::about(NULL, SS("错误提示"), SS("加载入库数据失败:")+QString::number(id));
         return;
     }
     wnd.exec();

 }


