/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "svgscanner.h"

#include <QDir>
#include <QTimer>

#include "svgfiledatamodel.h"

#if 0
#define USING_MULTITHREAD
#endif

SvgScanner::SvgScanner(SvgFileDataModel *model, QObject *parent)
    : QThread{parent}
    , m_model{model}
{}

void SvgScanner::didStartScanner(const QString &rootPath, const QString &filter)
{
    if (isRunning()) {
        return;
    }

    m_rootPath = rootPath;
    m_filter = filter;
    start();
}

void SvgScanner::run()
{
    if (m_rootPath.isEmpty()) {
        return;
    }

    didStartScannerActually(m_rootPath);
#if defined(USING_MULTITHREAD)
    std::this_thread::sleep_for(std::chrono::seconds(1));
    while (!this->m_threads.isEmpty()) {
        m_threadsMutex.lock();
        auto task = this->m_threads.takeFirst();
        m_threadsMutex.unlock();
        task->join();
        delete task;

        m_threadsMutex.lock();
        if (this->m_threads.isEmpty()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        m_threadsMutex.unlock();
    }
#endif
}

void SvgScanner::didStartScannerActually(const QString &path)
{
    QDir dir(path);
    QList<QFileInfo> infos = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs
                                               | QDir::NoSymLinks);
    for (auto &info : infos) {
        if (info.isDir()) {
#if defined(USING_MULTITHREAD)
            qInfo() << info.absoluteFilePath();
            auto task = new std::thread(&SvgScanner::didStartScannerActually,
                                        this,
                                        info.absoluteFilePath());
            m_threadsMutex.lock();
            m_threads.append(task);
            m_threadsMutex.unlock();
#else
            didStartScannerActually(info.absoluteFilePath());
#endif
        } else {
            bool endsWithSvg = info.fileName().endsWith(QString("svg"), Qt::CaseInsensitive);
            bool hasFilter = info.fileName().contains(m_filter, Qt::CaseInsensitive);
            if (endsWithSvg && hasFilter) {
                m_model->appendSvgFile(info.absoluteFilePath());
            }
        }
    }
}
