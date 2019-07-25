#ifndef MIANLIAOJIEYU_H
#define MIANLIAOJIEYU_H

#include <QWidget>

namespace Ui {
class MianLiaoJieYU;
}

class MianLiaoJieYU : public QWidget
{
    Q_OBJECT

public:
    explicit MianLiaoJieYU(QWidget *parent = 0);
    ~MianLiaoJieYU();

private:
    Ui::MianLiaoJieYU *ui;
};

#endif // MIANLIAOJIEYU_H
