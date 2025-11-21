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

struct AesCipherParametersKeys
{
    const QString workWay{"workWay"};
    const QString key{"key"};
    const QString iv{"iv"};
    const QString mode{"mode"};
    const QString padding{"padding"};
    const QString level{"level"};
};

class AesCipher : public BaseNode
{
    Q_OBJECT
public:
    enum class WorkWay { Encrypt, Decrypt };
    Q_ENUM(WorkWay);

    AesCipher(QObject *parent = nullptr);
    ~AesCipher() override;

    QString caption() const override;
    QString name() const override;
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QWidget *embeddedWidget() override;

private:
    void process(const QByteArray &bytes);
    Q_SIGNAL void input2run(const QByteArray &bytes);

private:
    QWidget *m_embeddedWidget{nullptr};
};
