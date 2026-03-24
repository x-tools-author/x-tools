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

class LogViewPrivate : public QObject
{
    Q_OBJECT
public:
    LogViewPrivate(LogView* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::LogView();
        ui->setupUi(q);
        ui->toolButtonScrolling->setIcon(xIcon(":/res/icons/wrap_text.svg"));
        ui->toolButtonScrolling->setCheckable(true);
        ui->toolButtonClear->setIcon(xIcon(":/res/icons/mop.svg"));
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        ui->tableView->setAlternatingRowColors(true);

        // Log level
        ui->comboBoxLevel->addItem(tr("Debug"), QtMsgType::QtDebugMsg);
        ui->comboBoxLevel->addItem(tr("Info"), QtMsgType::QtInfoMsg);
        ui->comboBoxLevel->addItem(tr("Warning"), QtMsgType::QtWarningMsg);
        ui->comboBoxLevel->addItem(tr("Critical"), QtMsgType::QtCriticalMsg);

        // Log modules
        // clang-format off
        ui->comboBoxCategory->addItem(tr("All categories"), QString("default"));
        connect(ui->comboBoxLevel, xComboBoxActivated, this, &LogViewPrivate::onLogLevelChanged);
        connect(ui->comboBoxCategory, xComboBoxActivated, this, &LogViewPrivate::onLogCategoryChanged);
        connect(ui->lineEditFilter, &QLineEdit::textChanged, this, &LogViewPrivate::onFilterTextChanged);
        connect(ui->toolButtonClear, &QToolButton::clicked, this, &LogViewPrivate::onClearLogs);
        // clang-format on
    }
    ~LogViewPrivate() override { delete ui; }

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

private:
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
};

} // namespace xLog