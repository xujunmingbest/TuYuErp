#ifndef CHENGPINCHURUKU_H
#define CHENGPINCHURUKU_H

#include <QWidget>
#include "MysqlOperate/mysqloperate.h"
namespace Ui {
class ChengPinChuRuKu;
}

class ChengPinChuRuKu : public QWidget
{
    Q_OBJECT

public:
    explicit ChengPinChuRuKu(QWidget *parent = 0);
    ~ChengPinChuRuKu();

private:
    Ui::ChengPinChuRuKu *ui;

    MysqlOperate* m_MysqlOperate;
private:
    bool RukuSlotValider();
    bool ChukuSlotValider(QMap<QString,QString> &RuKudata);
    void TiaoZhuang(int page);
private slots:
    void ChengPinRukuSlot();
    void ChengPinChukuSlot();
    void ChengPinJieYuSlot();
    void TiaoZhuangSlot();
};

#endif // CHENGPINCHURUKU_H
