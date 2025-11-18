/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMenu>
#include <QObject>
#include <QSettings>

#define xI18n xTools::I18n::singleton()

namespace xTools {

class I18nPrivate;
class I18n : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(I18n)
    Q_DISABLE_COPY(I18n)

    I18n(QObject* parent = nullptr);

public:
    ~I18n();

    static I18n& singleton();
    QMenu* languageMenu();
    void setupSettings(QSettings* settings); // You must call this before calling
    void setupLanguage();

signals:
    void languageChanged();
};

} // namespace xTools