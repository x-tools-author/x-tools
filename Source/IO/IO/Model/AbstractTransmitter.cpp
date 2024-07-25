/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "AbstractTransmitter.h"

#include <QDebug>
#include <QTimer>

#include "../Communication/Communication.h"

namespace xTools {

AbstractTransmitter::AbstractTransmitter(QObject *parent)
    : AbstractModel{parent}
{
    connect(m_tableModel,
            &QAbstractTableModel::dataChanged,
            this,
            &AbstractTransmitter::onDataChanged);
}

Communication *AbstractTransmitter::communicationTool(int index)
{
    Communication *tool = Q_NULLPTR;
    m_toolsMutex.lock();
    if (index >= 0 && index < m_tools.count()) {
        tool = m_tools.at(index);
    }
    m_toolsMutex.unlock();

    return tool;
}

void AbstractTransmitter::inputBytes(const QByteArray &bytes)
{
    m_toolsMutex.lock();
    for (auto tool : m_tools) {
        tool->inputBytes(bytes);
    }
    m_toolsMutex.unlock();
}

int AbstractTransmitter::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    int ret = m_tools.length();
    return ret;
}

bool AbstractTransmitter::removeRows(int row, int count, const QModelIndex &parent)
{
    if (m_tools.isEmpty()) {
        return true;
    }

    Q_UNUSED(parent)
    bool isValidRow = (row >= 0 && row < m_tools.count());
    Q_ASSERT_X(isValidRow, __FUNCTION__, "invalid row");

    bool isValidCount = (count >= 0 && count <= m_tools.count());
    Q_ASSERT_X(isValidCount, __FUNCTION__, "invalid count");

    if (!(isValidCount && isValidRow)) {
        return false;
    }

    while (count--) {
        auto tool = m_tools.takeAt(row);
        tool->exit();
        tool->wait();
        tool->deleteLater();
        tool = nullptr;
    }

    return true;
}

bool AbstractTransmitter::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent)

    auto initTool = [=](Communication *tool) {
        tool->setParent(this);
        connect(this, &Communication::outputBytes, tool, &Communication::inputBytes);
        connect(this, &Communication::started, tool, [=]() { tool->start(); });
        connect(this, &Communication::finished, tool, [=]() { tool->exit(); });

        connect(tool, &Communication::finished, this, [=]() {
            // Reboot the transmitter thread if tool box is wroking.
            if (this->isRunning()) {
                QTimer::singleShot(1 * 1000, tool, [=]() {
                    qDebug() << "reboot...";
                    tool->start();
                });
            }
        });
    };

    for (int i = 0; i < count; i++) {
        m_toolsMutex.lock();
        auto tool = createTool();
        initTool(tool);
        m_tools.insert(row, tool);
        m_toolsMutex.unlock();
    }

    return true;
}

void AbstractTransmitter::onDataChanged(const QModelIndex &topLeft,
                                        const QModelIndex &bottomRight,
                                        const QVector<int> &roles)
{
    Q_UNUSED(bottomRight)
    Q_UNUSED(roles)
    Q_UNUSED(topLeft)
#if 0
    int index = topLeft.row();
    if (index >= 0 && index < mToolVector.count()) {
        auto tool = mToolVector.at(index);
        tool->exit();
        tool->wait();
        tool->start();
    }
#endif
}

} // namespace xTools
