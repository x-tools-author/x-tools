/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QtNodes/Definitions>

#include "nodeeditor/nodes/common/basenodeui.h"

class DeviceUi;
class CommunicationUi : public BaseNodeUi
{
    Q_OBJECT
public:
    CommunicationUi(int type, BaseNode *node, QWidget *parent = nullptr);
    ~CommunicationUi();

    DeviceUi *deviceUi() const;
    void rebootDevice();

    QJsonObject save() const override;
    void load(const QJsonObject &parameters) override;

private:
    DeviceUi *m_deviceUi;
};
