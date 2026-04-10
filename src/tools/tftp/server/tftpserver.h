/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMap>
#include <QWidget>

class QProgressBar;
class TftpServerService;

namespace Ui {
class TftpServer;
}

class TftpServer : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit TftpServer(QWidget* parent = Q_NULLPTR);
    ~TftpServer();

private:
    struct SessionRow
    {
        int row{-1};
        QProgressBar* progressBar{Q_NULLPTR};
    };

    Ui::TftpServer* ui;
    TftpServerService* m_service;
    QMap<QString, SessionRow> m_sessionRows;

private:
    void setupUiState();
    void appendLog(const QString& message);
    int ensureSessionRow(const QString& sessionId,
                         const QString& peer,
                         const QString& direction,
                         const QString& fileName,
                         qint64 totalBytes,
                         const QString& status);
    QString selectedServerAddress() const;

private slots:
    void browseDirectory();
    void toggleServer();
    void onSessionCreated(const QString& sessionId,
                          const QString& peer,
                          const QString& direction,
                          const QString& fileName,
                          qint64 totalBytes,
                          const QString& status);
    void onSessionUpdated(const QString& sessionId,
                          qint64 transferredBytes,
                          qint64 totalBytes,
                          const QString& status);
    void onSessionFinished(const QString& sessionId, const QString& status);
};