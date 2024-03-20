/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsUiInterface.h"

#include <QApplication>
#include <QIcon>
#include <QMap>
#include <QPainter>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "qapplication.h"
#include "qmainwindow.h"
#include "qobject.h"
#include "xToolsDataStructure.h"

xToolsUiInterface::xToolsUiInterface(QObject* parent)
    : QObject{parent}
{}

void xToolsUiInterface::setValidator(QLineEdit* le, int textFormat)
{
    if (!le) {
        return;
    }

    static QMap<int, QRegularExpression> regExpMap;
    if (regExpMap.isEmpty()) {
        regExpMap.insert(xToolsDataStructure::TextFormatBin,
                         QRegularExpression("([01][01][01][01][01][01][01][01][ ])*"));
        regExpMap.insert(xToolsDataStructure::TextFormatOct, QRegularExpression("([0-7][0-7][ ])*"));
        regExpMap.insert(xToolsDataStructure::TextFormatDec, QRegularExpression("([0-9][0-9][ ])*"));
        regExpMap.insert(xToolsDataStructure::TextFormatHex,
                         QRegularExpression("([0-9a-fA-F][0-9a-fA-F][ ])*"));
        regExpMap.insert(xToolsDataStructure::TextFormatAscii, QRegularExpression("([ -~])*"));
    }

    if (le->validator()) {
        delete le->validator();
    }

    if (regExpMap.contains(textFormat)) {
        QRegularExpression re = regExpMap.value(textFormat);
        auto validator = new QRegularExpressionValidator(re, le);
        le->setValidator(validator);
    } else {
        le->setValidator(nullptr);
    }

    le->clear();
}

QIcon xToolsUiInterface::cookedIcon(const QIcon& icon)
{
    QPixmap pixmap = icon.pixmap(QSize(128, 128));
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), qApp->palette().windowText().color());
    QIcon colorIcon = QIcon(pixmap);
    return colorIcon;
}

QMainWindow* xToolsUiInterface::mainWindow()
{
    for (const auto& it : qobject_cast<QApplication*>(qApp)->topLevelWidgets()) {
        auto w = qobject_cast<QMainWindow*>(it);
        if (w) {
            return w;
        }
    }

    return nullptr;
}
