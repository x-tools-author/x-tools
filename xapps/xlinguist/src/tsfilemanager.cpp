/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tsfilemanager.h"

#include <QDir>
#include <QFileInfoList>

#include "tsfile.h"
#include "tsfileview.h"

#define TS_TABLE_COLUMN_FILE 2     // The file base name
#define TS_TABLE_COLUMN_LANGUAGE 0 // The target language name, such as: en, fr, zh_CN
#define TS_TABLE_COLUMN_STATUS 1   // The translation status, such as: 12/666

TsFileManager::TsFileManager(QObject *parent)
    : QAbstractTableModel(parent)
{}

TsFileManager::~TsFileManager() {}

TsFileManager &TsFileManager::instance()
{
    static TsFileManager instance;
    return instance;
}

TsFileView *TsFileManager::tsFileViewAt(int index) const
{
    if (index < 0 || index >= m_tsFileViews.count()) {
        return nullptr;
    }

    return m_tsFileViews.at(index);
}

QList<TsFileView *> TsFileManager::tsFileViews() const
{
    return m_tsFileViews;
}

void TsFileManager::loadTranslationFiles(const QString &dir)
{
    beginResetModel();
    while (!m_tsFileViews.isEmpty()) {
        TsFileView *view = m_tsFileViews.takeFirst();
        view->deleteLater();
    }
    endResetModel();

    QDir directory(dir);
    directory.setNameFilters(QStringList() << "*.ts");
    QFileInfoList fileInfoList = directory.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    if (fileInfoList.isEmpty()) {
        return;
    }

    for (const QFileInfo &info : std::as_const(fileInfoList)) {
        TsFileView *tsFileView = new TsFileView(info.absoluteFilePath());
        m_tsFileViews.append(tsFileView);
    }

    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

void TsFileManager::updateTranslation(const QString &file,
                                      const QString &translation,
                                      int sourceLineNumber)
{
    for (TsFileView *tsFileView : std::as_const(m_tsFileViews)) {
        TsFile *tsFile = tsFileView->tsFile();
        if (tsFile->filePath() == file) {
            tsFile->updateTranslation(translation, sourceLineNumber);
            break;
        }
    }
}

int TsFileManager::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_tsFileViews.count();
}

int TsFileManager::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant TsFileManager::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        TsFileView *tsFileView = m_tsFileViews.at(index.row());
        switch (index.column()) {
        case TS_TABLE_COLUMN_FILE:
            return tsFileView->tsFile()->baseName();
        case TS_TABLE_COLUMN_LANGUAGE:
            return tsFileView->tsFile()->targetLanguage();
        case TS_TABLE_COLUMN_STATUS:
            return tsFileView->tsFile()->translationStatus();
        default:
            return QVariant();
        }
    } else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    return QVariant();
}

bool TsFileManager::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if (row < 0 || row >= m_tsFileViews.count() || count <= 0
        || (row + count) > m_tsFileViews.count()) {
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        TsFileView *view = m_tsFileViews.takeAt(row);
        view->deleteLater();
    }
    endRemoveRows();

    return true;
}

QVariant TsFileManager::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        switch (section) {
        case TS_TABLE_COLUMN_FILE:
            return tr("File");
        case TS_TABLE_COLUMN_LANGUAGE:
            return tr("Target");
        case TS_TABLE_COLUMN_STATUS:
            return tr("STATUS");
        default:
            return QVariant();
        }
    }

    return QVariant();
}