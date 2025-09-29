/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xdebug.h"

#include <QQuickTextDocument>

#include "common/xtools.h"

#if defined(_MSC_VER)
#include <dwmapi.h>

#include <QColor>
#include <QSysInfo>
#include <QWindow>
#endif

xDebug::xDebug(int argc, char **argv)
    : Application(argc, argv)
{}

xDebug::~xDebug() {}

QString xDebug::version()
{
    return Application::applicationVersion();
}

QString xDebug::bytes2string(const QByteArray &bytes, int format)
{
    return ::bytes2string(bytes, format);
}

QByteArray xDebug::string2bytes(const QString &txt, int format)
{
    return ::string2bytes(txt, format);
}

QByteArray xDebug::arrayAppendArray(const QByteArray &array1, const QByteArray &array2)
{
    return ::arrayAppendArray(array1, array2);
}

QString xDebug::cookedEscapeCharacter(const QString &txt, int esc)
{
    return ::cookedEscapeCharacter(txt, esc);
}

QByteArray xDebug::cookedAffixes(int affixes)
{
    return ::cookedAffixes(affixes);
}

void xDebug::setQuickTextDocumentMaximumBlockCount(QVariant textDocument, int count)
{
    if (textDocument.canConvert<QQuickTextDocument *>()) {
        QQuickTextDocument *doc = textDocument.value<QQuickTextDocument *>();
        if (doc) {
            QTextDocument *textDoc = doc->textDocument();
            if (textDoc) {
                textDoc->setMaximumBlockCount(count);
            }
        }
    }
}

QString xDebug::dateTimeString(const QString &format)
{
    return ::dateTimeString(format);
}

void xDebug::updateWindowStyle(QWindow *window, const QString &color)
{
#if defined(_MSC_VER)
    if (!window) {
        return;
    }

    qInfo() << color;
    QColor c(color);
    COLORREF colorref = c.red() | (c.green() << 8) | (c.blue() << 16);
    DwmSetWindowAttribute((HWND) window->winId(), DWMWA_CAPTION_COLOR, &colorref, sizeof(colorref));
#endif
}
