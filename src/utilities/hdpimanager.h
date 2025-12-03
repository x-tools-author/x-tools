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

#define xHdpiMgr xTools::HdpiManager::singleton()

namespace xTools {

class HdpiManagerPrivate;
class HdpiManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HdpiManager)

    HdpiManager(QObject* parent = nullptr);
    HdpiManager(const HdpiManager&) = delete;
    HdpiManager& operator=(const HdpiManager&) = delete;
    HdpiManager(HdpiManager&&) = delete;
    HdpiManager& operator=(HdpiManager&&) = delete;

public:
    ~HdpiManager() override;

    static HdpiManager& singleton();
    void setupSettings(QSettings* settings); // You must call this function first before using
    void setupHdpi();
    QMenu* hdpiMenu();

signals:
    void hdpiChanged();
};

} // namespace xTools
