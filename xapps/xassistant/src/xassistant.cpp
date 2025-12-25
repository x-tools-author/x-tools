/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xassistant.h"

#include <QMenuBar>
#include <QtGui/qaction.h>

#include "page/page.h"
#include "pipe.h"

xAssistant::xAssistant(QWidget* parent)
    : MainWindow(parent)
    , m_pipe(nullptr)
{
    QMenuBar* menuBar = this->menuBar();
    QList<QAction*> actions = menuBar->actions();
    for (auto& action : const_cast<QList<QAction*>&>(actions)) {
        if (!action->menu()) {
            continue;
        }

        QMenu* menu = action->menu();
        if (menu->objectName() == "ViewMenu") {
            QList<QAction*> actions = menu->actions();
            for (auto& action : const_cast<QList<QAction*>&>(actions)) {
                if (action->objectName() == QString("PageViewAction")) {
                    menu->removeAction(action);
                }
            }
        }
    }

    hideHistoryAction();

    m_pipe = new Pipe(m_ioPage00, m_ioPage01, this);
    Q_UNUSED(m_pipe);
}

QUrl xAssistant::storeUrl() const
{
    return QUrl("https://apps.microsoft.com/detail/9n5n5n35vqjf");
}
