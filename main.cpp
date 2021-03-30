#include "mainwindow.h"

#include <QApplication>
#include <QSharedMemory>
#include <QLabel>
#include <mainwindow.h>

int main(int argc, char *argv[])
{

    QSharedMemory shared("62d60669-bb94-4a94-88bb-b964890a7e04");
    if( !shared.create( 512, QSharedMemory::ReadWrite) )
    {
        exit(0);
    }

    QApplication app(argc, argv);


    QCoreApplication::setApplicationName("YawVR Manager");
    app.setApplicationDisplayName("YawVR Manager");

    MainWindow *window = new MainWindow();

    window->setWindowTitle(app.applicationName());

    QFile file(":/stylesheet.qss");
    file.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(file.readAll());

    app.setStyleSheet(StyleSheet);

    return app.exec();
}


