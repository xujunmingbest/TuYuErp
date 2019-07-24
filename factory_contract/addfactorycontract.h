#ifndef ADDFACTORYCONTRACT_H
#define ADDFACTORYCONTRACT_H

#include <QWidget>
#include <QMap>
namespace Ui {
class AddFactoryContract;
}

class AddFactoryContract : public QWidget
{
    Q_OBJECT

public:
    explicit AddFactoryContract(QWidget *parent = 0);
    ~AddFactoryContract();

private:
    Ui::AddFactoryContract *ui;
    QMap<QString,int> ProductTableIndex;
private slots:
    void TianJiaProduct();
    void DeleteProduct();
    void AddContract();

};

#endif // ADDFACTORYCONTRACT_H
