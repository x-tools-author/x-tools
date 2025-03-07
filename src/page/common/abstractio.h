/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <atomic>

#include <QMutex>
#include <QThread>
#include <QVariantMap>

class AbstractIO : public QThread
{
    Q_OBJECT
public:
    explicit AbstractIO(QObject *parent = Q_NULLPTR);
    virtual ~AbstractIO();
    virtual void inputBytes(const QByteArray &bytes);
    virtual QVariantMap save() const;
    virtual void load(const QVariantMap &data);

    bool isEnable();
    void setIsEnable(bool enable);

signals:
    void outputBytes(const QByteArray &bytes);
    void warningOccurred(const QString &warningString);
    void errorOccurred(const QString &errorString);

    void isWorkingChanged();
    void isEnableChanged();

protected:
    std::atomic_bool m_enable{true};

private:
    QVariantMap m_parameters;
    mutable QMutex m_parametersMutex;
};
