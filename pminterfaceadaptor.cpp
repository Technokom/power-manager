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

int PMInterfaceAdaptor::CurrentStatus()
{
    return qvariant_cast<int>(parent()->property("CurrentStatus"));
}

