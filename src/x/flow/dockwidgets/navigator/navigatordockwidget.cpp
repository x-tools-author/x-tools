/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "navigatordockwidget.h"

#include <QEvent>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QScrollBar>

#include "navigatordockwidgetcontent.h"

NavigatorDockWidget::NavigatorDockWidget(QScrollArea *scrollArea, QWidget *parent)
    : m_scrollArea(scrollArea)
{
    setWindowTitle(tr("Navigator"));
    m_content = new NavigatorDockWidgetContent(scrollArea, this);
    setWidget(m_content);
    setMaximumHeight(200);
    setMinimumWidth(256);
}

NavigatorDockWidget::~NavigatorDockWidget() {}

void NavigatorDockWidget::updateNavigator()
{
    m_content->update();
}
