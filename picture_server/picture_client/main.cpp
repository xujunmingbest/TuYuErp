#include <QCoreApplication>

#include "qtclient.h"
#include <QDebug>
#include <windows.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString uuid;
    {
        QtClient client;
        client.Connect();
        client.UploadData("D:/TuYuErp/2.png");

        uuid = client.uuid;
        qDebug()<<client.Success;
        qDebug()<<client.uuid;
    }


    return a.exec();
}
