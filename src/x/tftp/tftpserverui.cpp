/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tftpserverui.h"
#include "ui_tftpserverui.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>

TftpServerUi::TftpServerUi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TftpServerUi)
    , m_tftpServer(new TftpServer(this))
{
    ui->setupUi(this);
    initUI();
    
    // 连接信号和槽
    connect(m_tftpServer, &TftpServer::clientConnected, this, &TftpServerUi::handleClientConnected);
    connect(m_tftpServer, &TftpServer::clientDisconnected, this, &TftpServerUi::handleClientDisconnected);
    connect(m_tftpServer, &TftpServer::transferProgress, this, &TftpServerUi::handleTransferProgress);
    connect(m_tftpServer, &TftpServer::errorOccurred, this, &TftpServerUi::handleError);
    connect(m_tftpServer, &TftpServer::transferCompleted, this, &TftpServerUi::handleTransferCompleted);
    
    // 默认根路径设置为当前目录
    ui->lineEditRootPath->setText(QDir::currentPath());
    
    log("TFTP Server initialized");
}

TftpServerUi::~TftpServerUi()
{
    delete ui;
}

void TftpServerUi::on_pushButtonBrowseRoot_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select Root Directory", ui->lineEditRootPath->text());
    if (!path.isEmpty()) {
        ui->lineEditRootPath->setText(path);
    }
}

void TftpServerUi::on_pushButtonStart_clicked()
{
    if (!validateInput()) {
        return;
    }
    
    quint16 port = ui->lineEditPort->text().toUInt();
    QString rootPath = ui->lineEditRootPath->text();
    
    // 设置服务器参数
    m_tftpServer->setRootPath(rootPath);
    
    // 开始服务器
    if (m_tftpServer->start(port)) {
        log(QString("TFTP Server started on port %1").arg(port));
        updateServerStatus(true);
    } else {
        log("Failed to start TFTP Server");
        QMessageBox::critical(this, "Server Error", "Failed to start TFTP Server");
    }
}

void TftpServerUi::on_pushButtonStop_clicked()
{
    m_tftpServer->stop();
    log("TFTP Server stopped");
    updateServerStatus(false);
}

void TftpServerUi::handleClientConnected(const QHostAddress &clientAddress, quint16 clientPort)
{
    QString clientAddr = clientAddress.toString();
    log(QString("Client connected: %1:%2").arg(clientAddr).arg(clientPort));
}

void TftpServerUi::handleClientDisconnected(const QHostAddress &clientAddress, quint16 clientPort)
{
    QString clientAddr = clientAddress.toString();
    log(QString("Client disconnected: %1:%2").arg(clientAddr).arg(clientPort));
}

void TftpServerUi::handleTransferProgress(const QString &fileName, const QHostAddress &clientAddress, int progress)
{
    QString clientAddr = clientAddress.toString();
    log(QString("Transfer progress from %1: %2 - %3%%").arg(clientAddr).arg(fileName).arg(progress));
}

void TftpServerUi::handleError(const QString &errorMessage)
{
    log(QString("Error: %1").arg(errorMessage));
    QMessageBox::warning(this, "Server Error", errorMessage);
}

void TftpServerUi::handleTransferCompleted(const QString &fileName, const QHostAddress &clientAddress, bool isUpload)
{
    QString clientAddr = clientAddress.toString();
    QString action = isUpload ? "uploaded" : "downloaded";
    log(QString("File %1 %2 by %3").arg(fileName).arg(action).arg(clientAddr));
}

void TftpServerUi::initUI()
{
    updateServerStatus(false);
}

void TftpServerUi::log(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->textEditLog->append(QString("[%1] %2").arg(timestamp).arg(message));
    
    // 自动滚动到底部
    QTextCursor cursor = ui->textEditLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEditLog->setTextCursor(cursor);
}

void TftpServerUi::updateServerStatus(bool running)
{
    if (running) {
        ui->labelStatusValue->setText("Running");
        ui->labelStatusValue->setStyleSheet("color: green;");
        ui->pushButtonStart->setEnabled(false);
        ui->pushButtonStop->setEnabled(true);
        ui->lineEditPort->setEnabled(false);
        ui->lineEditRootPath->setEnabled(false);
        ui->pushButtonBrowseRoot->setEnabled(false);
    } else {
        ui->labelStatusValue->setText("Not Running");
        ui->labelStatusValue->setStyleSheet("color: red;");
        ui->pushButtonStart->setEnabled(true);
        ui->pushButtonStop->setEnabled(false);
        ui->lineEditPort->setEnabled(true);
        ui->lineEditRootPath->setEnabled(true);
        ui->pushButtonBrowseRoot->setEnabled(true);
    }
}

bool TftpServerUi::validateInput() const
{
    bool portOk = false;
    quint16 port = ui->lineEditPort->text().toUInt(&portOk);
    if (!portOk || port == 0) {
        QMessageBox::warning(const_cast<TftpServerUi*>(this), "Input Error", "Please enter valid port");
        return false;
    }
    
    if (ui->lineEditRootPath->text().isEmpty()) {
        QMessageBox::warning(const_cast<TftpServerUi*>(this), "Input Error", "Please select root path");
        return false;
    }
    
    return true;
}
