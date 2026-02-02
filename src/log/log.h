/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QDateTime>
#include <QLoggingCategory>
#include <QObject>
#include <QWidget>

#include "utilities/serializable.h"

#define xLogMgr xLog::Log::singleton()
#define xLogLoggingCategory QLoggingCategory("xTools.log")

namespace xLog {

class LogPrivate;
class Log : public QObject, public xTools::Serializable
{
    Q_OBJECT
    LogPrivate* d{nullptr};

private:
    Log(QObject* parent = nullptr);
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

public:
    ~Log() override;

    static Log& singleton();
    static void messageHandler(QtMsgType, const QMessageLogContext&, const QString&);

    QWidget* logView() const;
    void addLogMessage(QtMsgType type,
                       const QString& category,
                       const QString& message,
                       const QDateTime& timestamp);

    QJsonObject save() override;
    void load(const QJsonObject& obj) override;
};

} // namespace xLog