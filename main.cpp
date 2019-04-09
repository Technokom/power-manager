#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
#include <signal.h>
#include <QObject>
#include <QCommandLineParser>
#include <QtCore>
#include "powercontrol.h"

using namespace std;


//--------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    PowerControl *power = new PowerControl();
    power->moveToThread(power);
    power->start();

    return a.exec();
}
//--------------------------------------------------------------------
