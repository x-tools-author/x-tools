/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tftpassistant.h"

#include "src/tftpclientui.h"
#include "src/tftpserverui.h"

TftpAssistant::TftpAssistant(QWidget* parent)
    : QWidget(parent)
{
    m_tabWidget = new QTabWidget(this);
    m_layout = new QVBoxLayout(this);
    m_layout->addWidget(m_tabWidget);
    m_layout->setContentsMargins(4, 4, 4, 4);
    setLayout(m_layout);

    m_serverUi = new TftpServerUi(this);
    m_tabWidget->addTab(m_serverUi, tr("Server"));

    m_clientUi = new TftpClientUi(this);
    m_tabWidget->addTab(m_clientUi, tr("Client"));
}

TftpAssistant::~TftpAssistant() {}