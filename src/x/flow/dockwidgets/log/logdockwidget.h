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

class LogDockWidgetContent;
class LogDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    LogDockWidget(QWidget *parent = nullptr);
    ~LogDockWidget() override;

private:
    LogDockWidgetContent *m_content;
};
