#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

class MyServer : public QTcpServer
{
    Q_OBJECT

public:
    MyServer();
    QTcpSocket *socket;

private:
    QVector<QTcpSocket*> sockets;
    QByteArray Data;
    void sendToClient(QString str);
    quint16 nextBlockSize;

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();

};

#endif // MYSERVER_H
