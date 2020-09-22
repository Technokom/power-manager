#ifndef POWERCONTROL_H
#define POWERCONTROL_H

#include <QThread>
#include <QTimer>
#include <QtDBus>


struct BoardInfo
{
    QString family;
    QString name;
    int major;
    int minor;

    BoardInfo () {
        family = "som";
        name = "mb";
        major = 1;
        minor = 0;
    }
};



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

private slots:
    void OnReadyPowerPin (int sock);

signals:
    void PowerStatusChange (int state);
    void PowerStatusChangeCaps (int state);
    void TimeToShutdown (int time);

private:
    int voltageOnBattery;
    int powerStatus;
    int reverse;
    QFile* powerPin;

    const QString   serviceName;
    const QString   servicePath;
    const BoardInfo versionBoard;

    QTimer *powerOffTimer;
    QTimer *timeoutTimer;
    int UpdateStatusPower ();
    int RegisterService ();
    void correctRtc ();
    BoardInfo GetVersionBoard();
private slots:
    void TimeoutDebounce (void);
    void Timeout (void);
    void ReverseTimeout ();
};

#endif // POWERCONTROL_H
