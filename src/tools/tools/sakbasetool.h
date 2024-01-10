/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKBASETOOL_H
#define SAKBASETOOL_H

#include <atomic>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QThread>

class SAKBaseTool : public QThread
{
    Q_OBJECT
    Q_PROPERTY(bool isWorking READ isWorking NOTIFY isWorkingChanged)
    Q_PROPERTY(bool isEnable READ isEnable WRITE setIsEnable NOTIFY isEnableChanged)
public:
    explicit SAKBaseTool(QObject *parent = Q_NULLPTR);
    virtual ~SAKBaseTool();
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

#endif // SAKBASETOOL_H
