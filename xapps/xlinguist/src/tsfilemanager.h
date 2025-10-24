/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>

#define gTsFileMgr TsFileManager::instance()

class TsFileView;
class TsFileManager : public QAbstractTableModel
{
    Q_OBJECT
private:
    TsFileManager(QObject *parent = nullptr);
    TsFileManager(const TsFileManager &) = delete;
    TsFileManager &operator=(const TsFileManager &) = delete;

public:
    ~TsFileManager();

    static TsFileManager &instance();
    TsFileView *tsFileViewAt(int index) const;
    QList<TsFileView *> tsFileViews() const;
    void loadTranslationFiles(const QString &dir);

    void updateTranslation(const QString &file, const QString &translation, int sourceLineNumber);

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

private:
    QList<TsFileView *> m_tsFileViews;
};