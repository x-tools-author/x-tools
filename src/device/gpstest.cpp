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

/*

以下为常见 GPS 消息的结构简述（以 NMEA 0183 为主）：

基本格式：$<TalkerID><MessageType>,<Field1>,<Field2>,...*<Checksum><CR><LF>

以“$”开头，逗号分隔字段，最后“*”后跟 2 位十六进制异或校验，行以回车换行结束。
TalkerID：两字符数据源标识，如 GP（GPS）、GN（多系统）、GL（GLONASS）。
MessageType：三字符语句类型，如 RMC、GGA、GSV 等。
常见语句与关键字段：

GGA（定位数据）：时间、纬度、经度、定位质量、卫星数、HDOP、海拔、地球椭球差、高度单位。
RMC（推荐最简数据）：时间、状态、纬度/经度、地面速度、航向、日期、磁偏角、模式。
GSA（DOP与活动卫星）：定位模式、维度（2D/3D）、参与解算的卫星 PRN、PDOP/HDOP/VDOP。
GSV（可见卫星）：总消息数、序号、可见卫星数、每颗卫星的 PRN、仰角、方位角、信噪比。
VTG（地面航迹与速度）：航向（真/磁）、速度（节/公里每小时）。
编码与单位：

经纬度常用度分格式（ddmm.mmmm），需结合 N/S、E/W 方向位转换为十进制度。
时间日期为 UTC。
速度单位常见为节与 km/h；高度单位为米。
扩展/其他协议：

NMEA 2000（CAN 总线，二进制 PGN）。
厂商二进制协议，如 u-blox UBX（同步头、类/ID、长度、负载、校验）。
通常解析流程：按行读取→验证校验和→按逗号分割→根据 MessageType 映射字段→单位与格式转换。

GPT-5 • 1x

*/

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