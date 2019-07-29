#ifndef CHUKUEDIT_H
#define CHUKUEDIT_H

#include <QDialog>

namespace Ui {
class ChuKuEdit;
}

#include "MysqlOperate/mysqloperate.h"


class ChuKuEdit : public QDialog
{
    Q_OBJECT

public:
    explicit ChuKuEdit(QWidget *parent = 0);
    ~ChuKuEdit();
    bool LoadData(int id);
private:
    Ui::ChuKuEdit *ui;

    MysqlOperate * m_MysqlOperate;
private:
    int jieyu_shuliang;
    int shuliang;

    int version_chuku;
    int version_ruku;
    int id;
    bool ChukuSlotValider();
private slots:
    void FuLiaoChukuSlot();
};

#endif // CHUKUEDIT_H
