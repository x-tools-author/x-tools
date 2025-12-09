/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "gpstest.h"

#include <QTimer>

struct GpsTestParameterKeys
{
    const QString gnss{"gnss"};
    const QString latitude{"latitude"};
    const QString longitude{"longitude"};
    const QString altitude{"altitude"};
    const QString localFormat{"localFormat"};
    const QString altitudeUnit{"altitudeUnit"};
};

QList<int> supportedGnssTypes()
{
    QList<int> gnssTypes;
    gnssTypes << static_cast<int>(GnssType::Gnss_GPS);
    gnssTypes << static_cast<int>(GnssType::Gnss_BDS);
    gnssTypes << static_cast<int>(GnssType::Gnss_GLONASS);
    gnssTypes << static_cast<int>(GnssType::Gnss_Galileo);
    gnssTypes << static_cast<int>(GnssType::Gnss_QZSS);
    gnssTypes << static_cast<int>(GnssType::Gnss_SBAS);
    return gnssTypes;
}

QString gnssTypeName(int gnssType)
{
    switch (static_cast<GnssType>(gnssType)) {
    case GnssType::Gnss_GPS:
        return QString("GPS");
    case GnssType::Gnss_BDS:
        return QString("BDS");
    case GnssType::Gnss_GLONASS:
        return QString("GLONASS");
    case GnssType::Gnss_Galileo:
        return QString("Galileo");
    case GnssType::Gnss_QZSS:
        return QString("QZSS");
    case GnssType::Gnss_SBAS:
        return QString("SBAS");
    default:
        return QString("Unknown");
    }
}

void setupGnssTypes(QComboBox *comboBox)
{
    if (!comboBox) {
        return;
    }

    comboBox->clear();
    const QList<int> gnssTypes = supportedGnssTypes();
    for (int gnssType : gnssTypes) {
        comboBox->addItem(gnssTypeName(gnssType), gnssType);
    }
}

GpsTestParameters loadGpsTestParameters(const QJsonObject &parameters)
{
    GpsTestParameters params;
    const GpsTestParameterKeys keys;
    params.gnss = parameters.value(keys.gnss).toInt(static_cast<int>(GnssType::Gnss_GPS));
    params.latitude = parameters.value(keys.latitude).toDouble(0.0);
    params.longitude = parameters.value(keys.longitude).toDouble(0.0);
    params.altitude = parameters.value(keys.altitude).toDouble(0.0);
    params.localFormat = parameters.value(keys.localFormat).toString("ddmm.mmmm");
    params.altitudeUnit = parameters.value(keys.altitudeUnit).toString("meters");
    return params;
}

QJsonObject saveGpsTestParameters(const GpsTestParameters &parameters)
{
    QJsonObject obj;
    const GpsTestParameterKeys keys;
    obj.insert(keys.gnss, parameters.gnss);
    obj.insert(keys.latitude, parameters.latitude);
    obj.insert(keys.longitude, parameters.longitude);
    obj.insert(keys.altitude, parameters.altitude);
    obj.insert(keys.localFormat, parameters.localFormat);
    obj.insert(keys.altitudeUnit, parameters.altitudeUnit);
    return obj;
}

GpsTest::GpsTest(QObject *parent)
    : Device(parent)
{}

GpsTest::~GpsTest() {}

void GpsTest::run()
{
    QTimer *timer = new QTimer();
    timer->setInterval(100);
    this->m_step = 0;

    connect(timer, &QTimer::timeout, timer, [this]() {
        QByteArray nmeaSentence;
        switch (this->m_step) {
        case 0:
            nmeaSentence = "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n";
            break;
        case 1:
            nmeaSentence = "$GPGLL,4916.45,N,12311.12,W,225444,A,*1D\r\n";
            break;
        case 2:
            nmeaSentence = "$GPRMC,225446,A,4916.45,N,12311.12,W,000.5,054.7,191194,020.3,E*68\r\n";
            break;
        default:
            nmeaSentence = "$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K*48\r\n";
            break;
        }

        emit bytesRead(nmeaSentence, "GpsTest");
        this->m_step = (this->m_step + 1) % 4;
    });

    timer->start();
    emit opened();
    exec();
    emit closed();
    timer->stop();
    timer->deleteLater();
}