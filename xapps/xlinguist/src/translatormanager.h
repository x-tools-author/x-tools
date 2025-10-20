/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QObject>

#define xTranslatorMgr TranslatorManager::instance()

class TranslatorManager : public QObject
{
    Q_OBJECT
private:
    explicit TranslatorManager(QObject *parent = nullptr);
    TranslatorManager(const TranslatorManager &) = delete;
    TranslatorManager &operator=(const TranslatorManager &) = delete;

public:
    ~TranslatorManager();
    static TranslatorManager &instance();

public:
    struct TranslatingItem
    {
        QString from;
        QString to;
        QString text;

        QString srcFile;
        int line;
    };

private:
    QList<TranslatingItem> m_translatingItems;
};
