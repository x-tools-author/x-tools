/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "outputdockwidget.h"

#include <QContextMenuEvent>
#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QStandardPaths>

#include "outputdockwidgetcontent.h"

OutputDockWidget::OutputDockWidget(QWidget *parent)
    : QDockWidget(parent)
{
    setWindowTitle(tr("Output"));
    m_content = new OutputDockWidgetContext(this);
    setWidget(m_content);
}

OutputDockWidget::~OutputDockWidget() {}

void OutputDockWidget::outputBytes(const QString &txt, int channel)
{
    m_content->outputBytes(txt, channel);
}

void OutputDockWidget::clearOutput(int channel)
{
    m_content->clearOutput(channel);
}
