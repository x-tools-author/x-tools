/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "logdockwidget.h"

#include "logdockwidgetcontent.h"

LogDockWidget::LogDockWidget(QWidget *parent)
    : QDockWidget(parent)
{
    setWindowTitle(tr("Log"));

    m_content = new LogDockWidgetContent(this);
    setWidget(m_content);
}

LogDockWidget::~LogDockWidget()
{

}
