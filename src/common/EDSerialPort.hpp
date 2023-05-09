/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDSERIALPORT_HPP
#define EDSERIALPORT_HPP

#include <QTimer>
#include <QObject>

class EDSerialPort : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList portNames READ portNames NOTIFY portNamesChanged)
    Q_PROPERTY(QStringList baudRates READ baudRates NOTIFY baudRatesChanged)
public:
    explicit EDSerialPort(QObject *parent = nullptr);

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
    QStringList portNames(){return mPortNames;}
    Q_SIGNAL void portNamesChanged();

    QStringList mBaudRates;
    QStringList baudRates(){return mBaudRates;}
    Q_SIGNAL void baudRatesChanged();
};

#endif // EDQMLSERIALPORTASSISTANT_HPP
