/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xlog.h"

#include <QHBoxLayout>

#include "log.h"

namespace xLog {

xLog::xLog(QWidget* parent)
    : xPage(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    Log& log = Log::singleton();
    layout->addWidget(log.logView());
}

xLog::~xLog() {}

QJsonObject xLog::save() const
{
    return Log::singleton().save();
}

void xLog::load(const QJsonObject& obj)
{
    Log::singleton().load(obj);
}

}; // namespace xLog