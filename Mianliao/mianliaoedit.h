#ifndef MIANLIAOEDIT_H
#define MIANLIAOEDIT_H

#include <QDialog>

namespace Ui {
class MianliaoEdit;
}
#include "MysqlOperate/mysqloperate.h"

class MianliaoEdit : public QDialog
{
    Q_OBJECT

public:
    explicit MianliaoEdit(QWidget *parent = 0);
    ~MianliaoEdit();
    bool LoadData(QString ruku_id);
private:
    MysqlOperate* m_MysqlOperate;
    Ui::MianliaoEdit *ui;
    int jieyu_mishu;
    int jieyu_gongjin;
    int jieyu_juanshu;
    int mishu;
    int gongjin;
    int juanshu;
    int version;
    QString ruku_id;
private slots:
    bool RukuSlotValider();
    void MianLiaoRukuEditSlot();
};

#endif // MIANLIAOEDIT_H
