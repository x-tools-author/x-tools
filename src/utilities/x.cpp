/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "x.h"

#include <QMetaObject>

namespace xTools {

QString systemDateFormat()
{
    return QLocale::system().dateFormat();
}

QString systemTimeFormat()
{
    return QLocale::system().timeFormat();
}

QString dateTimeString(const QString &format)
{
    return QDateTime::currentDateTime().toString(format);
}

QDateTime buildDateTime()
{
    QString dateString = QString(__DATE__);
    QString timeString = QString(__TIME__);
    dateString = dateString.replace(QString("  "), " 0");
    QString dateTimeString = dateString + " " + timeString;
    QDateTime dateTime = QLocale(QLocale::English).toDateTime(dateTimeString, "MMM dd yyyy hh:mm:ss");
    return dateTime;
}

QString buildDateTimeString(const QString &format)
{
    return buildDateTime().toString(format);
}

void setNoneBorder(QWidget *widget)
{
    if (!widget) {
        return;
    }

    const QMetaObject *metaObj = widget->metaObject();
    QString className = QString::fromLatin1(metaObj->className());
    QString objectName = widget->objectName();
    QString styleSheet = QString("%1#%2 {border: none;}").arg(className, objectName);
    widget->setStyleSheet(styleSheet);
}

} // namespace xTools