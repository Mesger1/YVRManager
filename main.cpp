#include "mainwindow.h"

#include <QApplication>
#include <mainwindow.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("YawVR Manager");
    app.setApplicationDisplayName("YawVR Manager");

    MainWindow *window = new MainWindow();
    window->setWindowTitle(app.applicationName());
    QFile File("stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());

    app.setStyleSheet(StyleSheet);

    return app.exec();
}


