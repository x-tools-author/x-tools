/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class xToolsResponserToolUiEditor;
}

class xToolsResponserToolUiEditor : public QDialog
{
    Q_OBJECT
public:
    explicit xToolsResponserToolUiEditor(QWidget *parent = nullptr);
    ~xToolsResponserToolUiEditor();

    Q_INVOKABLE QJsonObject parameters();
    Q_INVOKABLE void setParameters(const QJsonObject &params);

private:
    Ui::xToolsResponserToolUiEditor *ui{nullptr};
};
