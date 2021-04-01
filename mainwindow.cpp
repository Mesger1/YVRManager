#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    QPixmap pix(":/images/icon-32.png");
    icon = QIcon(pix);



    trayIcon = new QSystemTrayIcon(icon,this);
    //Contextmenu build
    QMenu* sysTrayMenu = new QMenu(); // want to get a context menu from system tray
    //QAction *logYawVRTrackerDataAction =new QAction("LogUDP");
    //QAction *makeMemoryMapAvailableAction = new QAction("Generate MemoryMap");
    QAction *exitApp = new QAction("Close YVRManager");



    //sysTrayMenu->addAction(logYawVRTrackerDataAction);
    //sysTrayMenu->addAction(makeMemoryMapAvailableAction);
    sysTrayMenu->addAction(exitApp);


    trayIcon->setContextMenu(sysTrayMenu);



    connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(onActivatedSysTray(QSystemTrayIcon::ActivationReason)));
    connect(exitApp,SIGNAL(triggered()),this,SLOT(onExit()));




    trayIcon->setVisible(true);

    //  trayIcon->showMessage("Test Message", "Text", QSystemTrayIcon::Information, 1000);


    ui->setupUi(this);

    this->setWindowIcon(icon);
    this->setWindowTitle("YVR-Manager");

    worker = new Worker();



    connect(this->ui->cnctButton,SIGNAL(clicked(bool)),this,SLOT(connectYaw(bool)));
    connect(this->ui->discnctButton,SIGNAL(clicked(bool)),this,SLOT(disconnectYaw(bool)));
    connect(worker->getUDPPoller(),SIGNAL(foundYaw(QString,int,QString)),this,SLOT(addYaw(QString,int,QString)));
    connect(worker->getTCPPoller(),SIGNAL(emitUDPPort(QString,int)),worker->getUDPPoller(),SLOT(setListeningPort(QString,int)));
    connect(worker->getUDPPoller(),SIGNAL(emitIMUData(yawvr_data)),this,SLOT(displayIMUData(yawvr_data)));

    connect(this->ui->mmfBox,SIGNAL(toggled(bool)),worker,SLOT(writeMmf(bool)));


    this->ui->discnctButton->setEnabled(false);
    this->ui->cnctButton->setEnabled(false);


    this->show();
    trayIcon->show();


}

void MainWindow::connectYaw(bool flag){
    //TODO Clean up regex
    QRegularExpression re("[^\(]+[\)]");
    QRegularExpressionMatch match = re.match(this->ui->yawVRDeviceCombo->currentText());
    QString extracted = match.capturedTexts().at(0);
    QStringList list = extracted.split(':');
    QString ip = list.at(0);
    QString port = list.at(1);
    port = port.replace(")","");
    qDebug() << "Connect yaw at IP " << ip << " and port " << port;
    this->ui->cnctButton->setEnabled(false);
    this->ui->discnctButton->setEnabled(true);
    worker->getTCPPoller()->startTCPConnection(ip,port.toInt());
}



void MainWindow::disconnectYaw(bool flag){
    this->ui->cnctButton->setEnabled(true);
    this->ui->discnctButton->setEnabled(false);
    worker->getUDPPoller()->reset();
    worker->getTCPPoller()->reset();
    this->ui->mmfBox->setChecked(false);
}

void MainWindow::addYaw(QString ip , int port, QString yawname){
    if(this->ui->yawVRDeviceCombo->findText(ip,Qt::MatchContains) == -1){
        this->ui->yawVRDeviceCombo->addItem(yawname + " (" + ip + ":" + QString::number(port) + ")");
        trayIcon->showMessage(yawname , "Found at ip " + ip, QSystemTrayIcon::Information, 1000);
        this->ui->cnctButton->setEnabled(true);
#ifdef INFO
        qDebug() << "Added " <<  yawname + " (" + ip + ":" + QString::number(port) + ")";
#endif
    }
}

QComboBox* MainWindow::getYawVRDevices(){
    return this->ui->yawVRDeviceCombo;
}
QPushButton* MainWindow::getConnectButton(){
    return this->ui->cnctButton;
}
QPushButton* MainWindow::getDisconnectButton(){
    return this->ui->discnctButton;
}

void MainWindow::displayIMUData(yawvr_data data){
#ifdef DEBUG
    qDebug() << "IMU Data " << "Yaw " << data.yaw << " Pitch " << data.pitch << " Roll " << data.roll << " Battery " << data.battery;
#endif
    statusBar()->showMessage(QString("IMU Data : ") + QString("Yaw ") + QString::number(data.yaw) + QString("° Pitch ") +
                             QString::number(data.pitch) + QString("° Roll ") + QString::number(data.roll) + QString("° Battery ") +
                             QString::number(data.battery) + QString("v"));
}

void MainWindow::onActivatedSysTray(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        toggleMainwindowVisibility();
        break;
    case QSystemTrayIcon::Context:
        break;
    default:
        break;
    }
}

void MainWindow::toggleMainwindowVisibility(){
    if(flagVisibility){
        this->hide();
    }else{
        this->show();
    }
    flagVisibility = !flagVisibility;
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(!forcequit){
        event->ignore();
    }
    toggleMainwindowVisibility();

}

void MainWindow::onExit(){
    qDebug() << "quitting application";
    forcequit = true;
    this->close();
}

MainWindow::~MainWindow()
{

    delete ui;
}

