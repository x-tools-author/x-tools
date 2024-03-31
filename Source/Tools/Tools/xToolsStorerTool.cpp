/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsStorerTool.h"

#include <QDataStream>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>

xToolsStorerTool::xToolsStorerTool(QObject *parent)
    : xToolsBaseTool{parent}
{}

xToolsStorerTool::~xToolsStorerTool()
{

}

void xToolsStorerTool::inputBytes(const QByteArray &bytes)
{
    if (isEnable()) {
        m_inputBytesListMutex.lock();
        m_inputBytesList.append(bytes);
        m_inputBytesListMutex.unlock();
    }
}

bool xToolsStorerTool::saveRx()
{
    m_parametersMutex.lock();
    auto tmp = m_parameters.saveRx;
    m_parametersMutex.unlock();
    return tmp;
}

void xToolsStorerTool::setSaveRx(bool save)
{
    m_parametersMutex.lock();
    m_parameters.saveRx = save;
    m_parametersMutex.unlock();
    emit saveRxChanged();
}

bool xToolsStorerTool::saveTx()
{
    m_parametersMutex.lock();
    auto tmp = m_parameters.saveTx;
    m_parametersMutex.unlock();
    return tmp;
}

void xToolsStorerTool::setSaveTx(bool save)
{
    m_parametersMutex.lock();
    m_parameters.saveTx = save;
    m_parametersMutex.unlock();
    emit saveTxChanged();
}

QString xToolsStorerTool::fileName()
{
    m_parametersMutex.lock();
    auto tmp = m_parameters.file;
    m_parametersMutex.unlock();
    return tmp;
}

void xToolsStorerTool::setFileName(const QString &name)
{
    m_parametersMutex.lock();
    m_parameters.file = name;
    m_parametersMutex.unlock();
    emit fileNameChanged();
}

void xToolsStorerTool::run()
{
    QTimer *writeTimer = new QTimer();
    writeTimer->setInterval(2000);
    writeTimer->setSingleShot(true);
    connect(writeTimer, &QTimer::timeout, writeTimer, [=]() {
        m_parametersMutex.lock();
        m_inputBytesListMutex.lock();
        this->write2file();
        m_inputBytesListMutex.unlock();
        m_parametersMutex.unlock();

        writeTimer->start();
    });
    writeTimer->start();

    exec();

    writeTimer->stop();
    writeTimer->deleteLater();
    writeTimer = nullptr;
    write2file();
    m_inputBytesList.clear();
}

void xToolsStorerTool::write2file()
{
    if (m_parameters.file.isEmpty()) {
        m_inputBytesList.clear();
        return;
    }

    QString path = m_parameters.file;
    QFile file(path);
    // Backup the file.
    if (file.exists() && (file.size() > 1024 * 1024)) {
        const QString format = QString("yyyy-MM-dd-hh-mm-ss_");
        auto dt = QDateTime::currentDateTime().toString(format);
        QDir dir(path);
        auto pathTemp = path;
        auto fileNameTemp = dir.dirName();
        pathTemp = pathTemp.remove(fileNameTemp);
        QString newFileName = pathTemp + dt + fileNameTemp;
        file.copy(newFileName);
        file.open(QFile::WriteOnly | QFile::Truncate);
        file.close();
    } else {
        qWarning() << QString("the file(%1) is not exist").arg(path);
    }

    // Write the data to the file.
    if (file.open(QFile::WriteOnly | QFile::Text | QFile::Append)) {
        QDataStream outStream(&file);
        while (!m_inputBytesList.isEmpty()) {
            auto bytes = m_inputBytesList.takeFirst();
            outStream << bytes << "\n";
        }
        file.close();
    } else {
        qWarning() << file.errorString();
    }
}
