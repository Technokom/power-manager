#ifndef POWERCONTROL_H
#define POWERCONTROL_H

#include <QThread>
#include <QTimer>
#include <QtDBus>

class PowerControl : public QThread
{
    Q_OBJECT
    Q_PROPERTY (int CurrentStatus READ CurrentStatus)
public:
    explicit PowerControl (QObject *parent = nullptr);
    void run () override;
    int CurrentStatus ();

public slots:
    Q_NOREPLY void PowerOff (void);


signals:
    void PowerStatusChange (int state);
    void TimeToShutdown (int time);

private:
    int voltageOnBattery;
    int powerStatus;
    int reverse;

    const QString serviceName;
    const QString servicePath;

    QTimer *powerOffTimer;
    QTimer *timeoutTimer;
    int UpdateStatusPower ();
    int RegisterService ();
    void correctRtc ();
private slots:
    void Timeout (void);
    void ReverseTimeout ();
};

#endif // POWERCONTROL_H
