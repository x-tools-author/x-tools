/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tsfileview.h"
#include "ui_tsfileview.h"

#include <QFileInfo>
#include <QListView>

#include "tsfile.h"
#include "tsfilefilter.h"
#include "tsfileitemdelegate.h"

TsFileView::TsFileView(const QString &filePath, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TsFileView)
{
    m_tsFile = new TsFile(filePath, this);
    m_filterModel = new TsFileFilter(this);
    m_filterModel->setSourceModel(m_tsFile);

    ui->setupUi(this);
    ui->listView->setItemDelegateForColumn(0, new TsFileItemDelegate(this));
    ui->listView->setModel(m_filterModel);
    ui->listView->setAlternatingRowColors(true);

    QFileInfo fileInfo(filePath);
    this->setWindowTitle(fileInfo.fileName());

    // clang-format off
    connect(ui->radioButtonAll, &QRadioButton::clicked, this, &TsFileView::onAllItemsChecked);
    connect(ui->radioButtonTranslation, &QRadioButton::clicked, this, &TsFileView::onTranslationItemsChecked);
    connect(ui->radioButtonUnfinished, &QRadioButton::clicked, this, &TsFileView::onUnfinishedItemsChecked);
    connect(ui->lineEditFilter, &QLineEdit::textChanged, this, &TsFileView::onFilterTextChanged);
    // clang-format on

    if (ui->radioButtonAll->isChecked()) {
        onAllItemsChecked(true);
    } else if (ui->radioButtonTranslation->isChecked()) {
        onTranslationItemsChecked(true);
    } else if (ui->radioButtonUnfinished->isChecked()) {
        onUnfinishedItemsChecked(true);
    }
}

TsFileView::~TsFileView()
{
    delete ui;
}

TsFile *TsFileView::tsFile() const
{
    return m_tsFile;
}

void TsFileView::onAllItemsChecked(bool checked)
{
    m_filterModel->setFilterType(TsFileFilter::AllItems);
}

void TsFileView::onTranslationItemsChecked(bool checked)
{
    m_filterModel->setFilterType(TsFileFilter::TranslationItems);
}

void TsFileView::onUnfinishedItemsChecked(bool checked)
{
    m_filterModel->setFilterType(TsFileFilter::UnfinishedItems);
}

void TsFileView::onFilterTextChanged(const QString &text)
{
    m_filterModel->setFilterFixedString(text);
}