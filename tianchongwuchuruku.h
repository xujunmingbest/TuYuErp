#ifndef TIANCHONGWUCHURUKU_H
#define TIANCHONGWUCHURUKU_H

#include <QWidget>

namespace Ui {
class TianChongWuChuRuKu;
}

class TianChongWuChuRuKu : public QWidget
{
    Q_OBJECT

public:
    explicit TianChongWuChuRuKu(QWidget *parent = 0);
    ~TianChongWuChuRuKu();

private:
    Ui::TianChongWuChuRuKu *ui;
};

#endif // TIANCHONGWUCHURUKU_H
