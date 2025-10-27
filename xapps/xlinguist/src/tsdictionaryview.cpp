/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tsdictionaryview.h"
#include "ui_tsdictionaryview.h"

#include <QFileDialog>
#include <QHeaderView>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QTextStream>

#include "tsdictionary.h"
#include "tsdictionaryfilter.h"

TsDictionaryView::TsDictionaryView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TsDictionaryView)
{
    ui->setupUi(this);
    m_dictionaryModel = new TsDictionary(this);
    m_dictionaryFilterModel = new TsDictionaryFilter(this);
    m_dictionaryFilterModel->setSourceModel(m_dictionaryModel);
    ui->tableView->setModel(m_dictionaryFilterModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setAlternatingRowColors(true);

    QStringList flags = m_dictionaryModel->languageFlags();
    QMenu *menu = new QMenu(this);
    ui->toolButtonColumns->setMenu(menu);
    ui->toolButtonColumns->setPopupMode(QToolButton::InstantPopup);
    for (const auto &flag : flags) {
        QAction *action = menu->addAction(QLocale(flag).nativeLanguageName());
        action->setCheckable(true);
        action->setChecked(true);

        int index = m_dictionaryModel->languageFlags().indexOf(flag);
        m_dictionaryFilterModel->setColumnsVisible(index + 1, true);
        connect(action, &QAction::toggled, this, [this, index](bool checked) {
            m_dictionaryFilterModel->setColumnsVisible(index + 1, checked);
        });
    }

    ui->toolButtonAdd->setIcon(QIcon(":/res/icons/add.svg"));
    ui->toolButtonRemove->setIcon(QIcon(":/res/icons/remove.svg"));
    ui->toolButtonImport->setIcon(QIcon(":/res/icons/file_open.svg"));
    ui->toolButtonExport->setIcon(QIcon(":/res/icons/save_as.svg"));
    ui->toolButtonColumns->setIcon(QIcon(":/res/icons/format_list_bulleted.svg"));

    // clang-format off
    connect(ui->toolButtonAdd, &QToolButton::clicked, this, &TsDictionaryView::onAddPushButtonClicked);
    connect(ui->toolButtonRemove, &QToolButton::clicked, this, &TsDictionaryView::onRemovePushButtonClicked);
    connect(ui->toolButtonImport, &QToolButton::clicked, this, &TsDictionaryView::onImportPushButtonClicked);
    connect(ui->toolButtonExport, &QToolButton::clicked, this, &TsDictionaryView::onExportPushButtonClicked);
    connect(ui->lineEditFilter, &QLineEdit::textChanged, this, &TsDictionaryView::onFilterLineEditTextChanged);
    // clang-format on
}

TsDictionaryView::~TsDictionaryView()
{
    delete ui;
}

void TsDictionaryView::onAddPushButtonClicked()
{
    bool ok = false;
    QString sourceText = QInputDialog::getText(this,
                                               tr("Add New Dictionary Item"),
                                               tr("Source Text:"),
                                               QLineEdit::Normal,
                                               QString("This is a new dictionary item."),
                                               &ok);
    if (ok && !sourceText.isEmpty()) {
        m_dictionaryModel->addItem(sourceText);
    }
}

void TsDictionaryView::onRemovePushButtonClicked()
{
    if (m_dictionaryModel->rowCount() == 0) {
        return;
    }

    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this,
                             tr("Remove Dictionary Item"),
                             tr("Please select an item to remove."));
        return;
    }

    int ret = QMessageBox::question(this,
                                    tr("Remove Dictionary Item"),
                                    tr("Are you sure you want to remove the selected item?"),
                                    QMessageBox::Yes | QMessageBox::No,
                                    QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }

    int row = m_dictionaryFilterModel->mapToSource(index).row();
    m_dictionaryModel->removeItem(row);
}

void TsDictionaryView::onImportPushButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Import Dictionary"),
                                                    QString("Dictionary.json"),
                                                    tr("Dictionary Files (*.json)"));
    if (fileName.isEmpty()) {
        return;
    }

    QJsonObject obj = m_dictionaryModel->save();
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QMessageBox::critical(this,
                              tr("Import Dictionary"),
                              tr("Failed to open file for writing: %1").arg(file.errorString()));
        return;
    }

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson(QJsonDocument::Indented);
    QTextStream out(&file);
    out << data;
    file.close();
}

void TsDictionaryView::onExportPushButtonClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Export Dictionary"),
                                                    QString("Dictionary.json"),
                                                    tr("Dictionary Files (*.json)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this,
                              tr("Export Dictionary"),
                              tr("Failed to open file for reading: %1").arg(file.errorString()));
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::critical(this,
                              tr("Export Dictionary"),
                              tr("Failed to parse JSON: %1 at offset %2")
                                  .arg(parseError.errorString())
                                  .arg(parseError.offset));
        return;
    }

    QJsonObject obj = doc.object();
    m_dictionaryModel->load(obj);
}

void TsDictionaryView::onFilterLineEditTextChanged(const QString &text)
{
    m_dictionaryFilterModel->setFilterFixedString(text);
}