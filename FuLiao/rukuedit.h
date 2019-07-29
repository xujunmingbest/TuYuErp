#ifndef RUKUEDIT_H
#define RUKUEDIT_H

#include <QDialog>
#include "MysqlOperate/mysqloperate.h"
namespace Ui {
class RuKuEdit;
}

class RuKuEdit : public QDialog
{
    Q_OBJECT

public:
    explicit RuKuEdit(QWidget *parent = 0);
    ~RuKuEdit();
    bool LoadData(QString Ruku_id);
private:
    Ui::RuKuEdit *ui;

    MysqlOperate* m_MysqlOperate;
    int jieyu_shuliang;
    int shuliang;
    int version;
    QString ruku_id;

private slots:
    bool RukuSlotValider();
    void FuLiaoRukuEditSlot();
};

#endif // RUKUEDIT_H
