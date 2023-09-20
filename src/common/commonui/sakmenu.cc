/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include "sakmenu.h"

#include <QMouseEvent>

SAKMenu::SAKMenu(const QString& title, QWidget* parent)
    : QMenu(title, parent) {}

SAKMenu::SAKMenu(QWidget* parent) : QMenu{parent} {}

void SAKMenu::mouseReleaseEvent(QMouseEvent* e) {
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
