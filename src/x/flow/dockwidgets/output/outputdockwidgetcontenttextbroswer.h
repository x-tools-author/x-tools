/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QTextBrowser>

class OutputDockWidgetContextTextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    OutputDockWidgetContextTextBrowser(QWidget *parent = nullptr);
    ~OutputDockWidgetContextTextBrowser() override;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};
