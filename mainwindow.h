#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QCloseEvent>
#include <QString>
#include <udppoller.h>
#include <tcppoller.h>
#include <workerthread.h>
#include "defines.h"
#include <QComboBox>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QComboBox *getYawVRDevices();
    QPushButton *getConnectButton();
    QPushButton *getDisconnectButton();

private:
    QThread *thread;
    Ui::MainWindow *ui;
    void toggleMainwindowVisibility();
    bool flagVisibility = false;
    bool forcequit = false;
    QSystemTrayIcon *trayIcon;
    Worker *worker;
private slots:
    void closeEvent(QCloseEvent *event);
    void onExit();
    void onActivatedSysTray(QSystemTrayIcon::ActivationReason reason);
    void displayIMUData(yawvr_data);
    void addYaw(QString ip , int port, QString yawname);
    void connectYaw(bool);
    void disconnectYaw(bool);
signals:
    void operate();
};
#endif // MAINWINDOW_H
