/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QWidget>

#include "utilities/serializable.h"

#define gCoAPGlobal CoAPGlobal::singleton()

namespace xCoAP {

class CoAPGlobalPrivate;
class CoAPGlobal : public QWidget, public xTools::Serializable
{
    Q_OBJECT
    CoAPGlobalPrivate* d{nullptr};

private:
    explicit CoAPGlobal(QWidget* parent = nullptr);
    CoAPGlobal(const CoAPGlobal&) = delete;
    CoAPGlobal& operator=(const CoAPGlobal&) = delete;

public:
    ~CoAPGlobal();

    static CoAPGlobal& singleton();
    void showThenMoveToCenter();

    QJsonObject save() override;
    void load(const QJsonObject& obj) override;

    bool isClientNameEnabled() const;
    QString clientName() const;
    bool isTokenEnabled() const;
    int tokenLength() const;
    bool isServerNameEnabled() const;
    QString serverName() const;
    QString serverCachePath() const;
};

} // namespace xCoAP
