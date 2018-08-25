/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#ifndef SAKSETTINGS_H
#define SAKSETTINGS_H

#include <QSettings>
#include <QStandardPaths>
#include <QApplication>

#define SAKSETTINGFILE (QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/SAKApplication.ini")

#define APPLICATIONSTYLE "Application/style"

class SAKSettings:public QSettings
{
    Q_OBJECT
public:
    SAKSettings(const QString &fileName = SAKSETTINGFILE, Format format = QSettings::IniFormat, QObject *parent = Q_NULLPTR);
    ~SAKSettings();

    static SAKSettings *_sakSettings;

    static QString valueApplicationStyle();
    void setValueApplicationStyle(QString value);
};

SAKSettings *sakSettings();

#endif
