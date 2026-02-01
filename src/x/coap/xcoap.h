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

#include "layoutpage.h"
#include "utilities/serializable.h"

namespace xCoAP {

class xCoAPPrivate;
class xCoAP : public QWidget, public xTools::Serializable, public LayoutPage
{
    Q_OBJECT
    xCoAPPrivate* d;

public:
    explicit xCoAP(QWidget* parent = nullptr);
    ~xCoAP();

    void setupSettings(QSettings* settings);
    void exportSettings(const QString& filePath);
    void importSettings(const QString& filePath);

    QJsonObject save() override;
    void load(const QJsonObject& obj) override;
    QMenu* toolButtonMenu() override;
};

} // namespace xCoAP
