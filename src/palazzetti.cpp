#include "palazzetti.h"
#include <QVariant>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QSettings>

const int TCP_PORT = 80;

class PalazzettiPrivate {
    Q_DISABLE_COPY(PalazzettiPrivate)
    Q_DECLARE_PUBLIC(Palazzetti)
    Palazzetti * const q_ptr;
    QNetworkAccessManager m_manager;
    QTimer m_timer;
    QString m_hostname;
    int m_refreshRate;
    int m_port {TCP_PORT};

    int m_currentSetPoint;
    int m_feeder;
    int m_globalCounter;
    float m_temperatureIn;
    float m_temperatureOut;
    float m_temperatureExhaust;
    int m_state;
    bool m_alarm;
    int m_power;
    int m_roomfan;
    int m_lastempty;

    PalazzettiPrivate(Palazzetti * palazzetti):
        q_ptr(palazzetti) {}


    friend class Palazzetti;
};

Palazzetti::Palazzetti(QObject *parent)
    : QObject(parent)
    , d_ptr(new PalazzettiPrivate(this))
{
    Q_D(Palazzetti);
    QObject::connect(&d->m_timer,        &QTimer::timeout,                this, &Palazzetti::refresh);
    qDebug() << "Palazzetti integration loaded";

    QSettings settings;
    d->m_globalCounter = settings.value("Palazzetti/lastempty").toInt();
    refresh();
}


Palazzetti::~Palazzetti() {

}


QString Palazzetti::hostname() const
{
    Q_D(const Palazzetti);
    return d->m_hostname;
}

void Palazzetti::setHostname(const QString &newHostname)
{
    Q_D(Palazzetti);
    int port  = d->m_port;
    QString hostname = newHostname;
    if(hostname.contains(":"))
    {
        QStringList h = hostname.split(":");
        hostname = h[0];
        port = h[1].toInt();
    }

    if (d->m_hostname == hostname && d->m_port == port)
        return;
    d->m_hostname = hostname;
    d->m_port = port;
    refresh();
    emit hostnameChanged();
}


void Palazzetti::refresh()
{
    Q_D(Palazzetti);
    // Autentication
    if(!d->m_hostname.isEmpty())
    {
        QUrl url(QString("http://%1:%2/cgi-bin/sendmsg.lua?cmd=GET+ALLS").arg(d->m_hostname).arg(d->m_port));
        QNetworkRequest request(url);
        QNetworkReply *reply = d->m_manager.get(request);

        QObject::connect(reply, &QIODevice::readyRead, this, &Palazzetti::_processResponse);
        QObject::connect(reply, &QNetworkReply::errorOccurred, [reply](){
            qWarning() << "Can't access " << reply->errorString();
            qWarning() << "in " << reply->url();
            reply->deleteLater();
        });
    }
}

void Palazzetti::_processResponse() {
    Q_D(Palazzetti);
    QObject *obj = QObject::sender();
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(obj);
    if(!reply)
    {
        qWarning() << "Invalid signal";
        return;
    }
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
    if(jsonDocument.isObject() == false) {
        qWarning() << "It is not a JSON object" << jsonDocument;
    }

    QJsonObject rootObject = jsonDocument.object();
    if( !rootObject.value("SUCCESS").toBool())
    {
        qWarning() << "Request fail";
    }

    QJsonObject infoObject = rootObject.value("INFO").toObject();
    if(infoObject.value("CMD").toString() == "GET ALLS" && infoObject.value("RSP").toString() == "OK")
    {
        int idata;
        float fdata;
        QJsonObject data = rootObject.value("DATA").toObject();
        //"APLTS":"2023-03-29 14:17:06",
        //"APLWDAY":3,
        //"CHRSTATUS":0,
        //"CORE":129,
        //"DP":111,
        //"DPT":0,
        //"EFLAGS":0,
        //"F1RPM":0,
        //"F1V":0,
        idata = data.value("F2L").toInt();
        if (d->m_roomfan != idata)
        {
            d->m_roomfan = idata;
            emit roomfanChanged(d->m_roomfan);
        }
        //"F2LF":0,
        //"F2V":0,
        //"FANLMINMAX":[1,5,0,1,0,1],
        idata = data.value("FDR").toInt();
        if (d->m_feeder != idata)
        {
            d->m_feeder = idata;
            emit feederChanged(d->m_feeder);
        }
        //"FWDATE":"2016-04-18",
        //"IN":13,
        //"LSTATUS":51,
        //"MAC":"40:F3:85:70:3C:3C",
        //"MBTYPE":0,
        //"MOD":328,
        //"OUT":0,
        idata = data.value("PQT").toInt();
        if (d->m_globalCounter != idata)
        {
            d->m_globalCounter = idata;
            emit globalCounterChanged(d->m_globalCounter);
        }
        //"PUMP":0,
        idata = data.value("PWR").toInt();
        if (d->m_power != idata)
        {
            d->m_power = idata;
            emit powerChanged(d->m_power);
        }

        idata = data.value("SETP").toInt();
        if (d->m_currentSetPoint != idata)
        {
            d->m_currentSetPoint = idata;
            emit currentSetPointChanged(d->m_currentSetPoint);
        }

        idata = data.value("STATUS").toInt();
        if (d->m_state != idata)
        {
            d->m_state = idata;
            emit stateChanged(d->m_state);
        }

        fdata = data.value("T1").toDouble();
        if (d->m_temperatureIn != fdata)
        {
            d->m_temperatureIn = fdata;
            emit temperatureInChanged(d->m_temperatureIn);
        }

        fdata = data.value("T2").toDouble();
        if (d->m_temperatureOut != fdata)
        {
            d->m_temperatureOut = fdata;
            emit temperatureOutChanged(d->m_temperatureOut);
        }

        fdata = data.value("T3").toDouble();
        if (d->m_temperatureExhaust != fdata)
        {
            d->m_temperatureExhaust = fdata;
            emit temperatureExhaustChanged(d->m_temperatureExhaust);
        }
        //"T4":0,
        //"T5":0,
        //"VER":43

        if(state() == 253)
        {
            d->m_lastempty = globalCounter();
            QSettings settings;
            settings.setValue("Palazzetti/lastempty", globalCounter());
        }
    }

    reply->deleteLater();
}

int Palazzetti::refreshRate() const
{
    Q_D(const Palazzetti);
    return d->m_refreshRate;
}

void Palazzetti::setRefreshRate(int newRefreshRate)
{
    Q_D(Palazzetti);
    if (d->m_refreshRate == newRefreshRate)
        return;
    d->m_timer.setInterval(newRefreshRate * 1000);
    d->m_timer.setSingleShot(false);
    d->m_timer.start();
    d->m_refreshRate = newRefreshRate;
    emit refreshRateChanged();
}


int Palazzetti::currentSetPoint() const
{
    Q_D(const Palazzetti);
    return d->m_currentSetPoint;
}

int Palazzetti::feeder() const
{
    Q_D(const Palazzetti);
    return d->m_feeder;
}

int Palazzetti::globalCounter() const
{
    Q_D(const Palazzetti);
    return d->m_globalCounter;
}

float Palazzetti::temperatureIn() const
{
    Q_D(const Palazzetti);
    return d->m_temperatureIn;
}

float Palazzetti::temperatureOut() const
{
    Q_D(const Palazzetti);
    return d->m_temperatureOut;
}

float Palazzetti::temperatureExhaust() const
{
    Q_D(const Palazzetti);
    return d->m_temperatureExhaust;
}

int Palazzetti::state() const
{
    Q_D(const Palazzetti);
    return d->m_state;
}

bool Palazzetti::alarm() const
{
    Q_D(const Palazzetti);
    return d->m_alarm;
}

int Palazzetti::power() const
{
    Q_D(const Palazzetti);
    return d->m_power;
}

int Palazzetti::roomfan() const
{
    Q_D(const Palazzetti);
    return d->m_roomfan;
}

int Palazzetti::lastempty() const
{
    Q_D(const Palazzetti);
    return d->m_lastempty;
}


void Palazzetti::setPoint(int order)
{
    Q_D(Palazzetti);
    QUrl url(QString("http://%1:%2/cgi-bin/sendmsg.lua?cmd=SET+SETP+%3").arg(d->m_hostname).arg(d->m_port).arg(order));
    QNetworkRequest request(url);
    QNetworkReply *reply = d->m_manager.get(request);

    QObject::connect(reply, &QIODevice::readyRead, [this, reply](){
        Q_D(Palazzetti);
        if(reply)
        {
            QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
            QJsonObject rootObject = jsonDocument.object();
            if( !rootObject.value("SUCCESS").toBool())
            {
                qWarning() << "setPoint fail";
                return;
            }
            QJsonObject data = rootObject.value("DATA").toObject();
            int idata = data.value("SETP").toInt();
            if (d->m_currentSetPoint != idata)
            {
                d->m_currentSetPoint = idata;
                emit currentSetPointChanged(d->m_currentSetPoint);
            }
            reply->deleteLater();
        }
    });

    QObject::connect(reply, &QNetworkReply::errorOccurred, [reply](){
        qWarning() << "Can't access " << reply->errorString();
        qWarning() << "in " << reply->url();
        reply->deleteLater();
    });
}

void Palazzetti::setPower(int power)
{
    Q_D(Palazzetti);
    QUrl url(QString("http://%1:%2/cgi-bin/sendmsg.lua?cmd=SET+POWR+%3").arg(d->m_hostname).arg(d->m_port).arg(power));
    QNetworkRequest request(url);
    QNetworkReply *reply = d->m_manager.get(request);

    QObject::connect(reply, &QIODevice::readyRead,  [this, reply](){
        Q_D(Palazzetti);
        if(reply)
        {
            QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
            QJsonObject rootObject = jsonDocument.object();
            if( !rootObject.value("SUCCESS").toBool())
            {
                qWarning() << "setPower fail";
            }
            QJsonObject data = rootObject.value("DATA").toObject();
            int idata = data.value("PWR").toInt();
            if (d->m_power != idata)
            {
                d->m_power = idata;
                emit powerChanged(d->m_power);
            }
            reply->deleteLater();
        }
    });

    QObject::connect(reply, &QNetworkReply::errorOccurred, [reply](){
        qWarning() << "Can't access " << reply->errorString();
        qWarning() << "in " << reply->url();
        reply->deleteLater();
    });
}

void Palazzetti::setFan(int fan)
{
    Q_D(Palazzetti);
    QUrl url(QString("http://%1:%2/cgi-bin/sendmsg.lua?cmd=SET+RFAN+%3").arg(d->m_hostname).arg(d->m_port).arg(fan));
    QNetworkRequest request(url);
    QNetworkReply *reply = d->m_manager.get(request);

    QObject::connect(reply, &QIODevice::readyRead,  [this, reply](){
        Q_D(Palazzetti);
        if(reply)
        {
            QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
            QJsonObject rootObject = jsonDocument.object();
            if( !rootObject.value("SUCCESS").toBool())
            {
                qWarning() << "setFan fail";
            }
            QJsonObject data = rootObject.value("DATA").toObject();
            int idata = data.value("F2L").toInt();
            if (d->m_roomfan != idata)
            {
                d->m_roomfan = idata;
                emit roomfanChanged(d->m_roomfan);
            }
            reply->deleteLater();
        }
    });

    QObject::connect(reply, &QNetworkReply::errorOccurred, [reply](){
        qWarning() << "Can't access " << reply->errorString();
        qWarning() << "in " << reply->url();
        reply->deleteLater();
    });
}
