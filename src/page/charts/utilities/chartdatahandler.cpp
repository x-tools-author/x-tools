/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "chartdatahandler.h"

#include <QDebug>
#include <QPointF>
#include <QStackedLayout>
#include <QTimer>
#include <QtMath>

ChartDataHandler::ChartDataHandler(QObject *parent)
    : QThread(parent)
    , m_binaryTail(QByteArray::fromHex("0000807f"))
{

}

ChartDataHandler::~ChartDataHandler()
{
    exit();
    wait();
}

void ChartDataHandler::inputBytes(const QByteArray &bytes)
{
    if (!bytes.isEmpty()) {
        emit input2run(bytes);
    }
}

QVariantMap ChartDataHandler::save()
{
    return QVariantMap();
}

void ChartDataHandler::load(const QVariantMap &parameters)
{
    Q_UNUSED(parameters);
}

void ChartDataHandler::run()
{
    m_cache.clear();
    QTimer *timer = new QTimer();

    QVariantMap parameters = save();
    int dataFormat = parameters.value("dataType").toInt();
    connect(this, &ChartDataHandler::input2run, timer, [this](const QByteArray &bytes) {
        this->m_cache.append(bytes);
    });

    connect(timer, &QTimer::timeout, timer, [this, timer, dataFormat] {
        if (dataFormat == static_cast<int>(DataFormat::BinaryY)) {
            handleBinaryY(this->m_cache);
        } else if (dataFormat == static_cast<int>(DataFormat::TextY)) {
            handleTextY(this->m_cache);
        } else if (dataFormat == static_cast<int>(DataFormat::BinaryXY)) {
            handleBinaryXY(this->m_cache);
        } else if (dataFormat == static_cast<int>(DataFormat::TextXY)) {
            handleTextXY(this->m_cache);
        } else {
            qWarning() << "Invalid data format!";
            emit outputBytes(QByteArray("Invalid data format!"));
        }

        timer->start();
    });
    timer->setSingleShot(true);
    timer->setInterval(50);
    timer->start();

    exec();

    timer->stop();
    timer->deleteLater();
}

void ChartDataHandler::handleBinaryY(QByteArray &bytes)
{
    while (bytes.indexOf(m_binaryTail) != -1 && !bytes.isEmpty()) {
        int index = bytes.indexOf(m_binaryTail);
        QByteArray yOfPoints = bytes.left(index);
        bytes.remove(0, index + m_binaryTail.size());

        if (yOfPoints.size() % 4 != 0) {
            qWarning() << "Data error: " << yOfPoints;
            emit outputBytes(QByteArray("Data error: ") + yOfPoints);
            continue;
        }

        QList<double> values;
        for (int i = 0; i < yOfPoints.size(); i += 4) {
            QByteArray tmp = yOfPoints.mid(i, 4);
            values.append(*reinterpret_cast<float *>(tmp.data()));
        }
        emit newValues(values);
    }
}

void ChartDataHandler::handleTextY(QByteArray &bytes)
{
    // 1.000,2.000,3.000,4.000,5.000\n
    while (bytes.indexOf('\n') != -1 && !bytes.isEmpty()) {
        int index = bytes.indexOf('\n');
        QByteArray yOfPoints = bytes.left(index);
        bytes.remove(0, index + 1);

        QString str = QString::fromLatin1(yOfPoints);
        if (str.isEmpty()) {
            qWarning() << "Data error: " << yOfPoints;
            emit outputBytes(QByteArray("Data error: ") + yOfPoints);
            continue;
        }

        QStringList yList = str.split(',', Qt::SkipEmptyParts);
        QList<double> values;
        for (QString &y : yList) {
            values.append(y.trimmed().toDouble());
        }
        emit newValues(values);
    }
}

void ChartDataHandler::handleBinaryXY(QByteArray &bytes)
{
    while (bytes.indexOf(m_binaryTail) != -1 && !bytes.isEmpty()) {
        int index = bytes.indexOf(m_binaryTail);
        QByteArray xyOfPoints = bytes.left(index);
        bytes.remove(0, index + m_binaryTail.size());

        if (xyOfPoints.size() % 8 != 0) {
            emit outputBytes(QByteArray("Data error: ") + xyOfPoints);
            continue;
        }

        QList<QPointF> points;
        for (int i = 0; i < xyOfPoints.size(); i += 8) {
            QByteArray tmp = xyOfPoints.mid(i, 8);
            float x = *reinterpret_cast<float *>(tmp.data());

            tmp.remove(0, 4);
            float y = *reinterpret_cast<float *>(tmp.data());
            points.append(QPointF(x, y));
        }
        emit newPoints(points);
    }
}

void ChartDataHandler::handleTextXY(QByteArray &bytes)
{
    //1.000,2.000,3.000,4.000\n
    while (bytes.indexOf('\n') != -1 && !bytes.isEmpty()) {
        int index = bytes.indexOf('\n');
        QByteArray xyOfPoints = bytes.left(index);
        bytes.remove(0, index + 1);

        QString str = QString::fromLatin1(xyOfPoints);
        if (str.isEmpty()) {
            emit outputBytes(QByteArray("Data error: ") + xyOfPoints);
            continue;
        }

        QStringList xyList = str.split(',');
        QList<QPointF> points;
        for (int i = 0; i < xyList.size(); i += 2) {
            points.append(QPointF(xyList.at(i).trimmed().toDouble(), xyList.at(i + 1).toDouble()));
        }
        emit newPoints(points);
    }
}
