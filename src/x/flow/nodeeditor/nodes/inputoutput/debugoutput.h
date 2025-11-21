/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "nodeeditor/nodes/common/basenode.h"

class DebugOutput : public BaseNode
{
    Q_OBJECT
public:
    struct DataKeys
    {
        const QString channel{"channel"};
        const QString format{"format"};
        const QString flag{"flag"};
        const QString showTime{"showTime"};
    };

public:
    DebugOutput(QObject *parent = nullptr);
    ~DebugOutput() override;

    QString caption() const override;
    QString name() const override;
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QWidget *embeddedWidget() override;

private:
    QWidget *m_embeddedWidget{nullptr};
};
