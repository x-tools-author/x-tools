/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "commandline.h"
#include "commandlineui.h"

#include <QEventLoop>
#include <QProcess>
#include <QThread>

#include "commandlinethread.h"

CommandLine::CommandLine(QObject *parent)
    : BaseNode(parent)
{
    m_handleInDataInThread = true;
    auto thread = new CommandLineThread(this);
    thread->start();
}

CommandLine::~CommandLine() {}

void CommandLine::execute()
{
    emit input2thread(std::make_shared<BaseNodeData>(QByteArray("")), 0);
}

QString CommandLine::caption() const
{
    return tr("Command Line");
}

QString CommandLine::name() const
{
    return QStringLiteral("CommandLine");
}

unsigned int CommandLine::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 1;
    } else if (portType == QtNodes::PortType::Out) {
        return 1;
    } else {
        return 0;
    }
}

QWidget *CommandLine::embeddedWidget()
{
    if (QThread::currentThread() != thread()) {
        return m_embeddedWidget;
    }

    if (!m_embeddedWidget) {
        m_embeddedWidget = new CommandLineUi(this);
    }

    return m_embeddedWidget;
}

bool CommandLine::resizable() const
{
    return false;
}
