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

#include <QJsonArray>
#include <QJsonValue>
#include <QMessageBox>

#include "common/xtools.h"
#include "utilities/compatibility.h"

struct PresetViewGroupEditorGroupItem
{
    QString name;
    QList<int> columns;
    int itemInterval;
    bool disabled;
};

struct PresetViewGroupEditorGroupItemKeys
{
    const QString Name{"name"};
    const QString Columns{"columns"};
    const QString ItemInterval{"itemInterval"};
    const QString Disabled{"disabled"};
};

PresetViewGroupEditorGroupItem decodePresetViewGroupEditorGroupItem(const QJsonObject &obj)
{
    PresetViewGroupEditorGroupItemKeys keys;
    PresetViewGroupEditorGroupItem item;
    item.name = obj.value(keys.Name).toString();
    QJsonArray arr = obj.value(keys.Columns).toArray();
    for (const QJsonValue &v : arr) {
        item.columns.append(v.toInt());
    }
    item.itemInterval = obj.value(keys.ItemInterval).toInt(100);
    item.disabled = obj.value(keys.Disabled).toBool(false);
    return item;
}

QJsonObject encodePresetViewGroupEditorGroupItem(const PresetViewGroupEditorGroupItem &item)
{
    PresetViewGroupEditorGroupItemKeys keys;
    QJsonObject obj;
    obj.insert(keys.Name, item.name);
    QJsonArray arr;
    for (const int &s : item.columns) {
        arr.append(s);
    }
    obj.insert(keys.Columns, arr);
    obj.insert(keys.ItemInterval, item.itemInterval);
    obj.insert(keys.Disabled, item.disabled);
    return obj;
}

PresetViewGroupEditor::PresetViewGroupEditor(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PresetViewGroupEditor)
{
    ui->setupUi(this);
    m_menu = new QMenu(tr("Group Sending"));
    addGroupsAction();

    m_timer = new QTimer(this);
    m_eventLoop = new QEventLoop(this);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, m_eventLoop, &QEventLoop::quit);

    // clang-format off
    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &PresetViewGroupEditor::onAddButtonClicked);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &PresetViewGroupEditor::onRemoveButtonClicked);
    connect(ui->pushButtonUp, &QPushButton::clicked, this, &PresetViewGroupEditor::onUpButtonClicked);
    connect(ui->pushButtonDown, &QPushButton::clicked, this, &PresetViewGroupEditor::onDownButtonClicked);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &PresetViewGroupEditor::onItemDoubleClicked);
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
    QJsonObject obj;
    obj.insert("groupInterval", ui->spinBoxGroupInterval->value());

    QJsonArray groupArray;
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        auto item = ui->listWidget->item(i);
        if (item) {
            QJsonObject groupObj = item->data(Qt::UserRole).toJsonObject();
            groupArray.append(groupObj);
        }
    }

    obj.insert("groups", groupArray);
    return obj;
}

void PresetViewGroupEditor::load(const QJsonObject &obj)
{
    int groupInterval = obj.value("groupInterval").toInt(100);
    ui->spinBoxGroupInterval->setValue(groupInterval);

    ui->listWidget->clear();
    m_menu->clear();
    addGroupsAction();

    QJsonArray groupArray = obj.value("groups").toArray();
    for (const QJsonValue &v : groupArray) {
        QJsonObject groupObj = v.toObject();
        PresetViewGroupEditorGroupItem item = decodePresetViewGroupEditorGroupItem(groupObj);
        addItem(&item);
    }
}

void PresetViewGroupEditor::onRemoveButtonClicked()
{
    auto item = ui->listWidget->currentItem();
    if (item) {
        for (QAction *action : m_menu->actions()) {
            if (action->data() == item->data(Qt::UserRole)) {
                m_menu->removeAction(action);
                break;
            }
        }

        ui->listWidget->removeItemWidget(item);
        delete item;
    }
}

void PresetViewGroupEditor::onAddButtonClicked()
{
    QStringList tmp = ui->lineEditColumns->text().split(' ', xSkipEmptyParts);
    QList<int> columns;
    for (const QString &s : tmp) {
        bool ok = false;
        int v = s.toInt(&ok);
        if (ok) {
            columns.append(v);
        }
    }

    PresetViewGroupEditorGroupItem item;
    item.name = ui->lineEditName->text();
    item.columns = columns;
    item.itemInterval = ui->spinBoxItemInterval->value();
    item.disabled = ui->checkBoxDisableGroup->isChecked();

    if (item.name.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("The group name cannot be empty."));
        ui->lineEditColumns->setFocus();
        return;
    }

    if (item.columns.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("The columns cannot be empty."));
        ui->lineEditColumns->setFocus();
        return;
    }

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        auto listItem = ui->listWidget->item(i);
        if (listItem->text() == item.name) {
            auto ret = QMessageBox::warning(this,
                                            tr("Warning"),
                                            tr("The group name \"%1\" already exists. Replace it?")
                                                .arg(item.name),
                                            QMessageBox::Yes | QMessageBox::No,
                                            QMessageBox::No);
            if (ret == QMessageBox::Yes) {
                listItem->setData(Qt::UserRole, encodePresetViewGroupEditorGroupItem(item));
            }

            return;
        }
    }

    addItem(&item);
}

void PresetViewGroupEditor::onUpButtonClicked()
{
    auto item = ui->listWidget->currentItem();
    if (item) {
        int row = ui->listWidget->row(item);
        if (row > 0) {
            ui->listWidget->takeItem(row);
            ui->listWidget->insertItem(row - 1, item);
            ui->listWidget->setCurrentItem(item);
        }
    }
}

void PresetViewGroupEditor::onDownButtonClicked()
{
    auto item = ui->listWidget->currentItem();
    if (item) {
        int row = ui->listWidget->row(item);
        if (row < ui->listWidget->count() - 1) {
            ui->listWidget->takeItem(row);
            ui->listWidget->insertItem(row + 1, item);
            ui->listWidget->setCurrentItem(item);
        }
    }
}

void PresetViewGroupEditor::onItemDoubleClicked(QListWidgetItem *item)
{
    if (item) {
        QJsonObject obj = item->data(Qt::UserRole).toJsonObject();
        PresetViewGroupEditorGroupItem groupItem = decodePresetViewGroupEditorGroupItem(obj);
        ui->lineEditName->setText(groupItem.name);
        QStringList tmp;
        for (const int &s : groupItem.columns) {
            tmp.append(QString::number(s));
        }
        ui->lineEditColumns->setText(tmp.join(' '));
        ui->spinBoxItemInterval->setValue(groupItem.itemInterval);
        ui->checkBoxDisableGroup->setChecked(groupItem.disabled);
    }
}

void PresetViewGroupEditor::addGroupsAction()
{
    m_menu->addAction(tr("Send Groups"), this, &PresetViewGroupEditor::sendGroups);
    m_menu->addSeparator();
}

void PresetViewGroupEditor::addItem(const PresetViewGroupEditorGroupItem *item)
{
    QJsonObject obj = encodePresetViewGroupEditorGroupItem(*item);
    QListWidgetItem *listItem = new QListWidgetItem(ui->listWidget);
    listItem->setText(item->name);
    listItem->setData(Qt::UserRole, obj);
    ui->listWidget->addItem(listItem);

    QAction *action = m_menu->addAction(item->name);
    action->setData(QVariant::fromValue(obj));
    connect(action, &QAction::triggered, this, [this, listItem]() {
        this->ui->listWidget->setCurrentItem(listItem);
        this->sendGroupItem(listItem);
    });
}

void PresetViewGroupEditor::sendGroups()
{
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        auto item = ui->listWidget->item(i);
        if (item) {
            sendGroupItem(item);
            delayMs(ui->spinBoxGroupInterval->value());
        }
    }
}

void PresetViewGroupEditor::sendGroupItem(const QListWidgetItem *item)
{
    QJsonObject obj = item->data(Qt::UserRole).toJsonObject();
    PresetViewGroupEditorGroupItem groupItem = decodePresetViewGroupEditorGroupItem(obj);
    if (groupItem.disabled) {
        return;
    }

    setEnabled(false);
    QList<int> columns = groupItem.columns;
    if (columns.isEmpty()) {
        setEnabled(true);
        return;
    }

    for (int i = 0; i < columns.size(); ++i) {
        emit invokeSend(columns.at(i) - 1);
        delayMs(groupItem.itemInterval);
    }

    setEnabled(true);
}

void PresetViewGroupEditor::delayMs(int ms)
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }

    m_timer->start(ms);
    m_eventLoop->exec();
}