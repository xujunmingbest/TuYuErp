#ifndef MIANLIAO_CHUKU_EDIT_H
#define MIANLIAO_CHUKU_EDIT_H

#include <QDialog>
#include "MysqlOperate/mysqloperate.h"

namespace Ui {
class mianliao_chuku_edit;
}

class mianliao_chuku_edit : public QDialog
{
    Q_OBJECT

public:
    explicit mianliao_chuku_edit(QWidget *parent = 0);
    ~mianliao_chuku_edit();
    MysqlOperate * m_MysqlOperate;
    bool LoadData(int id);
private:
    int jieyu_mishu;
    int jieyu_gongjin;
    int jieyu_juanshu;
    int mishu;
    int gongjin;
    int juanshu;
    int version_chuku;
    int version_ruku;
    int id;
    bool ChukuSlotValider();
    Ui::mianliao_chuku_edit *ui;
private slots:
    void MianLiaoChukuSlot();
};

#endif // MIANLIAO_CHUKU_EDIT_H
