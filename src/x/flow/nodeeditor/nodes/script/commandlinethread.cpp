/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "commandlinethread.h"

#include <QEventLoop>
#include <QProcess>
#include <QThread>

#include "commandline.h"
#include "commandlineui.h"

CommandLineThread::CommandLineThread(CommandLine *cmdLine)
    : QThread(cmdLine)
    , m_cmdLine(cmdLine)
{}

CommandLineThread::~CommandLineThread()
{
    if (isRunning()) {
        quit();
        wait();
    }
}

void CommandLineThread::run()
{
    QObject *obj = new QObject();
    connect(m_cmdLine,
            &CommandLine::input2thread,
            obj,
            [this](std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex const portIndex) {
                this->onInput2thread(nodeData, portIndex);
            });
    exec();
    obj->deleteLater();
}

void CommandLineThread::onInput2thread(std::shared_ptr<QtNodes::NodeData> nodeData,
                                       QtNodes::PortIndex const)
{
    auto cookedNodeData = std::dynamic_pointer_cast<BaseNodeData>(nodeData);
    if (!cookedNodeData) {
        return;
    }

    QByteArray bytes = cookedNodeData->bytes();
    QProcess process;
    CommandLineParametersKeys keys;
    QJsonObject parameters = m_cmdLine->save();
    const QString program = parameters[keys.program].toString();
    if (program.isEmpty()) {
        qWarning() << "CommandLine: program is empty!";
        return;
    }

    const QString arguments = parameters[keys.arguments].toString();
    // Split arguments by spaces
    static QRegularExpression re(R"(\s+)");
    QStringList args = arguments.split(re, Qt::SkipEmptyParts);
    bool isHexParameters = parameters[keys.isHexParameters].toBool(false);
    bool isUtf8Parameters = parameters[keys.isUtf8Parameters].toBool(true);
    if (!bytes.isEmpty()) {
        if (isHexParameters) {
            args << QString::fromLatin1(bytes.toHex());
        } else if (isUtf8Parameters) {
            args << QString::fromLocal8Bit(QString::fromUtf8(bytes).toLocal8Bit());
        }
    }

#if 0
        qInfo("The command line is: %s %s",
              program.toUtf8().constData(),
              args.join(QLatin1Char(' ')).toUtf8().constData());
#endif

    QEventLoop loop;
    QByteArray all;
    connect(&process, &QProcess::readAllStandardOutput, &loop, [&process, &all]() {
        all.append(process.readAll());
    });
    connect(&process, &QProcess::finished, &loop, [&loop](int exitCode, QProcess::ExitStatus) {
        loop.quit();
    });

    process.start(program, args);
    loop.exec();
    if (!all.isEmpty()) {
        QByteArray utf8 = QString::fromLocal8Bit(all).toUtf8();
        m_cmdLine->setOutData(std::make_shared<BaseNodeData>(utf8));
        emit m_cmdLine->dataUpdated(0);
    }
}