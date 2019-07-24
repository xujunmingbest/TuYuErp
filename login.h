#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    bool get_login_success(){return is_login_success;}
private:
    bool is_login_success;
    Ui::Login *ui;

private slots:
    void LoginSlot();
};

#endif // LOGIN_H
