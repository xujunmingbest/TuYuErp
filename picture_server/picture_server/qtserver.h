#ifndef QTSERVER_H
#define QTSERVER_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpServer>
#include "../share/msg_def.h"

class QtServer : public QObject
{
	Q_OBJECT

public:
	QtServer(QObject *parent);
	~QtServer();
public:
	int m_connect_num;
	
private slots:
	void accept();
	void displayError(QAbstractSocket::SocketError socketError);
    void ReadHead();
	void disconnectedS1ot();

private:
	QStringList m_oData;
	QTcpServer *m_pTcpServer;
private:
	void DownLoad(QTcpSocket *sock,Head &h);
	void UpLoad(QTcpSocket *sock, Head &h); //放回一个 image_uuid
    void Delete(QTcpSocket *sock,Head &h); //放回一个 image_uuid
};

#endif // QTSERVER_H
