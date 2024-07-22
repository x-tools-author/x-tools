/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QVariant>
#include <QWidget>

class xToolsBaseTool;
class xToolsBaseToolUi : public QWidget
{
    Q_OBJECT
public:
    explicit xToolsBaseToolUi(QWidget *parent = Q_NULLPTR);

    virtual void setupTool(xToolsBaseTool *tool);
    virtual void setUiEnabled(bool enable);

    virtual QVariantMap save() const;
    virtual void load(const QVariantMap &data);
};
