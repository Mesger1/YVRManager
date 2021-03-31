#include "workerthread.h"
#include "mainwindow.h"

Worker::Worker()
{

    timer = new QTimer(this);
    networkscannerTimer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(run()));
    connect(networkscannerTimer,SIGNAL(timeout()),this,SLOT(refreshNetworkIfaces()));
    timer->start(10);
    networkscannerTimer->start(3000);


    udpPoller = new UDPPoller();
    tcpPoller = new TCPPoller();


    networkIPV4List = getNetworkInterfaces();
    udpPoller->init(networkIPV4List.at(0),50020);


//    yawCBox = qobject_cast<MainWindow*>(parent)->getYawVRDevices();

//    parent->getDisconnectButton()->setEnabled(false);

    networkIPV4List = getNetworkInterfaces();
}

void Worker::refreshNetworkIfaces(){
    networkIPV4List = getNetworkInterfaces();
}

void Worker::writeMmf(bool _isWriteMmf){
        isWriteMmf = _isWriteMmf;
        if(isWriteMmf){
            mmf = new MemMap();
        }else{
            delete mmf;
        }
}



QStringList Worker::getNetworkInterfaces(){
#ifdef DEBUG
    qDebug() << "Getting network interfaces : ";
#endif
    QStringList items;
    foreach(QNetworkInterface iface, QNetworkInterface::allInterfaces())
    {
        if (iface.flags().testFlag(QNetworkInterface::IsUp) && !iface.flags().testFlag(QNetworkInterface::IsLoopBack))
            foreach (QNetworkAddressEntry entry, iface.addressEntries())
            {
                if ( iface.hardwareAddress() != "00:00:00:00:00:00" && entry.ip().toString().contains(".") && !iface.humanReadableName().contains("VM")){
                    items.append(entry.ip().toString());
#ifdef DEBUG
                    qDebug() << entry.ip().toString();
#endif
                }
            }

    }
    return items;
}


void Worker::run() {

    QString currentIP = networkIPV4List.at(interfaceNr);

    udpPoller->init(currentIP,50020);
#ifdef DEBUG
    qDebug() << "Trying IP " << currentIP;
#endif

    switch(udpPoller->run()){
        case yawvr_udp_run_status::yaw_not_reponding :{
            interfaceNr++;
            break;
        }
        case yawvr_udp_run_status::receiving_imu_data :{
            if(isWriteMmf){
                mmf->pushMemMap(udpPoller->getData());
            }
            break;
        }
        default:{
            break;
        }
    }

    if(interfaceNr > networkIPV4List.length()-1){
        interfaceNr = 0;
    }

    tcpPoller->run();
}


UDPPoller* Worker::getUDPPoller(){
    return udpPoller;
}
TCPPoller* Worker::getTCPPoller(){
    return tcpPoller;
}
