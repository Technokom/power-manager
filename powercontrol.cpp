#include <QFile>
#include "powercontrol.h"
#include "pminterfaceadaptor.h"
#include "bsp.h"



#define TIME_TO_SHUTDOWN        70
#define CHARGER_ONLINE_STATUS   "/sys/class/power_supply/bq25890-charger/online"



PowerControl::PowerControl (QObject *parent) :
    QThread (parent),
    serviceName ("ru.tknav.PowerManagerInterface"),
    servicePath ("/ru/tknav/PowerManagerInterface"),
    versionBoard (GetVersionBoard())
{

}



int PowerControl::UpdateStatusPower ()
{
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



void PowerControl::correctRtc (void)
{
    system ("hwclock -w");
}



void PowerControl::PowerOff (void)
{
    correctRtc ();
    system ("halt -p");
}



int PowerControl::CurrentStatus () {
    return powerStatus;
}



void PowerControl::ReverseTimeout () {
    reverse--;

    emit TimeToShutdown(reverse);

    if (reverse < 1) {
        PowerOff ();
    }
}


void PowerControl::TimeoutDebounce (void)
{
    emit PowerStatusChangeCaps (powerStatus);
}


void PowerControl::Timeout ()
{
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



int PowerControl::RegisterService ()
{
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



void PowerControl::OnReadyPowerPin (int sock)
{
    if (powerPin->handle() == sock) {
        powerPin->seek(0);
        int value = powerPin->readAll()[0] - 0x30;
        powerStatus = value;
        if (value == 0) {
            timeoutTimer->stop();
            timeoutTimer->start (1*1000);
        } else {
            timeoutTimer->stop();
            timeoutTimer->start (1*1000);
        }

    }
}



void PowerControl::run ()
{

    timeoutTimer = new QTimer (this);
    if (versionBoard.major == 1) {
        connect (timeoutTimer, SIGNAL (timeout ()), SLOT (Timeout ()));
        timeoutTimer->setSingleShot (false);
        timeoutTimer->start (1*1000);
    } else {
        Bsp::ExportPin(147);
        Bsp::SetGpioDirection(147, PinDirection::pdIn);
        Bsp::SetGpioEdge(147, PinEdge::peBoth);
        powerPin = new QFile (PATH_GPIO"147/value", this);
        connect (timeoutTimer, SIGNAL (timeout ()), SLOT (TimeoutDebounce ()));
        timeoutTimer->setSingleShot (true);

        if (powerPin->open(QIODevice::ReadOnly)) {
            QSocketNotifier *notifier = new QSocketNotifier(powerPin->handle(), QSocketNotifier::Exception, this);
            notifier->setEnabled(true);
            connect (notifier, &QSocketNotifier::activated, this, &PowerControl::OnReadyPowerPin);
        }
    }

    voltageOnBattery = -1;
    powerStatus = -1;

    RegisterService ();

    powerOffTimer = new QTimer (this);
    powerOffTimer->setSingleShot (false);
    connect (powerOffTimer, SIGNAL (timeout()), SLOT (ReverseTimeout()));


    exec ();
}



BoardInfo PowerControl::GetVersionBoard()
{
    BoardInfo result;
    QString  versionString = "som-mb-1.0";

    QFile file ("/sys/bus/i2c/devices/1-0050/eeprom");
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray fileCons = file.readAll();
            if (fileCons.at(0) != 0xff) {
                versionString = fileCons;
            }
            file.close();
        }
    }

    QStringList list = versionString.split("-");
    if (list.count() > 2) {
        result.family = list[0];
        result.name = list[1];
        result.major = list[2].split(".")[0].toInt();
        result.minor = list[2].split(".")[1].toInt();
    }

    return result;
}
