/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
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
    bool m_ignoredBusyDevice{false};
    bool m_ignoredUpdate{false};
    QTimer *m_autoUpdatePortNamesTimer;

private:
    bool isBusy(const QString &portName);

private:
    QStringList m_portNames;
    QStringList portNames() { return m_portNames; }
    Q_SIGNAL void portNamesChanged();

    QStringList m_baudRates;
    QStringList baudRates() { return m_baudRates; }
    Q_SIGNAL void baudRatesChanged();
};

#endif // EDQMLSERIALPORTASSISTANT_HH
