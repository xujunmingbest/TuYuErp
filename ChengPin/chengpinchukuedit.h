#ifndef CHENGPINCHUKUEDIT_H
#define CHENGPINCHUKUEDIT_H

#include <QDialog>

namespace Ui {
class ChengPinChuKuEdit;
}
#include "MysqlOperate/mysqloperate.h"
class ChengPinChuKuEdit : public QDialog
{
    Q_OBJECT

public:
    explicit ChengPinChuKuEdit(QWidget *parent = 0);
    ~ChengPinChuKuEdit();
        bool LoadData(int id);
private:
    Ui::ChengPinChuKuEdit *ui;
    MysqlOperate * m_MysqlOperate;
private:
    int jieyu_shuliang;
    int shuliang;

    int version_chuku;
    int version_ruku;
    int id;
    bool ChukuSlotValider();
private slots:
    void ChengPinChukuSlot();
};

#endif // CHENGPINCHUKUEDIT_H
