/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QSettings>
#include <QWidget>

#include "utilities/serializable.h"
#include "x/xpage.h"

namespace xCoAP {

class xCoAPPrivate;
class xCoAP : public xPage
{
    Q_OBJECT
    xCoAPPrivate* d{nullptr};

public:
    explicit xCoAP(QWidget* parent = nullptr);
    ~xCoAP();

    void setupSettings(QSettings* settings);
    void exportSettings(const QString& filePath);
    void importSettings(const QString& filePath);

    QJsonObject save() const override;
    void load(const QJsonObject& obj) override;
    QMenu* toolButtonMenu() override;
};

} // namespace xCoAP
