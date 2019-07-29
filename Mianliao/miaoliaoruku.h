#ifndef MIAOLIAORUKU_H
#define MIAOLIAORUKU_H

#include <QWidget>

namespace Ui {
class MiaoliaoRuku;
}

class MiaoliaoRuku : public QWidget
{
    Q_OBJECT

public:
    explicit MiaoliaoRuku(QWidget *parent = 0);
    ~MiaoliaoRuku();

private:
    Ui::MiaoliaoRuku *ui;
    bool ChukuSlotValider(QVector<QMap<QString,QString>> &RuKudata);
    bool RukuSlotValider();
    void FormatIntlineEdit();
    void TiaoZhuang(int page);
private slots:
    void MianLiaoRukuSlot();
    void MianLiaoChukuSlot();
    void MianLiaoJieYuSlot();
    void TiaoZhuangSlot();
};

#endif // MIAOLIAORUKU_H
