/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsBleDeviceInfoComboBox.h"

#include <QBluetoothDeviceInfo>
#include <QMessageBox>

xToolsBleDeviceInfoComboBox::xToolsBleDeviceInfoComboBox(QWidget* parent)
    : xToolsComboBox(parent)
    , m_scanner(Q_NULLPTR)
{
    m_scanner = new xToolsBleScanner(this);
    connect(m_scanner, &xToolsBleScanner::finished, this, &xToolsBleDeviceInfoComboBox::onFinished);
    connect(m_scanner, &xToolsBleScanner::started, this, &xToolsBleDeviceInfoComboBox::started);
    connect(m_scanner,
            &xToolsBleScanner::deviceDiscovered,
            this,
            &xToolsBleDeviceInfoComboBox::onDeviceDiscovered);
    connect(m_scanner,
            &xToolsBleScanner::errorOccurred,
            this,
            &xToolsBleDeviceInfoComboBox::onErrorOccurred);
}

xToolsBleDeviceInfoComboBox::~xToolsBleDeviceInfoComboBox()
{
    m_scanner->stopDiscover();
}

void xToolsBleDeviceInfoComboBox::startDiscover()
{
    clear();
    m_scanner->startDiscover();
}

void xToolsBleDeviceInfoComboBox::stopDiscover()
{
    m_scanner->stopDiscover();
}

bool xToolsBleDeviceInfoComboBox::isActive()
{
    return m_scanner->isActive();
    return false;
}

void xToolsBleDeviceInfoComboBox::setTimeoutInterval(int interval)
{
#if 0
    mScanner->setTimeoutInterval(interval);
#endif
}

void xToolsBleDeviceInfoComboBox::setNameFiltter(const QString& filtter)
{
    m_scanner->setNameFiltter(filtter);
}

void xToolsBleDeviceInfoComboBox::changeEvent(QEvent* event)
{
    xToolsComboBox::changeEvent(event);
    if ((event->type() == QEvent::EnabledChange) && isEnabled()) {
        onFinished();
    }
}

void xToolsBleDeviceInfoComboBox::onFinished()
{
    if (!isEnabled()) {
        return;
    }

    clear();
    auto infos = m_scanner->devicesInfoList();
    for (auto& info : infos) {
        QString name = m_scanner->deviceName(info);
        addItem(name, info);
    }

    emit finished();
}

void xToolsBleDeviceInfoComboBox::onDeviceDiscovered(const QBluetoothDeviceInfo& info)
{
    if (!isEnabled()) {
        return;
    }

    addItem(info.name(), QVariant::fromValue(info));
}

void xToolsBleDeviceInfoComboBox::onErrorOccurred(const QString& errStr)
{
    QMessageBox::warning(this, tr("Error Occurred"), errStr);
}
