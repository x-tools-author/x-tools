#include "SAKSslSocketTcpServer.hh"

SAKSslSocketTcpServer::SAKSslSocketTcpServer()
{

}

SAKSslSocketTcpServer::~SAKSslSocketTcpServer()
{

}

void SAKSslSocketTcpServer::incomingConnection(qintptr socketDescriptor)
{
    auto var = new QSslSocket(this);
    var->setSocketDescriptor(socketDescriptor);
    mSslSocketList.append(var);
}

QList<QSslSocket*> SAKSslSocketTcpServer::clients()
{
    return mSslSocketList;
}

void SAKSslSocketTcpServer::removeClient(QSslSocket *client)
{
    mSslSocketList.removeOne(client);
}
