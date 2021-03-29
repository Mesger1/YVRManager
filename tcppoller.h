#ifndef TCPPOLLER_H
#define TCPPOLLER_H

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include "defines.h"

class TCPPoller : public QObject
{

    Q_OBJECT

private:
    QTcpSocket *tcpSocket;
    QString ip;
    int port;
    int dest_port;
    QString dest_ip;

    enum state {
        TCP_CHECK_ACHIEVED = 1,
        SEND_TCP_CHECK = 2,
        CONNECT_TO_IP = 3,
        AWAITING_YAW_IP = 4,
        AWAITING_CHECK_RESPONSE_AVAILABLE = 5,
        AWAITING_CHECK_RESPONSE_PORT_DATA = 6
    } yawvr_tcp_state;

    bool sendData(QByteArray data,QString toAddress);
    bool bind(QString ip,int port);

public:
    TCPPoller();
    void run();
    void init(QString _ip,int _port);
    void reset();

public slots:
    void processTCP();
    void startTCPConnection(QString,int);

signals:
    void emitUDPPort(QString,int);
};

#endif // TCPPOLLER_H
