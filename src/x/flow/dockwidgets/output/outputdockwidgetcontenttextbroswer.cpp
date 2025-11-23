/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "outputdockwidgetcontenttextbroswer.h"

#include <QContextMenuEvent>
#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QStandardPaths>

namespace xFlow {

OutputDockWidgetContextTextBrowser::OutputDockWidgetContextTextBrowser(QWidget *parent)
    : QTextBrowser(parent)
{
    document()->setMaximumBlockCount(2000);
    setReadOnly(true);
    setStyleSheet("QTextBrowser { border: none; }");
}

OutputDockWidgetContextTextBrowser::~OutputDockWidgetContextTextBrowser() {}

void OutputDockWidgetContextTextBrowser::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();
    menu->addSeparator();
    menu->addAction(tr("Clear"), this, [=]() { clear(); });
    menu->addAction(tr("Save Current"), this, [=]() {
        auto text = toPlainText();
        auto location = QStandardPaths::DesktopLocation;
        auto fileName = QFileDialog::getSaveFileName(this,
                                                     tr("Save"),
                                                     QStandardPaths::writableLocation(location),
                                                     tr("Text (*.txt)"));
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(text.toUtf8());
                file.close();
            } else {
                QMessageBox::warning(this, tr("Warning"), tr("Failed to save file!"));
            }
        }
    });

    menu->exec(event->globalPos());
    menu->deleteLater();
}

} // namespace xFlow