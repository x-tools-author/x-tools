/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>

#include "nodeeditor/nodes/common/basenode.h"

class UserInput : public BaseNode
{
    Q_OBJECT
public:
    UserInput(QObject *parent = nullptr);
    ~UserInput() override;

public:
    QString caption() const override;
    QString name() const override;
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QWidget *embeddedWidget() override;

public:
    struct Data
    {
        int format;
        int prefix;
        int suffix;
        int escape;
        int interval;
        QString text;
    };

    struct DataKeys
    {
        const QString format{"format"};
        const QString prefix{"prefix"};
        const QString suffix{"suffix"};
        const QString escape{"escape"};
        const QString interval{"interval"};
        const QString text{"text"};
    };

    static QJsonObject saveData(const Data &data);
    static Data loadData(const QJsonObject &data);
    static QByteArray data2Bytes(const Data &data);

private:
    QWidget *m_embeddedWidget{nullptr};
};
