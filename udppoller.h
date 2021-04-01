#ifndef UDPPOLLER_H
#define UDPPOLLER_H

#include <QThread>
#include <QUdpSocket>
#include <QString>
#include <QNetworkDatagram>
#include <QTimer>
#include <QRegularExpression>
#include "defines.h";

class UDPPoller : public QObject
{
    Q_OBJECT

private:
    QUdpSocket *udpSocket;
    QString ip;
    int port;
    int dest_ip;
    int dest_port;
    yawvr_data imuData;
    int counter = 0;

    enum state {
        SEARCHING_YAW = 1,
        FOUND_YAW = 2,
        CONNECT_TO_IP = 3,
        GETTING_IMU_DATA = 4,
        WAITING_FOR_RESPONSE = 5
    } yawvr_udp_state;



    void sendData(QString data,QString toAddress);
    bool bind(QString ip,int port);

public:
    QString yaw_ip;
    QString yaw_name;
    QString yaw_port;
    QString yaw_nr;
    QString yaw_availability;
    UDPPoller();
    void init(QString _ip,int _port);
    void reset();
    yawvr_data getData();

public slots:
    bool processUDP();
    int run();
    void SocketError(QAbstractSocket::SocketError sx);
    void setListeningPort(QString,int dest_port);

signals:
    void foundYaw(QString ip,int port,QString name);
    void emitIMUData(yawvr_data imuData);
};

#endif // UDPPOLLER_H
