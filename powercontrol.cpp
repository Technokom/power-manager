#include <QFile>
#include "powercontrol.h"
#include "pminterfaceadaptor.h"

#define TIME_TO_SHUTDOWN        70
#define CHARGER_ONLINE_STATUS   "/sys/class/power_supply/bq25890-charger/online"

//--------------------------------------------------------------------
PowerControl::PowerControl (QObject *parent) :
    QThread (parent),
    serviceName ("ru.tknav.PowerManagerInterface"),
    servicePath ("/ru/tknav/PowerManagerInterface") {

}
//--------------------------------------------------------------------
int PowerControl::UpdateStatusPower () {
    int status;
    QFile file(CHARGER_ONLINE_STATUS);
    if (file.open(QIODevice::ReadOnly)) {
        QString qs(file.readAll());
        file.close();
        status = qs.toInt();
    } else {
        status = 0;
    }
    return status;
}
//--------------------------------------------------------------------
void PowerControl::correctRtc (void) {
    system ("hwclock -w");
}
//--------------------------------------------------------------------
void PowerControl::PowerOff (void) {
    system ("halt -p");
}
//--------------------------------------------------------------------
int PowerControl::CurrentStatus () {
    return powerStatus;
}
//--------------------------------------------------------------------
void PowerControl::ReverseTimeout () {
    reverse--;

    emit TimeToShutdown(reverse);

    if (reverse < 1) {
        correctRtc ();
        PowerOff ();
    }
}
//--------------------------------------------------------------------
void PowerControl::Timeout () {
    int measurement = UpdateStatusPower ();
    if (measurement != powerStatus) {
        powerStatus = measurement;
        emit PowerStatusChange (powerStatus);

        if (powerStatus == 0) {
            reverse = TIME_TO_SHUTDOWN;
            powerOffTimer->start (1*1000);
        } else {
            powerOffTimer->stop ();
        }
    }
}
//--------------------------------------------------------------------
int PowerControl::RegisterService () {
    new PMInterfaceAdaptor (this);
    QDBusConnection connection = QDBusConnection::connectToBus (QDBusConnection::SystemBus, serviceName);

    if (!connection.isConnected ()) {
        return -1;
    }

    if (!connection.registerObject (servicePath, this)) {
        return -2;
    }

    if (!connection.registerService (serviceName)) {
        return -3;
    }

    return 0;
}

//--------------------------------------------------------------------
void PowerControl::run () {
    voltageOnBattery = -1;
    powerStatus = -1;

    int registered = RegisterService ();
    if (registered != 0) {
        qDebug() << "unable register service " << registered;
    }

    timeoutTimer = new QTimer (this);
    connect (timeoutTimer, SIGNAL (timeout ()), SLOT (Timeout ()));
    timeoutTimer->setSingleShot (false);
    timeoutTimer->start (1*1000);

    powerOffTimer = new QTimer (this);
    powerOffTimer->setSingleShot (false);
    connect (powerOffTimer, SIGNAL (timeout()), SLOT (ReverseTimeout()));


    exec ();
}
//--------------------------------------------------------------------
