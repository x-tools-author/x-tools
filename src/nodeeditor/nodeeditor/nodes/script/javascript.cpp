/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "javascript.h"
#include "javascriptui.h"

#include <QFile>
#include <QJSEngine>
#include <QThread>

#include "javascriptthread.h"

JavaScript::JavaScript(QObject *parent)
    : BaseNode(parent)
    , m_embeddedWidget(nullptr)
{
    m_handleInDataInThread.store(true);
    auto thread = new JavaScriptThread(this);
    thread->start();
}

JavaScript::~JavaScript() {}

QString JavaScript::caption() const
{
    return tr("Java Script");
}

QString JavaScript::name() const
{
    return QString("JavaScript");
}

unsigned int JavaScript::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 1;
    } else if (portType == QtNodes::PortType::Out) {
        return 1;
    } else {
        return 0;
    }
}

QWidget *JavaScript::embeddedWidget()
{
    if (!m_embeddedWidget) {
        m_embeddedWidget = new JavaScriptUi(this);
    }

    return m_embeddedWidget;
}

bool JavaScript::resizable() const
{
    return true;
}
