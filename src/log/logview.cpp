/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "logview.h"
#include "logview_p.h"
#include "ui_logview.h"

#include <QMessageBox>

#include "utilities/compatibility.h"
#include "utilities/iconengine.h"

#include "log.h"
#include "logfilter.h"
#include "logmodel.h"

namespace xLog {

struct LogViewParameterKeys
{
    const QString logLevel{"logLevel"};
    const QString logCategory{"logCategory"};
    const QString filterText{"filterText"};
    const QString isDisableLog{"isDisableLog"};
    const QString scrollToBottom{"scrollToBottom"};
};

LogView::LogView(QWidget* parent)
    : QWidget(parent)
{
    d = new LogViewPrivate(this);
}

LogView::~LogView() {}

void LogView::setModel(QAbstractItemModel* model)
{
    d->ui->tableView->setModel(model);
}

void LogView::addLogCategory(const QString& category)
{
    if (category.isEmpty()) {
        return;
    }

    for (int i = 0; i < d->ui->comboBoxCategory->count(); ++i) {
        const QString txt = d->ui->comboBoxCategory->itemData(i).toString();
        if (txt == category) {
            return;
        }
    }

    d->ui->comboBoxCategory->addItem(category, category);
}

void LogView::scrollToBottom()
{
    d->ui->tableView->scrollToBottom();
}

bool LogView::isDisableLog()
{
    return d->ui->toolButtonScrolling->isChecked();
}

QJsonObject LogView::save()
{
    LogViewParameterKeys keys;
    QJsonObject obj;
    obj[keys.logLevel] = d->ui->comboBoxLevel->currentData().toInt();
    obj[keys.logCategory] = d->ui->comboBoxCategory->currentData().toString();
    obj[keys.filterText] = d->ui->lineEditFilter->text();
    obj[keys.isDisableLog] = d->ui->checkBoxDisable->isChecked();
    obj[keys.scrollToBottom] = d->ui->toolButtonScrolling->isChecked();
    return obj;
}

void LogView::load(const QJsonObject& obj)
{
    LogViewParameterKeys keys;
    int level = obj[keys.logLevel].toInt(QtMsgType::QtDebugMsg);
    int index = d->ui->comboBoxLevel->findData(level);
    if (index >= 0) {
        d->ui->comboBoxLevel->setCurrentIndex(index);
    }
    QString category = obj[keys.logCategory].toString();
    index = d->ui->comboBoxCategory->findData(category);
    if (index >= 0) {
        d->ui->comboBoxCategory->setCurrentIndex(index);
    }
    d->ui->lineEditFilter->setText(obj[keys.filterText].toString());
    d->ui->checkBoxDisable->setChecked(obj[keys.isDisableLog].toBool(false));
    d->ui->toolButtonScrolling->setChecked(obj[keys.scrollToBottom].toBool(false));

    if (d->ui->checkBoxDisable->isChecked()) {
        QAbstractItemModel* model = d->ui->tableView->model();
        LogFilter* filter = qobject_cast<LogFilter*>(model);
        if (filter) {
            model = filter->sourceModel();
            LogModel* logModel = qobject_cast<LogModel*>(model);
            if (logModel) {
                logModel->clearLogMessages();
            }
        }
    }
}

} // namespace xLog