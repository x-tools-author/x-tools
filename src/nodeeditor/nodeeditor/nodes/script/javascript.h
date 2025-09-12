/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "nodeeditor/nodes/common/basenode.h"

struct JavaScriptParametersKeys
{
    const QString script{"script"};
};

class JavaScript : public BaseNode
{
    Q_OBJECT
public:
    JavaScript(QObject *parent = nullptr);
    ~JavaScript() override;

    QString caption() const override;
    QString name() const override;
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QWidget *embeddedWidget() override;
    bool resizable() const override;

private:
    Q_SIGNAL void invokeExecScript(const QByteArray &bytes);

private:
    QWidget *m_embeddedWidget{nullptr};
};
