#include "udppoller.h"

UDPPoller::UDPPoller()
{
    udpSocket = new QUdpSocket(this);
    yawvr_udp_state = state::CONNECT_TO_IP;
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processUDP()),Qt::AutoConnection);
    connect(udpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SocketError(QAbstractSocket::SocketError)));

}

void UDPPoller::init(QString _ip, int _port){
    this->ip = _ip;
    this->port = _port;
}

void UDPPoller::reset(){
    udpSocket->close();
    yawvr_udp_state = state::CONNECT_TO_IP;
}

bool UDPPoller::processUDP(){


    QByteArray datagram;
        while (udpSocket->hasPendingDatagrams()) {

            datagram.resize(udpSocket->pendingDatagramSize());

            QHostAddress sender;
            quint16 port;
            udpSocket->readDatagram(datagram.data(),datagram.size(),&sender,&port);
            if(!datagram.isNull()){

#ifdef TRACE
                qDebug() << "BEGIN UDP DATA ##########################################################################################";
                qDebug() << datagram.toStdString().c_str();
                qDebug() << "########################################################################################## END UDP DATA";
#endif
                QString data = datagram.toStdString().c_str();
                if(data.startsWith("YAWDEVICE")){
                    QStringList list = data.split(';');
                    yaw_ip = sender.toString();
                    yaw_nr = list.at(1);
                    yaw_name = list.at(2);
                    yaw_port = list.at(3);
                    yaw_availability = list.at(4);
                    yawvr_udp_state = state::FOUND_YAW;
                    //emit startTCP(yaw_ip,yaw_port.toInt());
                    return 1;
                }else if(data.startsWith("S") && data.endsWith("]")){
                    QStringList regexData = data.split(QRegularExpression("\\[|\\]"));
                    memset(&imuData,'\0',sizeof(imuData));
                    imuData.yaw = regexData.at(1).toFloat();
                    imuData.pitch = regexData.at(3).toFloat();
                    imuData.roll = regexData.at(5).toFloat();
                    imuData.battery = regexData.at(7).toFloat();
                    yawvr_udp_state = state::GETTING_IMU_DATA;
                    emit emitIMUData(imuData);
                }
            }
        }

}

void UDPPoller::setListeningPort(QString dst_ip,int dst_port){
    udpSocket->close();
    bind(dst_ip,dst_port);
    yawvr_udp_state = state::GETTING_IMU_DATA;
}

int UDPPoller::run() {
        switch(yawvr_udp_state){
            case state::CONNECT_TO_IP : {
                if(bind(ip,port)){
                    yawvr_udp_state = state::SEARCHING_YAW;
                }
                break;
            }
            case state::SEARCHING_YAW : {
                sendData("YAW_CALLING","255.255.255.255");
                yawvr_udp_state = state::WAITING_FOR_RESPONSE;
                break;
            }   
            case state::WAITING_FOR_RESPONSE : {
                if(counter > udp_yawvr_retries){
                    counter = 0;
                    yawvr_udp_state = state::CONNECT_TO_IP;
                    udpSocket->close();
                    return yawvr_udp_run_status::yaw_not_reponding;
                }
                break;
            }
            case state::FOUND_YAW : {
#ifdef INFO
                qDebug() << "Found YawVR at ip " << yaw_ip;
#endif
                counter = 0;
                yawvr_udp_state = state::CONNECT_TO_IP;
                udpSocket->close();
                emit foundYaw(yaw_ip,yaw_port.toInt(),yaw_name);
                break;
            }
            case state::GETTING_IMU_DATA : {
                return yawvr_udp_run_status::receiving_imu_data;
                break;
            }
            default: {
            }
        }
        counter++;

        return yawvr_udp_run_status::yaw_run_loop_working;
}

yawvr_data UDPPoller::getData(){
    return imuData;
}

bool UDPPoller::bind(QString ip,int port){
#ifdef DEBUG
    qDebug() << "UDP Binding to ip " << ip << " with port " << port;
#endif
    udpSocket->bind(QHostAddress(ip.toStdString().c_str()),port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
return 1;
}
void UDPPoller::SocketError(QAbstractSocket::SocketError x)
{
    qDebug() << dynamic_cast<QUdpSocket *>(sender())->objectName() + QString(": ") + dynamic_cast<QUdpSocket *>(sender())->errorString();
}
void UDPPoller::sendData(QString data,QString toAddress){
#ifdef DEBUG
    qDebug() << "UDP Sending data : " << data << " to " << toAddress;
#endif
    udpSocket->writeDatagram(data.toStdString().c_str(), QHostAddress(toAddress.toStdString().c_str()), 50010);
}
