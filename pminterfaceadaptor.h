#ifndef PMINTERFACEADAPTOR_H
#define PMINTERFACEADAPTOR_H

#include <QtDBus>
#include <QObject>

class PMInterfaceAdaptor : public QDBusAbstractAdaptor
{

    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "ru.tknav.PowerManagerInterface")
    Q_CLASSINFO("D-Bus Introspection", ""
        "<interface name=\"ru.tknav.PowerManagerInterface\">\n"
            "<method name=\"PowerOff\"/>\n"
            "<method name=\"Reboot\"/>\n"
            "<property access=\"read\" type=\"i\" name=\"CurrentStatus\"/>\n"
            "<signal name=\"PowerStatusChangeCaps\">\n"
                "<arg direction=\"out\" type=\"i\" name=\"status\" />\n"
            "</signal>\n"
            "<signal name=\"PowerStatusChangeV2\">\n"
                "<arg direction=\"out\" type=\"i\" name=\"status\" />\n"
            "</signal>\n"
            "<signal name=\"TimeToShutdown\">\n"
                "<arg direction=\"out\" type=\"i\" name=\"time\" />\n"
            "</signal>"
        "</interface>\n"
                "")

public:
    PMInterfaceAdaptor(QObject *parent);
    virtual ~PMInterfaceAdaptor();

    Q_PROPERTY(int CurrentStatus READ CurrentStatus)
    int CurrentStatus();

public:
public Q_SLOTS:
    void PowerOff();
    void Reboot();

Q_SIGNALS:
    void PowerStatusChangeCaps (int status);
    void PowerStatusChange(int status);
    void TimeToShutdown(int time);
};

#endif // PMINTERFACEADAPTOR_H
