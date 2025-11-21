/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "outputdockwidgetcontent.h"
#include "ui_outputdockwidgetcontent.h"

#include <QContextMenuEvent>
#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QStandardPaths>

#include "outputdockwidgetcontenttextbroswer.h"

OutputDockWidgetContext::OutputDockWidgetContext(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OutputDockWidgetContext)
{
    ui->setupUi(this);

    for (int i = 0; i < 8; i++) {
        OutputDockWidgetContextTextBrowser *browser = new OutputDockWidgetContextTextBrowser(this);
        ui->tabWidget->addTab(browser, QString("[O%1]").arg(i + 1));
        m_outputViews.append(browser);
    }
}

OutputDockWidgetContext::~OutputDockWidgetContext()
{
    delete ui;
}

void OutputDockWidgetContext::outputBytes(const QString &txt, int channel)
{
    if (channel >= 0 && channel < m_outputViews.size() && !txt.isEmpty()) {
        QTextBrowser *view = m_outputViews[channel];
        view->append(txt);
    }
}

void OutputDockWidgetContext::clearOutput(int channel)
{
    if (channel >= 0 && channel < m_outputViews.size()) {
        QTextBrowser *view = m_outputViews[channel];
        view->clear();
    }
}
