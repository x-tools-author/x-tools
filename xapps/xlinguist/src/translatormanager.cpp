/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "translatormanager.h"

#include <QDebug>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

TranslatorManager::TranslatorManager(QObject *parent)
    : QObject(parent)
{}

TranslatorManager::~TranslatorManager() {}

TranslatorManager &TranslatorManager::instance()
{
    static TranslatorManager instance;
    return instance;
}