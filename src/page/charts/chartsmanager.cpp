/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "chartsmanager.h"

#include <QDebug>
#include <QPointF>
#include <QTimer>
#include <QtMath>

#include "linecharts/linechartview.h"

ChartsManager::ChartsManager(QObject *parent)
    : QThread(parent)
    , m_binaryTail(QByteArray::fromHex("0000807f"))
{
    LineChartView *lineChartView = new LineChartView();
    m_chartViews.append(lineChartView);

    QToolButton *lineChartController = new QToolButton();
    lineChartController->setText(tr());
    m_chartControllers.append(lineChartController);
}

ChartsManager::~ChartsManager() {}

QList<QWidget *> ChartsManager::chartViews()
{
    return QList<QWidget *>();
}

QList<QToolButton *> ChartsManager::chartControllers()
{
    return QList<QToolButton *>();
}

void ChartsManager::inputBytes(const QByteArray &bytes)
{
    if (!bytes.isEmpty() && isRunning()) {
        emit input2run(bytes);
    }
}

QVariantMap ChartsManager::save()
{
    return QVariantMap();
}

void ChartsManager::load(const QVariantMap &parameters)
{
    Q_UNUSED(parameters);
}

void ChartsManager::run()
{
    QByteArray tmp;
    QTimer *timer = new QTimer();
    m_testAngle = 0;

    QVariantMap parameters = save();
    int dataFormat = parameters.value("dataType").toInt();
    bool isTestData = parameters.value("testData").toBool();
    connect(this, &ChartsManager::input2run, timer, [&tmp, this, isTestData](const QByteArray &bytes) {
        if (!isTestData) {
            tmp.append(bytes);
        }
    });

    if (isTestData) {
        connect(timer, &QTimer::timeout, timer, [&tmp, this, timer, dataFormat]() {
            if (dataFormat == static_cast<int>(DataFormat::BinaryY)) {
                tmp.append(handleBinaryY());
            } else if (dataFormat == static_cast<int>(DataFormat::TextY)) {
                tmp.append(handleTextY());
            } else if (dataFormat == static_cast<int>(DataFormat::BinaryXY)) {
                tmp.append(handleBinaryXY());
            } else if (dataFormat == static_cast<int>(DataFormat::TextXY)) {
                tmp.append(handleTextXY());
            } else {
                qWarning() << "Invalid data format(test data)!";
                emit outputBytes(QByteArray("Invalid data format(test data)!"));
            }
        });
    }

    connect(timer, &QTimer::timeout, timer, [&tmp, this, timer, dataFormat] {
        if (dataFormat == static_cast<int>(DataFormat::BinaryY)) {
            handleBinaryY(tmp);
        } else if (dataFormat == static_cast<int>(DataFormat::TextY)) {
            handleTextY(tmp);
        } else if (dataFormat == static_cast<int>(DataFormat::BinaryXY)) {
            handleBinaryXY(tmp);
        } else if (dataFormat == static_cast<int>(DataFormat::TextXY)) {
            handleTextXY(tmp);
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

QByteArray ChartsManager::handleBinaryY()
{
    QByteArray bytes;
    for (int i = 0; i < 16; ++i) {
        float y = 10 * (i + 1) * qSin(m_testAngle * 3.1415926 / 180.0);
        bytes.append(reinterpret_cast<const char *>(&y), sizeof(y));
    };

    bytes.append(m_binaryTail);
    m_testAngle += 1;
    return bytes;
}

QByteArray ChartsManager::handleTextY()
{
    QString str;
    for (int i = 0; i < 16; ++i) {
        qreal y = 10 * (i + 1) * qSin(m_testAngle * 3.1415926 / 180.0);
        str.append(QString::number(y, 'f', 3) + (i == 15 ? "" : ","));
    };

    str = str.trimmed();
    str.append("\n");
    m_testAngle += 1;
    return str.toUtf8();
}

QByteArray ChartsManager::handleBinaryXY()
{
    QByteArray bytes;
    for (int i = 0; i < 16; ++i) {
        float y = 10 * (i + 1) * qSin(m_testAngle * 3.1415926 / 180.0);
        bytes.append(reinterpret_cast<const char *>(&y), sizeof(y));
    };

    bytes.append(m_binaryTail);
    m_testAngle += 1;
    return bytes;
}

QByteArray ChartsManager::handleTextXY()
{
    QByteArray bytes;
    for (int i = 0; i < 16; ++i) {
        qreal y = 10 * (i + 1) * qSin(m_testAngle * 3.1415926 / 180.0);
        bytes.append(reinterpret_cast<const char *>(&y), sizeof(y));
    };

    bytes.append(m_binaryTail);
    m_testAngle += 1;
    return bytes;
}

void ChartsManager::handleBinaryY(QByteArray &bytes)
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

void ChartsManager::handleTextY(QByteArray &bytes)
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
        for (const QString &y : yList) {
            values.append(y.trimmed().toDouble());
        }
        emit newValues(values);
    }
}

void ChartsManager::handleBinaryXY(QByteArray &bytes)
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

void ChartsManager::handleTextXY(QByteArray &bytes)
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
