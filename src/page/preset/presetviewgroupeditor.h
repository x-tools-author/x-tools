/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
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

struct PresetViewGroupEditorGroupItem;
class PresetViewGroupEditor : public QWidget
{
    Q_OBJECT
public:
    explicit PresetViewGroupEditor(QWidget *parent = nullptr);
    ~PresetViewGroupEditor();

    QMenu *groupMenu() const;
    QJsonObject save();
    void load(const QJsonObject &obj);

signals:
    void invokeSend(int row);

private:
    void onAddButtonClicked();
    void onRemoveButtonClicked();
    void onUpButtonClicked();
    void onDownButtonClicked();
    void onItemDoubleClicked(QListWidgetItem *item);

    void addGroupsAction();
    void addItem(const PresetViewGroupEditorGroupItem *item);
    void sendGroups();
    void sendGroupItem(const QListWidgetItem *item);
    void delayMs(int ms);

private:
    Ui::PresetViewGroupEditor *ui;
    QMenu *m_menu{nullptr};
    QAction *m_groupsAction{nullptr};
    QTimer *m_timer{nullptr};
    QEventLoop *m_eventLoop{nullptr};
};
