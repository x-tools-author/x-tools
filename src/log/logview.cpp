/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "logview.h"
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

class LogViewPrivate : public QObject
{
public:
    LogViewPrivate(LogView* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {}
    ~LogViewPrivate() override { delete ui; }

    void onLogLevelChanged()
    {
        LogFilter* f = filter();
        if (f) {
            int level = ui->comboBoxLevel->currentData().toInt();
            f->setFilterLogLevel(level);
            qCDebug(xLogLoggingCategory) << "Log level changed to" << level;
        }
    }
    void onLogCategoryChanged()
    {
        LogFilter* f = filter();
        if (f) {
            QString category = ui->comboBoxCategory->currentData().toString();
            f->setFilterCategory(category);
            qCDebug(xLogLoggingCategory) << "Log category changed to" << category;
        }
    }
    void onFilterTextChanged()
    {
        LogFilter* f = filter();
        if (f) {
            QString text = ui->lineEditFilter->text();
            f->setFilterText(text);
            qCDebug(xLogLoggingCategory) << "Log filter text changed to" << text;
        }
    }
    void onClearLogs()
    {
        int ret = QMessageBox::question(q,
                                        tr("Clear log messages"),
                                        tr("Are you sure to clear all log messages?"),
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::No);
        if (ret != QMessageBox::Yes) {
            return;
        }

        LogFilter* f = filter();
        if (f) {
            QAbstractItemModel* sourceModel = f->sourceModel();
            LogModel* logModel = qobject_cast<LogModel*>(sourceModel);
            if (logModel) {
                logModel->clearLogMessages();
            }
        }
    }

public:
    Ui::LogView* ui{nullptr};

private:
    LogView* q{nullptr};

private:
    LogFilter* filter()
    {
        QAbstractItemModel* model = ui->tableView->model();
        return qobject_cast<LogFilter*>(model);
    }
};

LogView::LogView(QWidget* parent)
    : QWidget(parent)
{
    d = new LogViewPrivate(this);
    d->ui = new Ui::LogView();
    d->ui->setupUi(this);
    d->ui->toolButtonScrolling->setIcon(xIcon(":/res/icons/wrap_text.svg"));
    d->ui->toolButtonScrolling->setCheckable(true);
    d->ui->toolButtonClear->setIcon(xIcon(":/res/icons/mop.svg"));
    d->ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    d->ui->tableView->horizontalHeader()->setStretchLastSection(true);
    d->ui->tableView->setAlternatingRowColors(true);

    // Log level
    d->ui->comboBoxLevel->addItem(tr("Debug"), QtMsgType::QtDebugMsg);
    d->ui->comboBoxLevel->addItem(tr("Info"), QtMsgType::QtInfoMsg);
    d->ui->comboBoxLevel->addItem(tr("Warning"), QtMsgType::QtWarningMsg);
    d->ui->comboBoxLevel->addItem(tr("Critical"), QtMsgType::QtCriticalMsg);

    // Log modules
    d->ui->comboBoxCategory->addItem(tr("All categories"), QString("default"));
    // clang-format off
    connect(d->ui->comboBoxLevel, xComboBoxActivated, this, [=]() { d->onLogLevelChanged(); });
    connect(d->ui->comboBoxCategory, xComboBoxActivated, this, [=]() { d->onLogCategoryChanged(); });
    connect(d->ui->lineEditFilter, &QLineEdit::textChanged, this, [=]() { d->onFilterTextChanged(); });
    connect(d->ui->toolButtonClear, &QToolButton::clicked, this, [=]() { d->onClearLogs(); });
    // clang-format on
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