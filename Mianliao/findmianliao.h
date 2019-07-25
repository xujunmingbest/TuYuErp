#ifndef FINDMIANLIAO_H
#define FINDMIANLIAO_H

#include <QWidget>

namespace Ui {
class FindMianLiao;
}

class FindMianLiao : public QWidget
{
    Q_OBJECT

public:
    explicit FindMianLiao(QWidget *parent = 0);
    ~FindMianLiao();

private:
    Ui::FindMianLiao *ui;
};

#endif // FINDMIANLIAO_H
