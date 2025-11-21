/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "nodesdockwidget.h"

#include <QGraphicsScene>

#include <QContextMenuEvent>
#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QStandardPaths>

#include "nodesdockwidgetcontent.h"

namespace xFlow {

NodesDockWidget::NodesDockWidget(NodeEditorView *view, QWidget *parent)
    : QDockWidget(parent)
{
    setWindowTitle(tr("Nodes"));
    setWidget(new NodesDockWidgetContent(view, this));
}

NodesDockWidget::~NodesDockWidget() {}

} // namespace xFlow