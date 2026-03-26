/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QEventLoop>
#include <QJsonObject>
#include <QListWidgetItem>
#include <QMenu>
#include <QTimer>
#include <QWidget>

namespace Ui {
class PresetViewGroupEditor;
}

class PresetViewGroupEditorPrivate;
struct PresetViewGroupEditorGroupItem;
class PresetViewGroupEditor : public QWidget
{
    Q_OBJECT
    PresetViewGroupEditorPrivate *d{nullptr};

public:
    explicit PresetViewGroupEditor(QWidget *parent = nullptr);
    ~PresetViewGroupEditor();

    QMenu *groupMenu() const;
    QJsonObject save();
    void load(const QJsonObject &obj);

signals:
    void invokeSend(int row);
};
