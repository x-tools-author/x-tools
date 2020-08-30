#ifndef SAKSSLSOCKETTCPSERVER_HH
#define SAKSSLSOCKETTCPSERVER_HH

#include <QList>
#include <QSslSocket>
#include <QTcpServer>

class SAKSslSocketTcpServer : public QTcpServer
{
public:
    SAKSslSocketTcpServer();
    ~SAKSslSocketTcpServer();

    /**
     * @brief clients: Get the clients
     * @return Client list
     */
    QList<QSslSocket*> clients();

    /**
     * @brief removeClient: Remove client from client list.
     * @param client: The client that need to be removed.
     */
    void removeClient(QSslSocket *client);
protected:
    void incomingConnection(qintptr socketDescriptor);
private:
    QList<QSslSocket*> mSslSocketList;
};

#endif // SAKSSLSOCKETTCPSERVER_HH
