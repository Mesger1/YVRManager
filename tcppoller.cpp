#include "tcppoller.h"

TCPPoller::TCPPoller()
{
    tcpSocket = new QTcpSocket(this);
    yawvr_tcp_state = state::AWAITING_YAW_IP;
}



void TCPPoller::init(QString _ip, int _port){
    this->ip = _ip;
    this->port = _port;
    dest_port = 0;
}

void TCPPoller::reset(){
    tcpSocket->close();
    yawvr_tcp_state = state::AWAITING_YAW_IP;
}

void TCPPoller::startTCPConnection(QString ip,int port){
#ifdef DEBUG
    qDebug() << "TCP INIT IP " << ip << " AND PORT " << port;
#endif
    init(ip,port);
    yawvr_tcp_state = state::CONNECT_TO_IP;
}

void TCPPoller::processTCP(){
    while(tcpSocket->bytesAvailable())
    {
        QString data = QString(tcpSocket->readAll().toStdString().c_str());
#ifdef TRACE
        qDebug() << "BEGIN TCP READ ##############################################################################################";
        qDebug() << data;
        qDebug() << "############################################################################################## END TCP READ";
#endif
        if(data.contains("AVAILABLE")){
#ifdef DEBUG
            qDebug() << "YAWVR AVAILABLE";
#endif
        }else if(data.contains("RESERVED")){
            dest_ip = ((data.split(';').at(2)).split(':')).at(0);
            dest_port = (((data.split(';').at(2)).split(':')).at(1)).toInt();
        }

    }
}

void TCPPoller::run() {


        switch(yawvr_tcp_state){
            case state::AWAITING_YAW_IP : {
#ifdef DEBUG
                qDebug() << "TCP waiting for yaw ip";
#endif
                break;
            }
            case state::CONNECT_TO_IP : {
                if(bind(ip,port)){
                    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(processTCP()));
                    yawvr_tcp_state = state::SEND_TCP_CHECK;
                }
                break;
            }
            case state::SEND_TCP_CHECK : {
                const char cmdBuf[] = { '0','G','A','M','E','Y','V','R','M','a','n','a','g','e','r'};
                QByteArray data = QByteArray::fromRawData(cmdBuf, sizeof(cmdBuf));
                if(sendData(data,ip)){
                    yawvr_tcp_state = state::AWAITING_CHECK_RESPONSE_AVAILABLE;
                }
                break;
            }
            case state::AWAITING_CHECK_RESPONSE_AVAILABLE : {
                const char cmdBuf[] = "\x30yvr";
                QByteArray data = QByteArray::fromRawData(cmdBuf, sizeof(cmdBuf)-1);
                if(sendData(data,ip)){
                    yawvr_tcp_state = state::AWAITING_CHECK_RESPONSE_PORT_DATA;
                }
                break;
            }
            case state::AWAITING_CHECK_RESPONSE_PORT_DATA : {
                if(dest_port != 0) {
                    yawvr_tcp_state = state::TCP_CHECK_ACHIEVED;
                    emit emitUDPPort(dest_ip,dest_port);
                }
                break;
            }
            case state::TCP_CHECK_ACHIEVED : {
                //qDebug() << "IN TCP CHECK ACHIEVED";
                break;
            }
        }
}



bool TCPPoller::bind(QString ip,int port){
#ifdef DEBUG
    qDebug() << "TCP Binding to ip " << ip << " with port " << port;
#endif
    tcpSocket->connectToHost(QHostAddress(ip.toStdString().c_str()),port);
    if(!tcpSocket->waitForConnected(5000))
    {
        qDebug() << "Error: " << tcpSocket->errorString();
        return 0;
    }
    return 1;
}

bool TCPPoller::sendData(QByteArray data,QString toAddress){
#ifdef DEBUG
    qDebug() << "TCP Sending data : " << data.toHex() << " to " << toAddress;
#endif
    int result = tcpSocket->write(data.toStdString().c_str());
    if(tcpSocket->waitForBytesWritten(5000)){
#ifdef DEBUG
        qDebug() << "bytes written : " << result;
#endif
        return true;
    }
    return false;
}
