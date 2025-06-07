/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "svgfiledatamodel.h"

#include <QDir>

#include "svgscanner.h"

SvgFileDataModel::SvgFileDataModel(int column, QObject *parent)
    : QAbstractTableModel{parent}
{
    m_parameters.column = column;
    m_scanner = new SvgScanner(this, this);
    connect(m_scanner, &SvgScanner::finished, this, &SvgFileDataModel::onScannerFinished);
}

void SvgFileDataModel::update(const QString &path, const QString &filter)
{
    m_svgFilesMutex.lock();
    m_svgFiles.clear();
    m_svgFilesMutex.unlock();

    m_scanner->didStartScanner(path, filter);
}

void SvgFileDataModel::updateColumn(int column)
{
    m_parameters.column = column;
}

void SvgFileDataModel::updateRecursive(bool recursive)
{
    m_parameters.recursive = recursive;
}

void SvgFileDataModel::updateOverrideColor(const QString &overrideColor)
{
    m_parameters.overrideColor = overrideColor;
}

void SvgFileDataModel::appendSvgFile(const QString &svgFile)
{
    m_svgFilesMutex.lock();
    m_svgFiles.append(svgFile);
    m_svgFilesMutex.unlock();
}

QStringList SvgFileDataModel::svgFiles() const
{
    return m_svgFiles;
}

int SvgFileDataModel::rowCount(const QModelIndex &index) const
{
    if (m_svgFiles.isEmpty()) {
        return 0;
    }

    int row1 = m_svgFiles.size() / m_parameters.column;
    int row2 = m_svgFiles.size() % m_parameters.column;
    return row1 + (row2 > 0 ? 1 : 0);
}

int SvgFileDataModel::columnCount(const QModelIndex &index) const
{
    return m_parameters.column;
}

QVariant SvgFileDataModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int column = index.column();
    int listIndex = row * m_parameters.column + column;
    QString svgFile{"0"};

    m_svgFilesMutex.lock();
    if (listIndex >= 0 && listIndex < m_svgFiles.size()) {
        svgFile = m_svgFiles.at(listIndex);
    }
    m_svgFilesMutex.unlock();

    return QVariant::fromValue(svgFile);
}

void SvgFileDataModel::onScannerFinished()
{
    auto info = QString("SVG scan finished, row: %1, column: %2").arg(rowCount()).arg(columnCount());
    qInfo() << qPrintable(info);
    emit dataReady();
}
