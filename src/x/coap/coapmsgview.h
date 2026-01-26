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
#include <QTableView>

#include "coapcommon.h"

#include "utilities/serializable.h"

namespace xCoAP {

struct CoAPMsgItem;
class CoAPMsgViewPrivate;
class CoAPMsgView : public QWidget, public xTools::Serializable
{
    Q_OBJECT
    CoAPMsgViewPrivate *d{nullptr};

public:
    explicit CoAPMsgView(QWidget *parent = nullptr);
    ~CoAPMsgView() override;

    QJsonObject save() override;
    void load(const QJsonObject &obj) override;
    void addMessage(std::shared_ptr<CoAPMsgItem> request, std::shared_ptr<CoAPMsgItem> response);
};

} // namespace xCoAP