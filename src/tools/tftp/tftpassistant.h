/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

class TftpClientUi;
class TftpServerUi;
class TftpAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit TftpAssistant(QWidget* parent = nullptr);
    ~TftpAssistant();

private:
    QVBoxLayout* m_layout{nullptr};
    TftpClientUi* m_clientUi{nullptr};
    TftpServerUi* m_serverUi{nullptr};
    QTabWidget* m_tabWidget{nullptr};
};