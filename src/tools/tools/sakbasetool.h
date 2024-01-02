/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
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
    Q_PROPERTY(bool enable READ enable WRITE setEnable NOTIFY enableChanged)
public:
    explicit SAKBaseTool(QObject *parent = Q_NULLPTR);
    virtual ~SAKBaseTool();
    QString toolName() { return mToolName; }
    void setToolName(const QString &name) { mToolName = name; }
    int toolType() { return m_type; }
    void setToolType(int type) { m_type = type; }

    virtual void inputBytes(const QByteArray &bytes);

signals:
    void bytesOutput(const QByteArray &bytes);
    void errorOccured(const QString &errorString);

protected:
    QString mToolName{""};

protected:
    void outputMessage(int type, const QString &info) const;

public:
    bool isWorking() { return mIsWorking; }
    bool enable() { return mEnable; }
    void setEnable(bool enable)
    {
        mEnable = enable;
        emit enableChanged();
    }

protected:
    std::atomic_bool mIsWorking{false};
    std::atomic_bool mEnable{true};
    int m_type;

signals:
    void isWorkingChanged();
    void enableChanged();
};

#endif // SAKBASETOOL_H
