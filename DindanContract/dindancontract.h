#ifndef DINDANCONTRACT_H
#define DINDANCONTRACT_H

#include <QWidget>

namespace Ui {
class DindanContract;
}

class DindanContract : public QWidget
{
    Q_OBJECT

public:
    explicit DindanContract(QWidget *parent = 0);
    ~DindanContract();

private:
    Ui::DindanContract *ui;
};

#endif // DINDANCONTRACT_H
