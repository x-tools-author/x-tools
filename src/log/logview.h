/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractItemModel>
#include <QWidget>

#include "utilities/serializable.h"

namespace xLog {

class LogViewPrivate;
class LogView : public QWidget, public xTools::Serializable
{
    Q_OBJECT
    LogViewPrivate *d{nullptr};

public:
    explicit LogView(QWidget *parent = nullptr);
    ~LogView() override;

    void setModel(QAbstractItemModel *model);
    void addLogCategory(const QString &category);
    void scrollToBottom();
    bool isDisableLog();

    QJsonObject save() override;
    void load(const QJsonObject &obj) override;
};

} // namespace xLog