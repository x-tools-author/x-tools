/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakmenu.h"

#include <QMouseEvent>

SAKMenu::SAKMenu(const QString& title, QWidget* parent)
    : QMenu(title, parent)
{}

SAKMenu::SAKMenu(QWidget* parent)
    : QMenu{parent}
{}

void SAKMenu::mouseReleaseEvent(QMouseEvent* e)
{
    auto p = QCursor::pos();
    if (geometry().contains(p)) {
        QAction* a = actionAt(e->pos());
        if (a) {
            a->activate(QAction::Trigger);
            return;
        }
    }

    QMenu::mouseReleaseEvent(e);
}
