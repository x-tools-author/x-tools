/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QComboBox>
#include <QJsonObject>
#include <QList>
#include <QString>

#include "device.h"

struct GpsTestParameters
{
    int gnss;
    double latitude;
    double longitude;
    double altitude;
    QString localFormat;
    QString altitudeUnit;
};

enum class GnssType { Gnss_GPS, Gnss_BDS, Gnss_GLONASS, Gnss_Galileo, Gnss_QZSS, Gnss_SBAS };

QList<int> supportedGnssTypes();
QString gnssTypeName(int gnssType);
void setupGnssTypes(QComboBox *comboBox);

GpsTestParameters loadGpsTestParameters(const QJsonObject &parameters);
QJsonObject saveGpsTestParameters(const GpsTestParameters &parameters);

class GpsTest : public Device
{
    Q_OBJECT
public:
    explicit GpsTest(QObject *parent = nullptr);
    ~GpsTest() override;

protected:
    void run() override;

private:
    int m_step{0};
};