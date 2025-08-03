/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QVariantMap>
#include <QWidget>

class Panel : public QWidget
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = nullptr);
    virtual ~Panel();

    virtual QVariantMap save() const;
    virtual void load(const QVariantMap &parameters);
    virtual QWidget *menuWidget();
    virtual QMenu *buttonMenu() const;
    virtual void inputBytes(const QByteArray &bytes);

private:
    QVariantMap m_parameters;
};
