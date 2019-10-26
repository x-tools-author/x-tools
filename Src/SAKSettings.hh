/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
 */
#ifndef SAKSETTINGS_HH
#define SAKSETTINGS_HH

#include <QSettings>

class SAKSettings:public QSettings
{
    Q_OBJECT
public:
    static SAKSettings* instance();
private:
    static SAKSettings* _instance;
    SAKSettings(const QString &fileName, QSettings::Format format, QObject *parent = nullptr);
    ~SAKSettings();
};

#endif
