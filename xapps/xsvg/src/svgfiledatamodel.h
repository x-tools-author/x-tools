/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>
#include <QMutex>
#include <QObject>

class SvgScanner;
class SvgFileDataModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    struct Parameters
    {
        bool recursive;
        int column;
        QString overrideColor;
    } m_parameters;

public:
    explicit SvgFileDataModel(int column, QObject *parent = nullptr);
    void update(const QString &path, const QString &filter);
    void updateColumn(int column);
    void updateRecursive(bool recursive);
    void updateOverrideColor(const QString &overrideColor);
    void appendSvgFile(const QString &svgFile);
    QStringList svgFiles() const;

public:
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

signals:
    void dataReady();

private:
    QStringList m_svgFiles;
    mutable QMutex m_svgFilesMutex;
    SvgScanner *m_scanner;

private:
    void onScannerFinished();
};
