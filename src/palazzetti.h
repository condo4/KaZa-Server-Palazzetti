#ifndef PALAZZETTI_H
#define PALAZZETTI_H

#include <QtQml/qqmlregistration.h>
#include <QObject>

class PalazzettiPrivate;

class Palazzetti : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString hostname     READ hostname       WRITE setHostname       NOTIFY hostnameChanged      FINAL)
    Q_PROPERTY(int refreshRate      READ refreshRate    WRITE setRefreshRate    NOTIFY refreshRateChanged   FINAL)

    Q_PROPERTY(int currentSetPoint READ currentSetPoint NOTIFY currentSetPointChanged)
    Q_PROPERTY(int feeder READ feeder NOTIFY feederChanged)
    Q_PROPERTY(int globalCounter READ globalCounter NOTIFY globalCounterChanged)
    Q_PROPERTY(int lastempty READ lastempty NOTIFY lastemptyChanged)
    Q_PROPERTY(float temperatureIn READ temperatureIn NOTIFY temperatureInChanged)
    Q_PROPERTY(float temperatureOut READ temperatureOut NOTIFY temperatureOutChanged)
    Q_PROPERTY(float temperatureExhaust READ temperatureExhaust NOTIFY temperatureExhaustChanged)
    Q_PROPERTY(int state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool alarm READ alarm NOTIFY alarmChanged)
    Q_PROPERTY(int power READ power NOTIFY powerChanged)
    Q_PROPERTY(int roomfan READ roomfan NOTIFY roomfanChanged)

    Q_DECLARE_PRIVATE(Palazzetti)
    QML_ELEMENT
    Q_DISABLE_COPY(Palazzetti)
    QScopedPointer<PalazzettiPrivate> const d_ptr;

public:
    explicit Palazzetti(QObject *parent = nullptr);
    ~Palazzetti() override;
    void setHostname(const QString &newHostname);
    QString hostname() const;

    void setRefreshRate(int newRefreshRate);
    int refreshRate() const;

    int currentSetPoint() const;
    int feeder() const;
    int globalCounter() const;
    float temperatureIn() const;
    float temperatureOut() const;
    float temperatureExhaust() const;
    int state() const;
    bool alarm() const;
    int power() const;
    int roomfan() const;
    int lastempty() const;

public slots:
    void refresh();
    void setPoint(int order);
    void setPower(int power);
    void setFan(int fan);

signals:
    void hostnameChanged();
    void connectedChanged();
    void refreshRateChanged();

    void currentSetPointChanged(int currentSetPoint);
    void feederChanged(int feeder);
    void globalCounterChanged(int globalCounter);
    void temperatureInChanged(int temperatureIn);
    void temperatureOutChanged(int temperatureOut);
    void temperatureExhaustChanged(int temperatureExhaust);
    void stateChanged(int state);
    void alarmChanged(bool alarm);
    void powerChanged(int power);
    void roomfanChanged(int roomfan);
    void lastemptyChanged(int lastempty);

private:
    void _processResponse();
};

#endif // PALAZZETTI_H
