#ifndef CHENGPINCHURUKU_H
#define CHENGPINCHURUKU_H

#include <QWidget>

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
};

#endif // CHENGPINCHURUKU_H
