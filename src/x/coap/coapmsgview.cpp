/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coapmsgview.h"
#include "ui_coapmsgview.h"

#include <QAction>
#include <QActionGroup>
#include <QHeaderView>
#include <QJsonArray>
#include <QMenu>
#include <QMessageBox>

#include <coap3/coap.h>

#include "utilities/iconengine.h"
#include "utilities/keepopenedmenu.h"

#include "coapmsgfilter.h"
#include "coapmsgmodel.h"

#define CO_AP_NO_LIMIT -1

namespace Ui {
class CoAPMsgView;
};

namespace xCoAP {

class CoAPMsgViewPrivate : public QObject
{
public:
    CoAPMsgViewPrivate(CoAPMsgView* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::CoAPMsgView();
        ui->setupUi(q);
        ui->toolButtonColumns->setToolButtonStyle(Qt::ToolButtonIconOnly);
        ui->toolButtonColumns->setIcon(xIcon(":res/icons/list.svg"));
        ui->toolButtonScrolling->setIcon(xIcon(":res/icons/wrap_text.svg"));
        ui->toolButtonScrolling->setCheckable(true);
        ui->toolButtonScrolling->setChecked(true);
        ui->toolButtonClear->setIcon(xIcon(":res/icons/mop.svg"));
        ui->toolButtonLimit->setIcon(xIcon(":res/icons/contract.svg"));
        setupLimitMenu(ui->toolButtonLimit);
        connect(ui->toolButtonClear, &QToolButton::clicked, q, [=]() { onClearBtnClicked(); });
        connect(ui->toolButtonScrolling, &QToolButton::clicked, q, [=](bool checked) {
            onScrollToBottomToggled(checked);
        });
        connect(ui->lineEditFilter, &QLineEdit::textChanged, q, [=](const QString& text) {
            m_filter->setFilterText(text);
        });

        m_model = new CoAPMsgModel(q);
        m_filter = new CoAPMsgFilter(q);
        m_filter->setSourceModel(m_model);
        ui->tableView->setModel(m_filter);
        QHeaderView* hView = ui->tableView->horizontalHeader();
        hView->setSectionResizeMode(QHeaderView::ResizeToContents);
        hView->setStretchLastSection(true);
        hView->setMinimumSectionSize(80);

        int columns = m_model->columnCount(QModelIndex());
        m_columnsMenu = new xTools::KeepOpenedMenu(q);
        for (int i = 0; i < columns; ++i) {
            QString colName = m_model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
            QAction* action = new QAction(colName);
            action->setCheckable(true);
            action->setChecked(true);
            action->setData(i);
            m_columnsMenu->addAction(action);
            QObject::connect(action, &QAction::toggled, q, [=](bool checked) {
                ui->tableView->setColumnHidden(i, !checked);
            });
        }
        ui->toolButtonColumns->setMenu(m_columnsMenu);
        ui->toolButtonColumns->setPopupMode(QToolButton::MenuButtonPopup);
    }
    ~CoAPMsgViewPrivate() override {}

public:
    Ui::CoAPMsgView* ui{nullptr};
    CoAPMsgModel* m_model{nullptr};
    CoAPMsgFilter* m_filter{nullptr};
    QMenu* m_columnsMenu{nullptr};
    QActionGroup* m_limitActionGroup{nullptr};

public:
    void onScrollToBottomToggled(bool checked)
    {
        if (checked) {
            ui->tableView->scrollToBottom();
        }
    }
    void onClearBtnClicked()
    {
        int ret = QMessageBox::warning(q,
                                       tr("Warning"),
                                       tr("Are you sure to clear all messages?"),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            m_model->clear();
        }
    }

public:
    int getLimit() const
    {
        for (QAction* action : m_limitActionGroup->actions()) {
            if (action->isChecked()) {
                return action->data().toInt();
            }
        }
        return -1;
    }

private:
    CoAPMsgView* q{nullptr};

private:
    void setupLimitMenu(QToolButton* button)
    {
        if (m_limitActionGroup) {
            return;
        }

        QMenu* menu = new QMenu(button);
        m_limitActionGroup = new QActionGroup(button);
        m_limitActionGroup->setExclusive(true);

        QList<int> limits = {CO_AP_NO_LIMIT, 1000, 10000, 100000, 1000000};
        for (int limit : limits) {
            QString text = (limit == CO_AP_NO_LIMIT) ? tr("No Limit") : QString::number(limit);
            QAction* action = new QAction(text, button);
            action->setCheckable(true);
            action->setData(limit);
            menu->addAction(action);
            m_limitActionGroup->addAction(action);
            if (limit == 10000) {
                action->setChecked(true);
            }
        }
        button->setMenu(menu);
        button->setPopupMode(QToolButton::MenuButtonPopup);
    }
};

struct CoAPMsgViewParameterKeys
{
    const QString columns{"columns"};
    const QString limit{"limit"};
    const QString scrollToBottom{"scrollToBottom"};
};

CoAPMsgView::CoAPMsgView(QWidget* parent)
    : QWidget(parent)
{
    d = new CoAPMsgViewPrivate(this);
}

CoAPMsgView::~CoAPMsgView() {}

QJsonObject CoAPMsgView::save()
{
    CoAPMsgViewParameterKeys keys;
    QJsonObject obj;
    // Save columns visibility
    QJsonArray columnsArray;
    int columnCount = d->m_model->columnCount(QModelIndex());
    for (int i = 0; i < columnCount; ++i) {
        bool isVisible = !d->ui->tableView->isColumnHidden(i);
        columnsArray.append(isVisible);
    }
    obj[keys.columns] = columnsArray;

    // Save limit
    for (QAction* action : d->m_limitActionGroup->actions()) {
        if (action->isChecked()) {
            obj[keys.limit] = action->data().toInt();
            break;
        }
    }

    obj[keys.scrollToBottom] = d->ui->toolButtonScrolling->isChecked();
    return obj;
}

void CoAPMsgView::load(const QJsonObject& obj)
{
    CoAPMsgViewParameterKeys keys;
    // Load columns visibility
    QJsonArray columnsArray = obj.value(keys.columns).toArray();
    int columnCount = d->m_model->columnCount(QModelIndex());
    QList<QAction*> actions = d->m_columnsMenu->actions();
    for (int i = 0; i < columnCount && i < columnsArray.size() && i < actions.size(); ++i) {
        bool isVisible = columnsArray.at(i).toBool(true);
        d->ui->tableView->setColumnHidden(i, !isVisible);
        actions.at(i)->setChecked(isVisible);
    }

    // Load limit(The default is no limit)
    int limit = obj.value(keys.limit).toInt(CO_AP_NO_LIMIT);
    for (QAction* action : d->m_limitActionGroup->actions()) {
        if (action->data().toInt() == limit) {
            action->setChecked(true);
            break;
        }
    }

    bool scrollToBottom = obj.value(keys.scrollToBottom).toBool(true);
    d->ui->toolButtonScrolling->setChecked(scrollToBottom);
}

void CoAPMsgView::addMessage(std::shared_ptr<CoAPMsgItem> request,
                             std::shared_ptr<CoAPMsgItem> response)
{
    d->m_model->addRow(request, response, d->getLimit());
    if (d->ui->toolButtonScrolling->isChecked()) {
        d->ui->tableView->scrollToBottom();
    }
}

} // namespace xCoAP
