#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QThread>
#include <udppoller.h>
#include <tcppoller.h>
#include <QComboBox>
#include "defines.h"
#include <QNetworkInterface>
#include "memmap.h"

class Worker : public QObject
{
    Q_OBJECT
private:
    UDPPoller *udpPoller;
    TCPPoller *tcpPoller;
    QTimer *timer;
    QTimer *networkscannerTimer;
    QStringList networkIPV4List;
    QStringList getNetworkInterfaces();
    QComboBox *yawCBox;
    int interfaceNr = 0;
    MemMap *mmf;
    bool isWriteMmf = false;
public:
    Worker();
    UDPPoller* getUDPPoller();
    TCPPoller* getTCPPoller();
public slots:
    void run();
    void refreshNetworkIfaces();
    void writeMmf(bool isWriteMmf);
};

#endif // WORKERTHREAD_H
