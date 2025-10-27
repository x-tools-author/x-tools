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
    connect(ui->checkBoxAll, &QCheckBox::clicked, this, &TsFileView::onAllItemsChecked);
    connect(ui->checkBoxSource, &QCheckBox::clicked, this, &TsFileView::onSourceItemsChecked);
    connect(ui->checkBoxTranslation, &QCheckBox::clicked, this, &TsFileView::onTranslationItemsChecked);
    // clang-format on
}

TsFileView::~TsFileView()
{
    delete ui;
}

TsFile *TsFileView::tsFile() const
{
    return m_tsFile;
}

void TsFileView::onAllItemsChecked(bool checked) {}

void TsFileView::onSourceItemsChecked(bool checked) {}

void TsFileView::onTranslationItemsChecked(bool checked) {}