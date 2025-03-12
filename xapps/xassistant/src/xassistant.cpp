/***************************************************************************************************
 * Copyright 2018-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xassistant.h"

#include <QMenuBar>

#include "page/page.h"

xAssistant::xAssistant(QWidget* parent)
    : MainWindow(parent)
{
    QMenuBar* menuBar = this->menuBar();
    QList<QAction*> actions = menuBar->actions();
    for (QAction* action : actions) {
        if (!action->menu()) {
            continue;
        }

        QMenu* menu = action->menu();
        if (menu->objectName() == "ViewMenu") {
            QList<QAction*> actions = menu->actions();
            for (QAction* action : actions) {
                if (action->objectName() == QString("PageViewAction")) {
                    menu->removeAction(action);
                }
            }
        }
    }

    m_iopage00->hideTransferWidgets();
    m_iopage01->hideTransferWidgets();
    m_iopage10->hideTransferWidgets();
    m_iopage11->hideTransferWidgets();

    m_iopage00->hideChartsWidgets();
    m_iopage01->hideChartsWidgets();
    m_iopage10->hideChartsWidgets();
    m_iopage11->hideChartsWidgets();
}

xAssistant::~xAssistant() {}

QUrl xAssistant::storeUrl() const
{
    return QUrl("https://www.microsoft.com/store/apps/9n5n5n35vqjf");
}