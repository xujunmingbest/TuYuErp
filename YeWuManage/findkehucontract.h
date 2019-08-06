#ifndef FINDKEHUCONTRACT_H
#define FINDKEHUCONTRACT_H

#include <QWidget>

namespace Ui {
class FindKeHuContract;
}

class FindKeHuContract : public QWidget
{
    Q_OBJECT

public:
    explicit FindKeHuContract(QWidget *parent = 0);
    ~FindKeHuContract();

private:
    Ui::FindKeHuContract *ui;
};

#endif // FINDKEHUCONTRACT_H
