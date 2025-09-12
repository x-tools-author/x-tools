/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "nodeeditor/nodes/common/basenode.h"

class DataView : public BaseNode
{
    Q_OBJECT
public:
    DataView(QObject *parent = nullptr);
    ~DataView();

    QString caption() const override;
    QString name() const override;
    unsigned int nPorts(QtNodes::PortType portType) const override;
    bool resizable() const override;
    QWidget *embeddedWidget() override;

private:
    QWidget *m_embeddedWidget;
};
