/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tsfileview.h"

#include <QFileInfo>

#include "tsfile.h"
#include "tsfilefilter.h"
#include "tsfileitemdelegate.h"

TsFileView::TsFileView(const QString &filePath, QWidget *parent)
    : QListView(parent)
{
    setItemDelegateForColumn(0, new TsFileItemDelegate(this));

    QFileInfo fileInfo(filePath);
    this->setWindowTitle(fileInfo.fileName());

    m_tsFile = new TsFile(filePath, this);
    m_filterModel = new TsFileFilter(this);
    m_filterModel->setSourceModel(m_tsFile);
    this->setModel(m_filterModel);
    this->setAlternatingRowColors(true);
}

TsFileView::~TsFileView() {}

TsFile *TsFileView::tsFile() const
{
    return m_tsFile;
}
