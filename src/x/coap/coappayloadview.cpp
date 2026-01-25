/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "coappayloadview.h"
#include "ui_coappayloadview.h"

#include <QAction>
#include <QActionGroup>
#include <QHeaderView>
#include <QJsonArray>
#include <QMenu>

#include "coapmsgmodel.h"
#include "utilities/iconengine.h"

namespace Ui {
class CoAPPayloadView;
};

namespace xCoAP {

class CoAPPayloadViewPrivate : public QObject
{
public:
    CoAPPayloadViewPrivate(CoAPPayloadView* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::CoAPPayloadView();
        ui->setupUi(q);
        ui->toolButtonColumns->setToolButtonStyle(Qt::ToolButtonIconOnly);
        ui->toolButtonColumns->setIcon(xIcon(":res/icons/list.svg"));

        m_model = new CoAPMsgModel(q);
        ui->tableView->setModel(m_model);
        ui->tableView->horizontalHeader()->setSizeAdjustPolicy(QHeaderView::AdjustToContents);
        ui->tableView->horizontalHeader()->setStretchLastSection(true);

        int columns = m_model->columnCount(QModelIndex());
        m_columnsMenu = new QMenu(q);
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
    ~CoAPPayloadViewPrivate() override {}

public:
    Ui::CoAPPayloadView* ui{nullptr};
    CoAPMsgModel* m_model{nullptr};
    QMenu* m_columnsMenu{nullptr};

private:
    CoAPPayloadView* q{nullptr};
};

struct CoAPPayloadViewParameterKeys
{
    const QString columns{"columns"};
};

CoAPPayloadView::CoAPPayloadView(QWidget* parent)
    : QWidget(parent)
{
    d = new CoAPPayloadViewPrivate(this);
}

CoAPPayloadView::~CoAPPayloadView() {}

QJsonObject CoAPPayloadView::save()
{
    CoAPPayloadViewParameterKeys keys;
    QJsonObject obj;
    QJsonArray columnsArray;
    int columnCount = d->m_model->columnCount(QModelIndex());
    for (int i = 0; i < columnCount; ++i) {
        bool isVisible = !d->ui->tableView->isColumnHidden(i);
        columnsArray.append(isVisible);
    }
    obj[keys.columns] = columnsArray;
    return obj;
}

void CoAPPayloadView::load(const QJsonObject& obj)
{
    CoAPPayloadViewParameterKeys keys;
    QJsonArray columnsArray = obj.value(keys.columns).toArray();
    int columnCount = d->m_model->columnCount(QModelIndex());
    QList<QAction*> actions = d->m_columnsMenu->actions();
    for (int i = 0; i < columnCount && i < columnsArray.size() && i < actions.size(); ++i) {
        bool isVisible = columnsArray.at(i).toBool(true);
        d->ui->tableView->setColumnHidden(i, !isVisible);
        actions.at(i)->setChecked(isVisible);
    }
}

} // namespace xCoAP
