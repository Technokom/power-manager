#include "pminterfaceadaptor.h"



PMInterfaceAdaptor::PMInterfaceAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    setAutoRelaySignals(true);
}



PMInterfaceAdaptor::~PMInterfaceAdaptor()
{

}



void PMInterfaceAdaptor::PowerOff()
{
    QMetaObject::invokeMethod(parent(), "PowerOff");
}



void PMInterfaceAdaptor::Reboot()
{
    QMetaObject::invokeMethod(parent(), "Reboot");
}



int PMInterfaceAdaptor::CurrentStatus()
{
    return qvariant_cast<int>(parent()->property("CurrentStatus"));
}

