#include "myserver.h"

MyServer::MyServer()
{
    if(this->listen(QHostAddress::Any, 2323))
    {
        qDebug() << "start";
    }
    else
    {
        qDebug() << "error";
    }
    nextBlockSize = 0;
}



void MyServer::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &MyServer::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    sockets.push_back(socket);
    qDebug() << "client connected" << socketDescriptor;
}

void MyServer::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_3);
    if(in.status() == QDataStream::Ok){
        qDebug() << "read...";
//        QString str;
//        in >> str;
//        qDebug() << str;
//        sendToClient(str);
        for (;;){
            if (nextBlockSize == 0)
            {
                qDebug() << "nextBlockSize = 0";
                if (socket->bytesAvailable() < 2)
                {
                    qDebug() << "nextBlockSize < 2, break";
                    break;
                }
                in >> nextBlockSize;
                qDebug() << "nextBlockSize = " << nextBlockSize;
            }
                if (socket->bytesAvailable() < nextBlockSize)
                {
                    qDebug() << "Data not full, break";
                    break;
                }
                QString str;
                in >> str;
                nextBlockSize = 0;
                qDebug() << str;
                sendToClient(str);
                break;
        }
    }
    else
    {
        qDebug() << "DataStream error";
    }
}


void MyServer::sendToClient(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_3);
    out << quint16(0) << str;
    out.device()->seek(0);
    out << quint16(Data.size() - sizeof(quint16));
//    socket->write(Data);
    for (int i = 0; i < sockets.size(); i++){
        sockets[i]->write(Data);
    }

}










