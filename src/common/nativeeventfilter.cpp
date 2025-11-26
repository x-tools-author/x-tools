/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "nativeeventfilter.h"

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <QWidget>
#endif

#include "utilities/thememanager.h"

namespace xTools {

bool NativeEventFilter::nativeEventFilter(const QByteArray &eventType,
                                          void *message,
                                          qintptr *result)
{
#ifdef Q_OS_WIN
    MSG *msg = (MSG *) message;
    if (msg->message == WM_GETOBJECT) {
        // QTBUG-77974: https://blog.csdn.net/omg_orange/article/details/116779492
        return true;
    }

    if (eventType == "windows_generic_MSG") {
        MSG *msg = static_cast<MSG *>(message);
        if (msg->message == WM_ACTIVATE) {
            HWND hwnd = msg->hwnd;
            QWidget *w = QWidget::find((WId) hwnd);
            if (w) {
                xThemeMgr.updateWindowCaptionColor(w);
            }
        }
    }
#endif

    return false;
}

} // namespace xTools