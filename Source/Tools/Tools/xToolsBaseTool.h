/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <atomic>
#include <QJsonObject>
#include <QThread>

class xToolsBaseTool : public QThread
{
    Q_OBJECT
    Q_PROPERTY(bool isWorking READ isWorking NOTIFY isWorkingChanged)
    Q_PROPERTY(bool isEnable READ isEnable WRITE setIsEnable NOTIFY isEnableChanged)
public:
    explicit xToolsBaseTool(QObject *parent = Q_NULLPTR);
    virtual ~xToolsBaseTool();
    virtual void inputBytes(const QByteArray &bytes) = 0;

    int toolType() { return m_type; }
    void setToolType(int type) { m_type = type; }

signals:
    void outputBytes(const QByteArray &bytes);
    void errorOccurred(const QString &errorString);

public:
    bool isWorking() { return m_isWorking; }
    bool isEnable() { return m_enable; }
    void setIsEnable(bool enable)
    {
        m_enable = enable;
        emit isEnableChanged();
    }

protected:
    std::atomic_bool m_isWorking{false};
    std::atomic_bool m_enable{true};
    int m_type;

signals:
    void isWorkingChanged();
    void isEnableChanged();
};
