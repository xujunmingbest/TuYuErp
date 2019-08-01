#include <QCoreApplication>

#include "qtserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QtServer Server(nullptr);

    return a.exec();
}
