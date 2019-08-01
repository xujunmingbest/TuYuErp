#include "qtserver.h"
#include <assert.h>
#include <QTcpSocket>

QtServer::QtServer(QObject *parent)
	: QObject(parent)
{
	m_pTcpServer = new QTcpServer(this);
	//2. 新连接、错误信号
	connect(m_pTcpServer, &QTcpServer::newConnection, this, &QtServer::accept);
	connect(m_pTcpServer, &QTcpServer::acceptError, this, &QtServer::displayError);

	//3. 启动服务端
	if (!m_pTcpServer->listen(QHostAddress::Any, 8888))
	{
		qDebug() << "m_pTcpServer->listen() error";
		assert(false);
	}
	else {
		qDebug() << "open success";
	}
	m_connect_num = 0;
}

QtServer::~QtServer()
{

}

void QtServer::accept() {
	QTcpSocket *currentClient = m_pTcpServer->nextPendingConnection();
    connect(currentClient, SIGNAL(readyRead()), this, SLOT(ReadHead()));
	connect(currentClient, SIGNAL(disconnected()), this, SLOT(disconnectedS1ot()));
	m_connect_num++;
	qDebug() << "now connect number:" << m_connect_num;
}

void QtServer::displayError(QAbstractSocket::SocketError socketError)
{
	qDebug() << "SimpleTcpSocketServerDemo::displayError " << socketError;
}

//协议

#include "../share/msg_def.h"
using namespace std;
void QtServer::ReadHead() {
    qDebug() << "ReadHead";

	QTcpSocket *tcp = (QTcpSocket *)sender();
	int headlen = HEADLEN;
	Head h;
	{
		char head[HEADLEN];
		int readlen = 0;
		while (readlen < headlen) {
            int nowreadlen = tcp->read( (char*)&head +readlen , headlen - readlen);
            readlen += nowreadlen;
            if (nowreadlen <= 0) {
				tcp->close();
				return;
			}
            if(readlen < headlen )  tcp->waitForReadyRead(5000);
		}
		memcpy(&h, head, headlen);
        qDebug()<< "h.msg_type" << h.msg_type;
        if (string(h.identify_code,38) != identifying_code) {
			tcp->close();
			return;
		}
		if (h.image_len < 0 || h.image_len > 10000000) {
			tcp->close();
			return;
		}
	}

	switch (h.msg_type) {
	case e_msg_type::download:
        qDebug() << "down_load";
		DownLoad(tcp, h);
        break;
	case e_msg_type::upload:
        qDebug() << "UpLoad";
		UpLoad(tcp, h);
        break;
    case e_msg_type::delete_picture:
        qDebug() << "delete_picture";
        Delete(tcp,h);
        break;
	default:
		tcp->close();
		return;
	}
    tcp->close();
    return;
}

void QtServer::disconnectedS1ot() {
	m_connect_num--;
	qDebug() << "disconnect";
}

#include <QFile>
#include <QDataStream>
#include <QUuid>
void QtServer::DownLoad(QTcpSocket *sock, Head &h) {

    QFile file(save_dir + QString::fromStdString(string(h.image_uuid,38)));
    e_msg_type e_ret = e_msg_type::download_return;
    if( !file.open(QIODevice::ReadOnly)) e_ret = e_msg_type::failed;
    QByteArray ba = file.readAll();
	file.close();

	Head h_send;
    h_send.image_len = ba.length();
    h_send.msg_type = e_ret;
	memcpy(h_send.image_uuid, h.image_uuid, 38);
	sock->write((char*)&h_send, HEADLEN);
    sock->write(ba);

}


void QtServer::UpLoad(QTcpSocket *sock, Head &h) {
	char *body = new char[h.image_len];
	int readlen = 0;
	while (readlen < h.image_len) {
        int nowreadlen = sock->read( body +readlen , h.image_len - readlen);
		readlen += nowreadlen;;
        if (nowreadlen <= 0) { //读数据超时
			sock->close();
            delete []body;
			return;
		}
        if(readlen < h.image_len )  sock->waitForReadyRead(5000);
	}
	QUuid id = QUuid::createUuid();
    e_msg_type e_ret = e_msg_type::upload_return;
	QFile file(save_dir + id.toString());
    if( !file.open(QIODevice::WriteOnly) ) e_ret = e_msg_type::failed;
    file.write(body, h.image_len);
	file.close();
    delete []body;
	Head h_send;
	h_send.image_len = 0;
    h_send.msg_type = e_ret;
	memcpy(h_send.image_uuid, id.toString().toStdString().c_str(), 38);
    sock->write( (char*)&h_send,HEADLEN);
}


 void QtServer::Delete(QTcpSocket *sock,Head &h){
     QFile::remove(save_dir + QString::fromStdString(string(h.image_uuid,38)));
     Head h_send;
     h_send.image_len = 0;
     h_send.msg_type = e_msg_type::delete_picture_return;
     memcpy(h_send.image_uuid, h.image_uuid, 38);
     sock->write( (char*)&h_send,HEADLEN);
 }
