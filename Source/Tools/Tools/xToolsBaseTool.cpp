/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsBaseTool.h"

#include <QDateTime>
#include <QDebug>
#include <QtGlobal>

xToolsBaseTool::xToolsBaseTool(QObject *parent)
    : QThread{parent}
{
    connect(this, &xToolsBaseTool::started, this, [=]() {
        this->m_isWorking = true;
        emit this->isWorkingChanged();
    });
    connect(this, &xToolsBaseTool::finished, this, [=]() {
        this->m_isWorking = false;
        emit this->isWorkingChanged();
    });
    connect(this, &xToolsBaseTool::errorOccurred, this, [=](const QString &errorString) {
        qWarning() << "Error occured: " << errorString;
        exit();
        wait();
    });
}

xToolsBaseTool::~xToolsBaseTool()
{
    if (isRunning()) {
        exit();
        wait();
    }
}

QVariantMap xToolsBaseTool::save() const
{
    QVariantMap data;
    data["isEnable"] = m_enable.load();
    return data;
}

void xToolsBaseTool::load(const QVariantMap &data)
{
    if (data.isEmpty()) {
        return;
    }

    bool isEnable = data["isEnable"].toBool();
    m_enable.store(isEnable);
    emit isEnableChanged();
}

bool xToolsBaseTool::isWorking()
{
    return m_isWorking;
}

bool xToolsBaseTool::isEnable()
{
    return m_enable;
}

void xToolsBaseTool::setIsEnable(bool enable)
{
    m_enable = enable;
    emit isEnableChanged();
}
