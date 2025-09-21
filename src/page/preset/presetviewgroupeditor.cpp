/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "presetviewgroupeditor.h"
#include "ui_presetviewgroupeditor.h"

PresetViewGroupEditor::PresetViewGroupEditor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PresetViewGroupEditor)
{
    ui->setupUi(this);
    m_menu = new QMenu(tr("Group Sending"));
    m_menu->addAction(tr("Send Groups"), this, &PresetViewGroupEditor::sendGroups);
    m_menu->addSeparator();
    // clang-format off
    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &PresetViewGroupEditor::onAddButtonClicked);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &PresetViewGroupEditor::onRemoveButtonClicked);
    connect(ui->pushButtonUp, &QPushButton::clicked, this, &PresetViewGroupEditor::onUpButtonClicked);
    connect(ui->pushButtonDown, &QPushButton::clicked, this, &PresetViewGroupEditor::onDownButtonClicked);
    // clang-format on
}

PresetViewGroupEditor::~PresetViewGroupEditor()
{
    delete ui;
}

QMenu *PresetViewGroupEditor::groupMenu() const
{
    return m_menu;
}

QJsonObject PresetViewGroupEditor::save()
{
    return QJsonObject();
}

void PresetViewGroupEditor::load(const QJsonObject &obj)
{
    Q_UNUSED(obj);
}

void PresetViewGroupEditor::onRemoveButtonClicked() {}

void PresetViewGroupEditor::onAddButtonClicked() {}

void PresetViewGroupEditor::onUpButtonClicked() {}

void PresetViewGroupEditor::onDownButtonClicked() {}

void PresetViewGroupEditor::sendGroups() {}