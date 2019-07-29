#ifndef FULIAOCHURUKU_H
#define FULIAOCHURUKU_H

#include <QWidget>
#include "MysqlOperate/mysqloperate.h"
#include "MysqlTableConfig/MysqlTableConfig.h"
namespace Ui {
class FuLiaoChuRuKu;
}

class FuLiaoChuRuKu : public QWidget
{
    Q_OBJECT

public:
    explicit FuLiaoChuRuKu(QWidget *parent = 0);
    ~FuLiaoChuRuKu();
    MysqlOperate* m_MysqlOperate;
private:
    Ui::FuLiaoChuRuKu *ui;
    bool RukuSlotValider();
    bool ChukuSlotValider(QMap<QString,QString> &RuKudata);
    void TiaoZhuang(int page);
private slots:
    void FuLiaoRukuSlot();
    void FuLiaoChukuSlot();
    void FuLiaoJieYuSlot();
    void TiaoZhuangSlot();
};

#endif // FULIAOCHURUKU_H
