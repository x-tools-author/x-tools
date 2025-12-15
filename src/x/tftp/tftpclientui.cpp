/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tftpclientui.h"
#include "ui_tftpclientui.h"
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>

TftpClientUi::TftpClientUi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TftpClientUi)
    , m_tftpClient(new TftpClient(this))
{
    ui->setupUi(this);
    initUI();

    // 连接信号和槽
    //connect(m_tftpClient, &TftpClient::progressChanged, this, &TftpClientUi::updateProgress);
    connect(m_tftpClient, &TftpClient::errorOccurred, this, &TftpClientUi::handleError);
    connect(m_tftpClient,
            &TftpClient::transferCompleted,
            this,
            &TftpClientUi::handleTransferCompleted);

    // 默认本地路径设置为当前目录
    ui->lineEditLocalPath->setText(QDir::currentPath());

    log("TFTP Client initialized");
}

TftpClientUi::~TftpClientUi()
{
    delete ui;
}

void TftpClientUi::on_pushButtonBrowseLocal_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,
                                                     "Select Local Directory",
                                                     ui->lineEditLocalPath->text());
    if (!path.isEmpty()) {
        ui->lineEditLocalPath->setText(path);
    }
}

void TftpClientUi::on_pushButtonDownload_clicked()
{
    if (!validateInput()) {
        return;
    }

    QString serverAddr = ui->lineEditServerAddr->text();
    quint16 port = ui->lineEditPort->text().toUInt();
    QString localPath = ui->lineEditLocalPath->text();
    QString remoteFile = ui->lineEditRemoteFile->text();

    // 设置客户端参数
    //m_tftpClient->setServerAddress(serverAddr, port);
    m_tftpClient->setLocalWorkPath(localPath);

    // 开始下载
    if (m_tftpClient->downloadFile(remoteFile)) {
        log(QString("Download started: %1 from %2:%3").arg(remoteFile).arg(serverAddr).arg(port));
        ui->pushButtonDownload->setEnabled(false);
        ui->pushButtonUpload->setEnabled(false);
        ui->pushButtonStop->setEnabled(true);
    } else {
        log("Failed to start download");
    }
}

void TftpClientUi::on_pushButtonUpload_clicked()
{
    if (!validateInput()) {
        return;
    }

    QString serverAddr = ui->lineEditServerAddr->text();
    quint16 port = ui->lineEditPort->text().toUInt();
    QString localPath = ui->lineEditLocalPath->text();
    QString remoteFile = ui->lineEditRemoteFile->text();

    // 上传需要先选择本地文件
    QString localFile = QFileDialog::getOpenFileName(this, "Select File to Upload", localPath);
    if (localFile.isEmpty()) {
        return;
    }

    // 从本地文件路径提取文件名
    QFileInfo fileInfo(localFile);
    QString fileName = fileInfo.fileName();

    // 如果没有指定远程文件名，则使用本地文件名
    if (remoteFile.isEmpty()) {
        remoteFile = fileName;
        ui->lineEditRemoteFile->setText(fileName);
    }

    // 设置客户端参数
    //m_tftpClient->setServerAddress(serverAddr, port);
    m_tftpClient->setLocalWorkPath(localPath);

    // 开始上传
    if (m_tftpClient->uploadFile(localFile, remoteFile)) {
        log(QString("Upload started: %1 to %2:%3 as %4")
                .arg(localFile)
                .arg(serverAddr)
                .arg(port)
                .arg(remoteFile));
        ui->pushButtonDownload->setEnabled(false);
        ui->pushButtonUpload->setEnabled(false);
        ui->pushButtonStop->setEnabled(true);
    } else {
        log("Failed to start upload");
    }
}

void TftpClientUi::on_pushButtonStop_clicked()
{
    //m_tftpClient->stopTransfer();
    log("Transfer stopped by user");
    ui->pushButtonDownload->setEnabled(true);
    ui->pushButtonUpload->setEnabled(true);
    ui->pushButtonStop->setEnabled(false);
}

void TftpClientUi::updateProgress(int progress)
{
    ui->progressBar->setValue(progress);
    log(QString("Progress: %1%").arg(progress));
}

void TftpClientUi::handleError(const QString &errorMessage)
{
    log(QString("Error: %1").arg(errorMessage));
    QMessageBox::warning(this, "TFTP Error", errorMessage);

    // 恢复按钮状态
    ui->pushButtonDownload->setEnabled(true);
    ui->pushButtonUpload->setEnabled(true);
    ui->pushButtonStop->setEnabled(false);
}

void TftpClientUi::handleTransferCompleted(const QString &fileName)
{
    log(QString("Transfer completed: %1").arg(fileName));
    QMessageBox::information(this,
                             "TFTP Complete",
                             QString("File transfer completed: %1").arg(fileName));

    // 恢复按钮状态
    ui->pushButtonDownload->setEnabled(true);
    ui->pushButtonUpload->setEnabled(true);
    ui->pushButtonStop->setEnabled(false);

    // 重置进度条
    ui->progressBar->setValue(0);
}

void TftpClientUi::initUI()
{
    ui->progressBar->setRange(0, 100);
    ui->pushButtonStop->setEnabled(false);
}

void TftpClientUi::log(const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->textEditLog->append(QString("[%1] %2").arg(timestamp).arg(message));

    // 自动滚动到底部
    QTextCursor cursor = ui->textEditLog->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEditLog->setTextCursor(cursor);
}

bool TftpClientUi::validateInput() const
{
    if (ui->lineEditServerAddr->text().isEmpty()) {
        QMessageBox::warning(const_cast<TftpClientUi *>(this),
                             "Input Error",
                             "Please enter server address");
        return false;
    }

    bool portOk = false;
    quint16 port = ui->lineEditPort->text().toUInt(&portOk);
    if (!portOk || port == 0) {
        QMessageBox::warning(const_cast<TftpClientUi *>(this),
                             "Input Error",
                             "Please enter valid port");
        return false;
    }

    if (ui->lineEditLocalPath->text().isEmpty()) {
        QMessageBox::warning(const_cast<TftpClientUi *>(this),
                             "Input Error",
                             "Please select local path");
        return false;
    }

    if (ui->lineEditRemoteFile->text().isEmpty()) {
        QMessageBox::warning(const_cast<TftpClientUi *>(this),
                             "Input Error",
                             "Please enter remote file name");
        return false;
    }

    return true;
}
