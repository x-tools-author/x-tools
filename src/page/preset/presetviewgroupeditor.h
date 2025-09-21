/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QMenu>
#include <QWidget>

namespace Ui {
class PresetViewGroupEditor;
}

struct PresetViewGroupEditorParameterKey
{};

class PresetViewGroupEditor : public QWidget
{
    Q_OBJECT
public:
    explicit PresetViewGroupEditor(QWidget *parent = nullptr);
    ~PresetViewGroupEditor();

    QMenu *groupMenu() const;
    QJsonObject save();
    void load(const QJsonObject &obj);

private:
    void onAddButtonClicked();
    void onRemoveButtonClicked();
    void onUpButtonClicked();
    void onDownButtonClicked();

    void sendGroups();

private:
    Ui::PresetViewGroupEditor *ui;
    QMenu *m_menu{nullptr};
};
