/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <thread>

#include <QMutex>
#include <QThread>

class SvgFileDataModel;
class SvgScanner : public QThread
{
    Q_OBJECT
public:
    explicit SvgScanner(SvgFileDataModel *model, QObject *parent = nullptr);

    void didStartScanner(const QString &rootPath, const QString &filter);

protected:
    void run() override;

private:
    QString m_rootPath;
    QString m_filter;
    QList<std::thread *> m_threads;
    QMutex m_threadsMutex;

    SvgFileDataModel *m_model;

private:
    void didStartScannerActually(const QString &path);
};
