/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QDockWidget>
#include <QTextBrowser>

class OutputDockWidgetContext;
class OutputDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    OutputDockWidget(QWidget *parent = nullptr);
    ~OutputDockWidget() override;

    void outputBytes(const QString &txt, int channel);
    void clearOutput(int channel);

private:
    OutputDockWidgetContext *m_content;
};
