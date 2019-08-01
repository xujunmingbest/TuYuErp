#ifndef QTCLIENT_H
#define QTCLIENT_H

#include <QObject>
#include <QTcpSocket>


class QtClient : public QObject
{
    Q_OBJECT
public:
    explicit QtClient(QObject *parent = 0);
    bool Connect();
signals:


public:
    QTcpSocket *tcpClient;
    void UploadData(QString path);
    void DownData(QString uuid);
    void DeleteData(QString uuid);

    QString imageData;
    QString uuid;
    bool Success ;
private slots:
    //¿Í»§¶Ë²Ûº¯Êý
    void ReadData();
    void ReadError(QAbstractSocket::SocketError);
};

#endif // QTCLIENT_H
