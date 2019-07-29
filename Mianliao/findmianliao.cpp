#include "findmianliao.h"
#include "ui_findmianliao.h"

FindMianLiao::FindMianLiao(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindMianLiao)
{
    ui->setupUi(this);
    m_MysqlOperate = MysqlOperate::getInstance();
    {
        QTableWidget *m_tb = ui->tableWidget_ruku;
        m_tb->setColumnCount(20);
        m_tb->setRowCount(0);
        m_tb->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QStringList head ;
        head << SS("入库单号") << SS("日期") << SS("面料名称") <<SS("门幅")<<
                SS("米数") << SS("公斤") << SS("卷数") <<SS("克重")<<
                SS("颜色") << SS("其他费用") << SS("单价") <<SS("总金额")<<
                SS("供应商") << SS("备注") << SS("结余米数") << SS("结余公斤数") <<
                SS("结余卷数")<<SS("入库人")<< SS("编辑") << SS("删除");

        m_tb->setHorizontalHeaderLabels(head);
        m_tb->horizontalHeader()->setMinimumHeight(25);
        m_tb->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;font:11pt}");

        RukuTableIndex.insert("ruku_id",0);
        RukuTableIndex.insert("date",1);
        RukuTableIndex.insert("name",2);
        RukuTableIndex.insert("menfu",3);
        RukuTableIndex.insert("mishu",4);
        RukuTableIndex.insert("gongjin",5);
        RukuTableIndex.insert("juanshu",6);
        RukuTableIndex.insert("kezhong",7);
        RukuTableIndex.insert("color",8);
        RukuTableIndex.insert("other_fee",9);
        RukuTableIndex.insert("danjia",10);
        RukuTableIndex.insert("zongjine",11);
        RukuTableIndex.insert("gongyingshang",12);
        RukuTableIndex.insert("comment",13);
        RukuTableIndex.insert("jieyu_mishu",14);
        RukuTableIndex.insert("jieyu_gongjin",15);
        RukuTableIndex.insert("jieyu_juanshu",16);
        RukuTableIndex.insert("rukuren",17);
    }
    {
        QTableWidget *m_tb = ui->tableWidget_chuku;
        m_tb->setColumnCount(11);
        m_tb->setRowCount(0);
        m_tb->setEditTriggers(QAbstractItemView::NoEditTriggers);
        QStringList head ;
        head << SS("入库单号") << SS("日期") << SS("米数") << SS("公斤")
             << SS("卷数") << SS("订单号") << SS("领取人") <<SS("目标去向")
             << SS("备注") << SS("编辑") << SS("删除");

        m_tb->setHorizontalHeaderLabels(head);
        m_tb->horizontalHeader()->setMinimumHeight(25);
        m_tb->horizontalHeader()->setStyleSheet("QHeaderView::section{background:yellow;font:11pt}");

        ChukuTableIndex.insert("ruku_id",0);
        ChukuTableIndex.insert("date",1);
        ChukuTableIndex.insert("mishu",2);
        ChukuTableIndex.insert("gongjin",3);
        ChukuTableIndex.insert("juanshu",4);
        ChukuTableIndex.insert("dingdanhao",5);
        ChukuTableIndex.insert("lingquren",6);
        ChukuTableIndex.insert("targetto",7);
        ChukuTableIndex.insert("comment",8);
    }
}

FindMianLiao::~FindMianLiao()
{
    delete ui;
}


bool FindMianLiao::getConditions(MakeConditions &conditions){
    QString name = ui->lineEdit_name->text().trimmed();
    if(name.length() != 0){
        conditions.AddEqual("name",name);
    }
    QString BeginTime = ui->dateEdit_begindate->text();
    QString EndTime = ui->dateEdit_enddate->text();
    conditions.AddBetween("date",BeginTime,EndTime);
    QString MenFu = ui->lineEdit_menfu->text().trimmed();
    QString KeZhong = ui->lineEdit_kezhong->text().trimmed();
    if(MenFu.length() != 0){
         conditions.AddEqual("menfu",MenFu);
    }
    if(KeZhong.length() != 0){
         conditions.AddEqual("kezhong",KeZhong);
    }
    return true;
}

bool FindMianLiao::getConditions_2(MakeConditions &conditions){

    QString name = ui->lineEdit_name_2->text().trimmed();
    if(name.length() != 0){
        conditions.AddEqual("name",name);
    }
    QString MenFu = ui->lineEdit_menfu_2->text().trimmed();
    QString KeZhong = ui->lineEdit_kezhong_2->text().trimmed();
    if(MenFu.length() != 0){
         conditions.AddEqual("menfu",MenFu);
    }
    if(KeZhong.length() != 0){
         conditions.AddEqual("kezhong",KeZhong);
    }
    return true;
}


void FindMianLiao::shaixuanSlot(){
   TiaoZhuang(1);
}

void FindMianLiao::shaixuanSlot_2(){
   TiaoZhuang_2(1);
}

void FindMianLiao::TiaoZhuangSlot(){
    int page = ui->lineEdit_page->text().trimmed().toInt();
    TiaoZhuang(page);
}

void FindMianLiao::TiaoZhuangSlot_2(){
    int page = ui->lineEdit_page_2->text().trimmed().toInt();
    TiaoZhuang_2(page);
}

  void FindMianLiao::TiaoZhuang(int page){
      MakeConditions cd_ruku("mianliao_ruku");
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
              m_tb->setCellWidget(i,18,pBtn);
          }
          {
              QPushButton *pBtn = new QPushButton(SS("删除"));
              pBtn->setProperty("ruku_id",rowdata.value("ruku_id"));
              pBtn->setProperty("row",i);
              connect(pBtn, SIGNAL(clicked()), this, SLOT(DeleteRuKu()));
              m_tb->setCellWidget(i,19,pBtn);
          }
      }

  }


  void FindMianLiao::TiaoZhuang_2(int page){
      MakeConditions cd_ruku("mianliao_ruku");
      if( !getConditions_2(cd_ruku) ) return ;

      MakeConditions cd_chuku("mianliao_chuku");

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



  void FindMianLiao::DeleteRuKu(){
      QTableWidget *m_tw = ui->tableWidget_ruku;
      QPushButton *btn = (QPushButton *)sender();
      QString ruku_id = btn->property("ruku_id").toString();
      MakeConditions mc_ruku("mianliao_ruku");
      mc_ruku.AddEqual("ruku_id",ruku_id);
      MakeConditions mc_chuku("mianliao_chuku");
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
                  m_tw->cellWidget(i,19)->setProperty("row",i);
              }
              QMessageBox::about(NULL, SS("提示"), SS("删除成功"));
          }else{
              m_MysqlOperate->Rollback();
              QMessageBox::about(NULL, SS("提示"), SS("删除失败"));
          }
      }

  }

#include "Mianliao/mianliaoedit.h"
  //调用删除面料接口
 void FindMianLiao::EditRuKu(){
    MianliaoEdit wnd ;
    QPushButton *btn = (QPushButton *)sender();
    QString ruku_id = btn->property("ruku_id").toString();
    if( !wnd.LoadData(ruku_id)){
        QMessageBox::about(NULL, SS("错误提示"), SS("加载入库数据失败:")+ruku_id);
        return;
    }
    wnd.exec();

 }


 void FindMianLiao::DeleteChuKu(){
     QTableWidget *m_tw = ui->tableWidget_chuku;
     QPushButton *btn = (QPushButton *)sender();
     QString ruku_id = btn->property("ruku_id").toString();
     QString id = btn->property("id").toString();

     MakeConditions mc_ruku("mianliao_ruku");
     mc_ruku.AddEqual("ruku_id",ruku_id);
     MakeConditions mc_chuku("mianliao_chuku");
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
         int chuku_mishu = chuku_data.value("mishu").toInt();
         int chuku_gongjin = chuku_data.value("gongjin").toInt();
         int chuku_juanshu = chuku_data.value("juanshu").toInt();
         int jieyu_mishu = chuku_mishu + ruku_data.value("jieyu_mishu").toInt();
         int jieyu_gongjin = chuku_gongjin + ruku_data.value("jieyu_gongjin").toInt();
         int jieyu_juanshu = chuku_juanshu + ruku_data.value("jieyu_juanshu").toInt();
         QMap<QString,QString> updatedata ={
             {"jieyu_mishu",QString::number(jieyu_mishu)},
             {"jieyu_juanshu",QString::number(jieyu_juanshu)},
             {"jieyu_gongjin",QString::number(jieyu_gongjin)},
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

#include "mianliao_chuku_edit.h"

 void FindMianLiao::EditChuKu(){
     mianliao_chuku_edit wnd;
     QPushButton *btn = (QPushButton *)sender();
     int id = btn->property("id").toInt();
     if( !wnd.LoadData(id)){
         QMessageBox::about(NULL, SS("错误提示"), SS("加载入库数据失败:")+QString::number(id));
         return;
     }
     wnd.exec();

 }
