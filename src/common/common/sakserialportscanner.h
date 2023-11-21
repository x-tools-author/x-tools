/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKSERIALPORTSCANNER_H
#define SAKSERIALPORTSCANNER_H

#include <QObject>
#include <QTimer>

class SAKSerialPortScanner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList portNames READ portNames NOTIFY portNamesChanged)
    Q_PROPERTY(QStringList baudRates READ baudRates NOTIFY baudRatesChanged)
public:
    explicit SAKSerialPortScanner(QObject *parent = nullptr);

    Q_INVOKABLE void refresh();
    Q_INVOKABLE void setIgnoredBusyDevice(bool ignored);
    Q_INVOKABLE void setAutoUpdatePortNames(bool autoUpdate);
    Q_INVOKABLE void setIgnoredUpdate(bool ignored);

private:
    bool mIgnoredBusyDevice{false};
    bool mIgnoredUpdate{false};
    QTimer *nAutoUpdatePortNamesTimer;

private:
    bool isBusy(const QString &portName);

private:
    QStringList mPortNames;
    QStringList portNames() { return mPortNames; }
    Q_SIGNAL void portNamesChanged();

    QStringList mBaudRates;
    QStringList baudRates() { return mBaudRates; }
    Q_SIGNAL void baudRatesChanged();
};

#endif // EDQMLSERIALPORTASSISTANT_HH
