/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "log.h"

#include <QCoreApplication>
#include <QDateTime>

#include "logfilter.h"
#include "logmodel.h"
#include "logview.h"

namespace xLog {

class LogPrivate : public QObject
{
public:
    LogPrivate(Log* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        m_model = new LogModel(q);
        m_filter = new LogFilter(q);
        m_filter->setSourceModel(m_model);
    }
    ~LogPrivate() override
    {
        if (m_logView) {
            m_logView->deleteLater();
            m_logView = nullptr;
        }
    }

public:
    LogView* m_logView{nullptr};
    LogModel* m_model{nullptr};
    LogFilter* m_filter{nullptr};

private:
    Log* q{nullptr};
};

Log::Log(QObject* parent)
    : QObject(parent)
{
    d = new LogPrivate(this);
}

Log::~Log() {}

Log& Log::singleton()
{
    static Log instance(qApp);
    return instance;
}

void Log::messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString category = QString::fromUtf8(context.category);
    xLogMgr.addLogMessage(type, category, msg, QDateTime::currentDateTime());
}

QWidget* Log::logView() const
{
    if (d->m_logView == nullptr) {
        d->m_logView = new LogView();
        d->m_logView->setModel(d->m_filter);
#if 1
        qCDebug(xLogLoggingCategory) << "This is a debug message from xTools Log module.";
        qCInfo(xLogLoggingCategory) << "This is an info message from xTools Log module.";
        qCWarning(xLogLoggingCategory) << "This is a warning message from xTools Log module.";
        qCCritical(xLogLoggingCategory) << "This is a critical message from xTools Log module.";
#endif
    }

    return d->m_logView;
}

void Log::addLogMessage(QtMsgType type,
                        const QString& category,
                        const QString& message,
                        const QDateTime& timestamp)
{
    if (d->m_logView && d->m_logView->isDisableLog()) {
        return;
    }

    d->m_model->addLogMessage(LogModel::LogMessageItem{timestamp, type, category, message});
    if (d->m_logView) {
        d->m_logView->addLogCategory(category);
    }
}

QJsonObject Log::save()
{
    QJsonObject obj;
    obj.insert("logView", d->m_logView ? d->m_logView->save() : QJsonObject{});
    return obj;
}

void Log::load(const QJsonObject& obj)
{
    if (obj.contains("logView") && d->m_logView) {
        d->m_logView->load(obj.value("logView").toObject());
    }
}

} // namespace xLog