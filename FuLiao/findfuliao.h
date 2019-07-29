#ifndef FINDFULIAO_H
#define FINDFULIAO_H

#include <QWidget>
#include "MysqlOperate/mysqloperate.h"
namespace Ui {
class findfuliao;
}

class findfuliao : public QWidget
{
    Q_OBJECT

public:
    explicit findfuliao(QWidget *parent = 0);
    ~findfuliao();

private:
    Ui::findfuliao *ui;
    QMap<QString,int> RukuTableIndex;
    QMap<QString,int> ChukuTableIndex;

    bool getConditions(MakeConditions &conditions);
    void TiaoZhuang(int page);

    bool getConditions_2(MakeConditions &conditions);
    void TiaoZhuang_2(int page);
    MysqlOperate* m_MysqlOperate;
private slots:
    void shaixuanSlot();
    void TiaoZhuangSlot();

    void shaixuanSlot_2();
    void TiaoZhuangSlot_2();

    void DeleteRuKu();
    void EditRuKu();

    void DeleteChuKu();
    void EditChuKu();
};

#endif // FINDFULIAO_H
