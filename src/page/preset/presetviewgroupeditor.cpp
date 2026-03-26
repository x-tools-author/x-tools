/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
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

class PresetViewGroupEditorPrivate : public QObject
{
public:
    PresetViewGroupEditorPrivate(PresetViewGroupEditor *q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~PresetViewGroupEditorPrivate() { delete ui; }

public:
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

public:
    Ui::PresetViewGroupEditor *ui;
    QMenu *m_menu{nullptr};
    QAction *m_groupsAction{nullptr};
    QTimer *m_timer{nullptr};
    QEventLoop *m_eventLoop{nullptr};

private:
    PresetViewGroupEditor *q;
};

void PresetViewGroupEditorPrivate::onRemoveButtonClicked()
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

void PresetViewGroupEditorPrivate::onAddButtonClicked()
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
        QMessageBox::warning(q, tr("Warning"), tr("The group name cannot be empty."));
        ui->lineEditColumns->setFocus();
        return;
    }

    if (item.columns.isEmpty()) {
        QMessageBox::warning(q, tr("Warning"), tr("The columns cannot be empty."));
        ui->lineEditColumns->setFocus();
        return;
    }

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        auto listItem = ui->listWidget->item(i);
        if (listItem->text() == item.name) {
            auto ret = QMessageBox::warning(q,
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

void PresetViewGroupEditorPrivate::onUpButtonClicked()
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

void PresetViewGroupEditorPrivate::onDownButtonClicked()
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

void PresetViewGroupEditorPrivate::onItemDoubleClicked(QListWidgetItem *item)
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

void PresetViewGroupEditorPrivate::addGroupsAction()
{
    m_menu->addAction(tr("Send Groups"), this, &PresetViewGroupEditorPrivate::sendGroups);
    m_menu->addSeparator();
}

void PresetViewGroupEditorPrivate::addItem(const PresetViewGroupEditorGroupItem *item)
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

void PresetViewGroupEditorPrivate::sendGroups()
{
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        if (item) {
            sendGroupItem(item);
            delayMs(ui->spinBoxGroupInterval->value());
        }
    }
}

void PresetViewGroupEditorPrivate::sendGroupItem(const QListWidgetItem *item)
{
    QJsonObject obj = item->data(Qt::UserRole).toJsonObject();
    PresetViewGroupEditorGroupItem groupItem = decodePresetViewGroupEditorGroupItem(obj);
    if (groupItem.disabled) {
        return;
    }

    q->setEnabled(false);
    QList<int> columns = groupItem.columns;
    if (columns.isEmpty()) {
        q->setEnabled(true);
        return;
    }

    for (int i = 0; i < columns.size(); ++i) {
        emit q->invokeSend(columns.at(i) - 1);
        delayMs(groupItem.itemInterval);
    }

    q->setEnabled(true);
}

void PresetViewGroupEditorPrivate::delayMs(int ms)
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }

    m_timer->start(ms);
    m_eventLoop->exec();
}

PresetViewGroupEditor::PresetViewGroupEditor(QWidget *parent)
    : QWidget(parent)
{
    d = new PresetViewGroupEditorPrivate(this);
    d->ui = new Ui::PresetViewGroupEditor;
    d->ui->setupUi(this);
    d->m_menu = new QMenu(tr("Group Sending"));
    d->addGroupsAction();

    d->m_timer = new QTimer(this);
    d->m_eventLoop = new QEventLoop(this);
    d->m_timer->setSingleShot(true);
    connect(d->m_timer, &QTimer::timeout, d->m_eventLoop, &QEventLoop::quit);

    // clang-format off
    connect(d->ui->pushButtonAdd, &QPushButton::clicked, d, &PresetViewGroupEditorPrivate::onAddButtonClicked);
    connect(d->ui->pushButtonDelete, &QPushButton::clicked, d, &PresetViewGroupEditorPrivate::onRemoveButtonClicked);
    connect(d->ui->pushButtonUp, &QPushButton::clicked, d, &PresetViewGroupEditorPrivate::onUpButtonClicked);
    connect(d->ui->pushButtonDown, &QPushButton::clicked, d, &PresetViewGroupEditorPrivate::onDownButtonClicked);
    connect(d->ui->listWidget, &QListWidget::itemDoubleClicked, d, &PresetViewGroupEditorPrivate::onItemDoubleClicked);
    // clang-format on
}

PresetViewGroupEditor::~PresetViewGroupEditor() {}

QMenu *PresetViewGroupEditor::groupMenu() const
{
    return d->m_menu;
}

QJsonObject PresetViewGroupEditor::save()
{
    QJsonObject obj;
    obj.insert("groupInterval", d->ui->spinBoxGroupInterval->value());

    QJsonArray groupArray;
    for (int i = 0; i < d->ui->listWidget->count(); ++i) {
        auto item = d->ui->listWidget->item(i);
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
    d->ui->spinBoxGroupInterval->setValue(groupInterval);

    d->ui->listWidget->clear();
    d->m_menu->clear();
    d->addGroupsAction();

    QJsonArray groupArray = obj.value("groups").toArray();
    for (const QJsonValue &v : groupArray) {
        QJsonObject groupObj = v.toObject();
        PresetViewGroupEditorGroupItem item = decodePresetViewGroupEditorGroupItem(groupObj);
        d->addItem(&item);
    }
}
