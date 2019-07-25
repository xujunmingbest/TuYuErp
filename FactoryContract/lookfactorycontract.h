#ifndef LOOKFACTORYCONTRACT_H
#define LOOKFACTORYCONTRACT_H

#include <QWidget>
#include <QMap>
namespace Ui {
class LookFactoryContract;
}

class LookFactoryContract : public QWidget
{
    Q_OBJECT

public:
    explicit LookFactoryContract(QWidget *parent = 0);
    ~LookFactoryContract();
    void LoadData(QString Contract_id);
private:
    Ui::LookFactoryContract *ui;
    QMap<QString,int> ProductTableIndex;
};

#endif // LOOKFACTORYCONTRACT_H
