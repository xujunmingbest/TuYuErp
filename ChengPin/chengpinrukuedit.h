#ifndef CHENGPINRUKUEDIT_H
#define CHENGPINRUKUEDIT_H

#include <QDialog>
#include "MysqlOperate/mysqloperate.h"
namespace Ui {
class ChengPinRuKuEdit;
}

class ChengPinRuKuEdit : public QDialog
{
    Q_OBJECT

public:
    explicit ChengPinRuKuEdit(QWidget *parent = 0);
    ~ChengPinRuKuEdit();
    bool LoadData(QString Ruku_id);
private:
    Ui::ChengPinRuKuEdit *ui;

    MysqlOperate* m_MysqlOperate;
    int jieyu_shuliang;
    int shuliang;
    int version;
    QString ruku_id;

private slots:
    bool RukuSlotValider();
    void ChengPinRukuEditSlot();
};

#endif // CHENGPINRUKUEDIT_H
