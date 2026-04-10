#include "tftpserver.h"
#include "ui_tftpserver.h"

#include <QDateTime>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QProgressBar>
#include <QTableWidgetItem>

#include "common/x.h"
#include "tftpserverservice.h"

TftpServer::TftpServer(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TftpServer)
    , m_service(new TftpServerService(this))
{
    ui->setupUi(this);
    setupUiState();

    connect(ui->pushButtonBrowse, &QPushButton::clicked, this, &TftpServer::browseDirectory);
    connect(ui->pushButtonStartStop, &QPushButton::clicked, this, &TftpServer::toggleServer);
    connect(m_service, &TftpServerService::logMessage, this, &TftpServer::appendLog);
    connect(m_service, &TftpServerService::sessionCreated, this, &TftpServer::onSessionCreated);
    connect(m_service, &TftpServerService::sessionUpdated, this, &TftpServer::onSessionUpdated);
    connect(m_service, &TftpServerService::sessionFinished, this, &TftpServer::onSessionFinished);
}

TftpServer::~TftpServer()
{
    delete ui;
}

void TftpServer::setupUiState()
{
    ui->comboBoxServerAddress->setEditable(true);
    xSetupSocketAddress(ui->comboBoxServerAddress);
    if (ui->comboBoxServerAddress->findText(QStringLiteral("0.0.0.0")) < 0) {
        ui->comboBoxServerAddress->insertItem(0, QStringLiteral("0.0.0.0"));
    }
    ui->comboBoxServerAddress->setCurrentText(QStringLiteral("0.0.0.0"));
    ui->lineEditServerDirectory->setText(QDir::currentPath());

    ui->tableWidgetSession->setColumnCount(6);
    ui->tableWidgetSession->setHorizontalHeaderLabels(
        QStringList() << tr("Peer") << tr("Direction") << tr("File") << tr("Bytes")
                      << tr("Progress") << tr("Status"));
    ui->tableWidgetSession->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetSession->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidgetSession->setRowCount(0);
}

void TftpServer::appendLog(const QString& message)
{
    const QString timestamp = QDateTime::currentDateTime().toString(
        QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    ui->textBrowserLog->append(QStringLiteral("[%1] %2").arg(timestamp, message.toHtmlEscaped()));
}

QString TftpServer::selectedServerAddress() const
{
    QString address = ui->comboBoxServerAddress->currentText().trimmed();
    if (address.isEmpty()) {
        address = ui->comboBoxServerAddress->currentData().toString().trimmed();
    }

    return address;
}

void TftpServer::browseDirectory()
{
    const QString directory
        = QFileDialog::getExistingDirectory(this,
                                            tr("Select server directory"),
                                            ui->lineEditServerDirectory->text().trimmed());
    if (!directory.isEmpty()) {
        ui->lineEditServerDirectory->setText(directory);
    }
}

void TftpServer::toggleServer()
{
    if (m_service->isRunning()) {
        m_service->stop();
        ui->pushButtonStartStop->setText(tr("Start"));
        appendLog(tr("Server stopped"));
        return;
    }

    const QString directory = ui->lineEditServerDirectory->text().trimmed();
    if (directory.isEmpty()) {
        QMessageBox::warning(this, tr("TFTP Server"), tr("Please select a server directory."));
        return;
    }

    QDir dir(directory);
    if (!dir.exists() && !dir.mkpath(QStringLiteral("."))) {
        QMessageBox::warning(this, tr("TFTP Server"), tr("Failed to create server directory."));
        return;
    }

    QHostAddress address;
    const QString addressText = selectedServerAddress();
    if (addressText == QStringLiteral("0.0.0.0") || addressText.isEmpty()) {
        address = QHostAddress::AnyIPv4;
    } else {
        address = QHostAddress(addressText);
    }

    if (address.isNull()) {
        QMessageBox::warning(this, tr("TFTP Server"), tr("Invalid server address."));
        return;
    }

    QString errorMessage;
    if (!m_service->start(address, dir.absolutePath(), &errorMessage)) {
        QMessageBox::warning(this, tr("TFTP Server"), errorMessage);
        return;
    }

    ui->pushButtonStartStop->setText(tr("Stop"));
    appendLog(tr("Server started at %1, directory: %2").arg(address.toString(), dir.absolutePath()));
}

int TftpServer::ensureSessionRow(const QString& sessionId,
                                 const QString& peer,
                                 const QString& direction,
                                 const QString& fileName,
                                 qint64 totalBytes,
                                 const QString& status)
{
    if (m_sessionRows.contains(sessionId)) {
        return m_sessionRows.value(sessionId).row;
    }

    const int row = ui->tableWidgetSession->rowCount();
    ui->tableWidgetSession->insertRow(row);
    ui->tableWidgetSession->setItem(row, 0, new QTableWidgetItem(peer));
    ui->tableWidgetSession->setItem(row, 1, new QTableWidgetItem(direction));
    ui->tableWidgetSession->setItem(row, 2, new QTableWidgetItem(fileName));
    ui->tableWidgetSession->setItem(row,
                                    3,
                                    new QTableWidgetItem(
                                        totalBytes >= 0 ? tr("0/%1").arg(totalBytes) : tr("0")));
    ui->tableWidgetSession->setItem(row, 5, new QTableWidgetItem(status));

    QProgressBar* progressBar = new QProgressBar(ui->tableWidgetSession);
    progressBar->setTextVisible(true);
    if (totalBytes > 0) {
        progressBar->setRange(0, 100);
        progressBar->setValue(0);
        progressBar->setFormat(QStringLiteral("0%"));
    } else {
        progressBar->setRange(0, 0);
    }
    ui->tableWidgetSession->setCellWidget(row, 4, progressBar);

    SessionRow sessionRow;
    sessionRow.row = row;
    sessionRow.progressBar = progressBar;
    m_sessionRows.insert(sessionId, sessionRow);
    return row;
}

void TftpServer::onSessionCreated(const QString& sessionId,
                                  const QString& peer,
                                  const QString& direction,
                                  const QString& fileName,
                                  qint64 totalBytes,
                                  const QString& status)
{
    ensureSessionRow(sessionId, peer, direction, fileName, totalBytes, status);
}

void TftpServer::onSessionUpdated(const QString& sessionId,
                                  qint64 transferredBytes,
                                  qint64 totalBytes,
                                  const QString& status)
{
    if (!m_sessionRows.contains(sessionId)) {
        return;
    }

    const SessionRow sessionRow = m_sessionRows.value(sessionId);
    if (QTableWidgetItem* item = ui->tableWidgetSession->item(sessionRow.row, 3)) {
        if (totalBytes >= 0) {
            item->setText(tr("%1/%2").arg(transferredBytes).arg(totalBytes));
        } else {
            item->setText(QString::number(transferredBytes));
        }
    }
    if (QTableWidgetItem* item = ui->tableWidgetSession->item(sessionRow.row, 5)) {
        item->setText(status);
    }

    if (sessionRow.progressBar) {
        if (totalBytes > 0) {
            sessionRow.progressBar->setRange(0, 100);
            const int percent = qBound(0,
                                       static_cast<int>((transferredBytes * 100) / totalBytes),
                                       100);
            sessionRow.progressBar->setValue(percent);
            sessionRow.progressBar->setFormat(QStringLiteral("%1%").arg(percent));
        } else {
            sessionRow.progressBar->setRange(0, 0);
        }
    }
}

void TftpServer::onSessionFinished(const QString& sessionId, const QString& status)
{
    if (!m_sessionRows.contains(sessionId)) {
        return;
    }

    const SessionRow sessionRow = m_sessionRows.value(sessionId);
    if (QTableWidgetItem* item = ui->tableWidgetSession->item(sessionRow.row, 5)) {
        item->setText(status);
    }
    if (sessionRow.progressBar && sessionRow.progressBar->maximum() == 100
        && status.contains(tr("completed"), Qt::CaseInsensitive)) {
        sessionRow.progressBar->setValue(100);
        sessionRow.progressBar->setFormat(QStringLiteral("100%"));
    }
}