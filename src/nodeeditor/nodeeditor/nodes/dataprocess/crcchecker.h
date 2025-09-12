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

struct CrcCheckerParametersKeys
{
    const QString startIndex{"startIndex"};
    const QString endIndex{"endIndex"};
    const QString algorithm{"algorithm"};
    const QString bigEndian{"bigEndian"};
};

class CrcChecker : public BaseNode
{
    Q_OBJECT
public:
    CrcChecker(QObject *parent = nullptr);
    ~CrcChecker() override;

    QString caption() const override;
    QString name() const override;
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QWidget *embeddedWidget() override;

protected:
    void handleData(std::shared_ptr<BaseNodeData> nodeData, QtNodes::PortIndex const index) override;

private:
    QWidget *m_embeddedWidget{nullptr};
};
