/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "plotdatahandler.h"

#include <QDebug>
#include <QPointF>
#include <QStackedLayout>
#include <QTimer>
#include <QtMath>

#include "common/xtools.h"

PlotDataHandler::PlotDataHandler(QObject *parent)
    : QThread(parent)
    , m_binaryTail(QByteArray::fromHex("0000807f"))
{
    m_dataFormat = static_cast<int>(DataFormat::Text);
}

PlotDataHandler::~PlotDataHandler()
{
    exit();
    wait();
}

int PlotDataHandler::dataFormat() const
{
    m_dataFormatMutex.lock();
    int tmp = m_dataFormat;
    m_dataFormatMutex.unlock();
    return tmp;
}

void PlotDataHandler::setDataFormat(int type)
{
    m_dataFormatMutex.lock();
    m_dataFormat = type;
    m_dataFormatMutex.unlock();
}

void PlotDataHandler::inputBytes(const QByteArray &bytes)
{
    if (!bytes.isEmpty()) {
        m_cacheMutex.lock();
        m_cache.append(bytes);
        m_cacheMutex.unlock();
    }
}

void PlotDataHandler::setupDataFormat(QComboBox *comboBox)
{
    if (!comboBox) {
        return;
    }

    comboBox->addItem(tr("Binary") + "-Y", static_cast<int>(DataFormat::Binary));
    comboBox->addItem(tr("Text") + "-Y", static_cast<int>(DataFormat::Text));
    connect(comboBox, xComboBoxActivated, this, [=]() {
        int type = comboBox->currentData().toInt();
        setDataFormat(type);
    });
}

void PlotDataHandler::run()
{
    m_cacheMutex.lock();
    m_cache.clear();
    m_cacheMutex.unlock();

    QTimer *timer = new QTimer();
    connect(timer, &QTimer::timeout, timer, [this, timer] {
        int dataFormat = this->dataFormat();
        this->m_cacheMutex.lock();
        if (dataFormat == static_cast<int>(DataFormat::Binary)) {
            handleBinary(this->m_cache);
        } else if (dataFormat == static_cast<int>(DataFormat::Text)) {
            handleText(this->m_cache);
        } else {
            qWarning() << "Invalid data format!";
        }
        this->m_cacheMutex.unlock();
        timer->start();
    });

    timer->setSingleShot(true);
    timer->setInterval(50);
    timer->start();

    exec();

    timer->stop();
    timer->deleteLater();
}

void PlotDataHandler::handleBinary(QByteArray &bytes)
{
    while (bytes.indexOf(m_binaryTail) != -1 && !bytes.isEmpty()) {
        int index = bytes.indexOf(m_binaryTail);
        QByteArray yOfPoints = bytes.left(index);
        bytes.remove(0, index + m_binaryTail.size());

        if (yOfPoints.size() % 4 != 0) {
            qWarning() << "Data error: " << yOfPoints;
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

void PlotDataHandler::handleText(QByteArray &bytes)
{
    // 1.000,2.000,3.000,4.000,5.000\n
    while (bytes.indexOf('\n') != -1 && !bytes.isEmpty()) {
        int index = bytes.indexOf('\n');
        QByteArray yOfPoints = bytes.left(index);
        bytes.remove(0, index + 1);

        QString str = QString::fromLatin1(yOfPoints);
        if (str.isEmpty()) {
            qWarning() << "Data error: " << yOfPoints;
            continue;
        }

        QStringList yList = str.split(',', xSkipEmptyParts);
        QList<double> values;
        for (QString &y : yList) {
            values.append(y.trimmed().toDouble());
        }
        emit newValues(values);
    }
}
