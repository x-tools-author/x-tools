/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsPingAssistant.h"
#include "ui_xToolsPingAssistant.h"

#include <QApplication>
#include <QButtonGroup>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QProcess>
#include <QRadioButton>
#include <QRunnable>
#include <QStandardPaths>
#include <QStyle>
#include <QThreadPool>

static const QString fileName()
{
    QStandardPaths::StandardLocation type = QStandardPaths::AppConfigLocation;
    QString path = QStandardPaths::writableLocation(type);
    auto ret = QString("%1/%2.ini").arg(path, QCoreApplication::applicationName());
    return ret;
}

class PingTask : public QRunnable
{
public:
    PingTask(xToolsPingAssistant *mainWindow, const QString &ip, int timeout)
    {
        m_mainWindow = mainWindow;
        m_ip = ip;
        m_timeout = timeout;
    }

    void run() override
    {
        QProcess p;
        p.start("ping", QStringList() << "-a" << m_ip);
        m_mainWindow->emitPingStarted(m_ip);
        bool ret = p.waitForFinished();
        if (!ret) {
            p.terminate();
        }

        QByteArray all = p.readAll();
        QString allString = QString::fromUtf8(all);
        QString description = m_ip;
        if (allString.contains("TTL=")) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QStringList list = allString.split(" ", Qt::SkipEmptyParts);
#else
            QStringList list = allString.split(" ", QString::SkipEmptyParts);
#endif
            if (list.length() >= 2) {
                description = list.at(2);
            }
            m_mainWindow->emitPingFinished(m_ip, true, description);
        } else {
            m_mainWindow->emitPingFinished(m_ip, false, description);
        }
    }

private:
    xToolsPingAssistant *m_mainWindow;
    QString m_ip;
    int m_timeout;
};

xToolsPingAssistant::xToolsPingAssistant(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::xToolsPingAssistant)
    , m_pausing(false)
    , m_finishedCount(0)
{
    ui->setupUi(this);

    m_buildDate = QString(__DATE__);
    m_buildTime = QString(__TIME__);

    m_playTimer = new QTimer(this);
    m_playTimer->setInterval(100);
    m_playTimer->setSingleShot(true);
    connect(m_playTimer, &QTimer::timeout, this, &xToolsPingAssistant::onPlayTimerTimeout);

    m_settings = new QSettings(::fileName(), QSettings::IniFormat, this);

    connect(this,
            &xToolsPingAssistant::pingStarted,
            this,
            &xToolsPingAssistant::onPingStarted,
            Qt::QueuedConnection);
    connect(this,
            &xToolsPingAssistant::pingFinished,
            this,
            &xToolsPingAssistant::onPingFinished,
            Qt::QueuedConnection);

#if 0
    int count = QThread::idealThreadCount();
    count = 10*count;
    QThreadPool::globalInstance()->setMaxThreadCount(count);
#endif

    init();
    setWindowTitle("Ping Assistant");
    setWindowIcon(QIcon(":/Resources/Icons/ping.ico"));
    ui->tableWidget->setAlternatingRowColors(true);
    setMinimumSize(720, 480);
}

xToolsPingAssistant::~xToolsPingAssistant()
{
    delete ui;
}

void xToolsPingAssistant::emitPingStarted(const QString &ip)
{
    emit pingStarted(ip);
}

void xToolsPingAssistant::emitPingFinished(const QString &ip,
                                        bool is_online,
                                        const QString &description)
{
    emit pingFinished(ip, is_online, description);
}

void xToolsPingAssistant::init()
{
    m_toolBar = new QToolBar(this);
    ui->horizontalLayoutToolBar->addWidget(m_toolBar);
    ui->horizontalLayoutToolBar->setContentsMargins(0, 0, 0, 0);

    m_statusBar = new QStatusBar(this);
    ui->horizontalLayoutStatusBar->addWidget(m_statusBar);
    ui->horizontalLayoutStatusBar->setContentsMargins(0, 0, 0, 0);

    initTableWidget();
    initToolBar();

    m_progressBar = new QProgressBar(this);
    m_progressBar->setMinimumWidth(200);
    m_progressBar->setFormat("%p%");
    m_progressBar->setTextVisible(true);
#if 0
    m_progressBar->setAlignment(Qt::AlignCenter);
#endif
    m_progressBar->hide();
    m_progressStatus = new QLabel(this);
    QString txt = tr("Total: --, Active: --, Finished: --, Remain: --");
    m_progressStatus->setText(txt);
    m_statusBar->addPermanentWidget(m_progressBar);
    m_statusBar->addPermanentWidget(m_progressStatus);
}

void xToolsPingAssistant::initTableWidget()
{
    ui->tableWidget->setColumnCount(4);
    QStringList header_labels;
    header_labels << tr("Target Address") << tr("Online Status") << tr("Comparing")
                  << tr("Host Name");
    ui->tableWidget->setHorizontalHeaderLabels(header_labels);
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(150);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->hide();
}

void xToolsPingAssistant::initToolBar()
{
    initRunToolBar();
    initSettingToolBar();
    initShowToolBar();
}

void xToolsPingAssistant::initRunToolBar()
{
    QIcon icon = QApplication::style()->standardIcon(QStyle::SP_MediaPlay);
    m_playAction = m_toolBar->addAction(icon, "", [=]() { this->play(); });
    m_playAction->setToolTip(tr("Start scan"));
    connect(m_playAction, &QAction::hovered, this, [=]() {
        m_statusBar->showMessage(m_playAction->toolTip(), 3 * 1000);
    });

    icon = QApplication::style()->standardIcon(QStyle::SP_MediaPause);
    m_pauseAction = m_toolBar->addAction(icon, "", [=]() { this->pause(); });
    m_pauseAction->setEnabled(false);
    m_pauseAction->setToolTip(tr("Pause scan"));
    connect(m_pauseAction, &QAction::hovered, this, [=]() {
        m_statusBar->showMessage(m_pauseAction->toolTip(), 3 * 1000);
    });

    icon = QApplication::style()->standardIcon(QStyle::SP_MediaStop);
    m_stopAction = m_toolBar->addAction(icon, "", [=]() { this->stop(); });
    m_stopAction->setEnabled(false);
    m_stopAction->setToolTip(tr("Stop scan"));
    connect(m_stopAction, &QAction::hovered, this, [=]() {
        m_statusBar->showMessage(m_stopAction->toolTip(), 3 * 1000);
    });
}

void xToolsPingAssistant::initSettingToolBar()
{
    QWidget *w = new QWidget(this);
    QHBoxLayout *hBoxLayout = new QHBoxLayout(w);
    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_toolBar->addWidget(w);

    hBoxLayout->addWidget(new QLabel(tr("Start address")));
    m_beginIpLineEdit = new QLineEdit(this);
    m_beginIpLineEdit->setText(m_settings->value(m_keyCtx.beginIp).toString());
    m_beginIp = m_beginIpLineEdit->text().trimmed();
    hBoxLayout->addWidget(m_beginIpLineEdit);
    connect(m_beginIpLineEdit, &QLineEdit::textChanged, this, [=](const QString &txt) {
        this->m_beginIp = txt;
        m_settings->setValue(this->m_keyCtx.beginIp, txt);
    });

    hBoxLayout->addWidget(new QLabel(tr("End address")));
    m_endIpLineEdit = new QLineEdit(this);
    m_endIpLineEdit->setText(m_settings->value(m_keyCtx.endIp).toString());
    m_endIp = m_endIpLineEdit->text().trimmed();
    hBoxLayout->addWidget(m_endIpLineEdit);
    connect(m_endIpLineEdit, &QLineEdit::textChanged, this, [=](const QString &txt) {
        this->m_endIp = txt;
        m_settings->setValue(this->m_keyCtx.endIp, txt);
    });

#if 0
    hBoxLayout->addWidget(new QLabel(tr("Timeout(S)")));
#endif
    m_timeoutSpinBox = new QSpinBox(this);
    m_timeoutSpinBox->setValue(5);
    m_timeoutSpinBox->setRange(5, 100);
    m_timeoutSpinBox->setSingleStep(5);
    m_timeoutSpinBox->setMinimumWidth(100);
    m_timeoutSpinBox->hide();
    m_timeoutSpinBox->setValue(m_settings->value(m_keyCtx.timeout, 10).toInt());
    m_timeout = m_timeoutSpinBox->value();
    hBoxLayout->addWidget(m_timeoutSpinBox);
    connect(m_timeoutSpinBox, &QSpinBox::editingFinished, this, [=]() {
        this->m_timeout = m_timeoutSpinBox->value();
        m_settings->setValue(this->m_keyCtx.timeout, m_timeoutSpinBox->value());
    });
}

void xToolsPingAssistant::initShowToolBar()
{
    QRadioButton *show_all = new QRadioButton(tr("Show all"));
    QRadioButton *show_online = new QRadioButton(tr("Show online"));

    QWidget *w = new QWidget();
    QHBoxLayout *h_box_layout = new QHBoxLayout(w);
    h_box_layout->setContentsMargins(0, 0, 0, 0);
    h_box_layout->addWidget(show_all);
    h_box_layout->addWidget(show_online);

    QButtonGroup *button_group = new QButtonGroup(this);
    button_group->addButton(show_all);
    button_group->addButton(show_online);
    m_toolBar->addWidget(w);

    int model = m_settings->value(m_keyCtx.showModel).toInt();
    m_showModel = model;
    if (model == ShowAll) {
        show_all->setChecked(true);
    } else if (model == ShowOnline) {
        show_online->setChecked(true);
    } else {
        m_showModel = ShowAll;
        show_all->setChecked(true);
    }

    connect(show_all, &QRadioButton::clicked, this, [=]() {
        this->m_showModel = ShowAll;
        m_settings->setValue(m_keyCtx.showModel, ShowAll);
        updateRowVisible();
    });
    connect(show_online, &QRadioButton::clicked, this, [=]() {
        this->m_showModel = ShowOnline;
        m_settings->setValue(m_keyCtx.showModel, ShowOnline);
        updateRowVisible();
    });
}

void xToolsPingAssistant::play()
{
    if (!isValidIp()) {
        return;
    }

    m_playAction->setEnabled(false);
    m_pauseAction->setEnabled(true);
    m_stopAction->setEnabled(true);

    if (m_pausing) {
        m_pausing = false;
        m_pauseAction->setEnabled(true);
    }

    m_preScanResult.clear();
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        QTableWidgetItem *item1 = ui->tableWidget->item(i, 0);
        QTableWidgetItem *item2 = ui->tableWidget->item(i, 1);
        if (item1 && item2) {
            QPair<QString, bool> p(item1->text(), item2->data(Qt::UserRole).toBool());
            m_preScanResult.append(p);
        }
    }

    m_finishedCount = 0;
    int count = ipCount();
    ui->tableWidget->setRowCount(count);
    updateProgressStatus();

    m_progressBar->setValue(0);
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(count);
    m_progressBar->show();

    int index = m_beginIp.lastIndexOf('.');
    QString beginIpLeft = m_beginIp.left(index + 1);
    QString beginIpRight = m_beginIp.right(m_beginIp.length() - index - 1);
    int beginIp = beginIpRight.toInt();

    for (int i = 0; i < count; i++) {
        QString ip = beginIpLeft + QString::number(beginIp + i);
        QTableWidgetItem *ip_item = new QTableWidgetItem(ip);
        ui->tableWidget->setItem(i, 0, ip_item);

        QTableWidgetItem *status_item = new QTableWidgetItem("--");
        status_item->setData(Qt::UserRole, false);
        ui->tableWidget->setItem(i, 1, status_item);

        QTableWidgetItem *compare_item = new QTableWidgetItem("--");
        ui->tableWidget->setItem(i, 2, compare_item);

        QTableWidgetItem *description_item = new QTableWidgetItem("--");
        ui->tableWidget->setItem(i, 3, description_item);
    }

    m_beginIpLineEdit->setEnabled(false);
    m_endIpLineEdit->setEnabled(false);
    m_timeoutSpinBox->setEnabled(false);

    updateRowVisible();
    m_currentRow = 0;
    m_playTimer->start();
}

void xToolsPingAssistant::pause()
{
    m_playAction->setEnabled(true);
    m_pauseAction->setEnabled(false);
    m_stopAction->setEnabled(true);

    m_pausing = true;
}

void xToolsPingAssistant::stop()
{
    m_playAction->setEnabled(true);
    m_pauseAction->setEnabled(false);
    m_stopAction->setEnabled(false);
    m_toolBar->setEnabled(true);

    m_beginIpLineEdit->setEnabled(true);
    m_endIpLineEdit->setEnabled(true);
    m_timeoutSpinBox->setEnabled(true);
    m_progressBar->hide();

    m_pausing = false;
    m_playTimer->stop();
    m_progressBar->hide();
}

bool xToolsPingAssistant::isValidIp()
{
    if (m_beginIp.isEmpty() || m_endIp.isEmpty()) {
        QMessageBox::warning(this,
                             tr("IP Error"),
                             tr("IP settings error, "
                                "the start ip and end ip must be set, "
                                "please check and try again. "));
        return false;
    }

    int beginIpIndex = m_beginIp.lastIndexOf('.');
    QString partOfBeginIp = m_beginIp.left(beginIpIndex);

    int end_ip_index = m_endIp.lastIndexOf('.');
    QString part_of_end_ip = m_endIp.left(end_ip_index);

    if (partOfBeginIp != part_of_end_ip) {
        QMessageBox::warning(this,
                             tr("IP Error"),
                             tr("IP settings error, "
                                "the start ip and end ip must be in the same network segment, "
                                "please check and try again. "));
        return false;
    }

    if (ipCount() < 1) {
        QMessageBox::warning(this,
                             tr("IP Error"),
                             tr("IP settings error, "
                                "the start ip must be less than the end ip, "
                                "please check and try again."));
        return false;
    }

    return true;
}

int xToolsPingAssistant::row(const QString &ip)
{
    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        QTableWidgetItem *item = ui->tableWidget->item(row, 0);
        if (item && item->text() == ip) {
            return row;
        }
    }

    return -1;
}

void xToolsPingAssistant::updateRowVisible()
{
    for (int row = 0; row < ui->tableWidget->rowCount(); row++) {
        QTableWidgetItem *item = ui->tableWidget->item(row, 1);
        if (item) {
            if (m_showModel == ShowAll) {
                ui->tableWidget->setRowHidden(row, false);
            } else {
                bool is_online = item->data(Qt::UserRole).toBool();
                ui->tableWidget->setRowHidden(row, !is_online);
            }
        } else {
            ui->tableWidget->setRowHidden(row, false);
        }
    }
}

void xToolsPingAssistant::updateProgressStatus()
{
    int all = ui->tableWidget->rowCount();
    int active = QThreadPool::globalInstance()->activeThreadCount();
    int finished = m_finishedCount;
    int remain = all - m_finishedCount - active;
    remain = remain > 0 ? remain : remain;

    QString txt = tr("Total：%1，Active：%2，Finished：%3，Remain：%4")
                      .arg(all)
                      .arg(active)
                      .arg(finished)
                      .arg(remain);
    m_progressStatus->setText(txt);
}

int xToolsPingAssistant::ipCount()
{
    int begin = m_beginIp.split('.').last().toInt();
    int end = m_endIp.split('.').last().toInt();

    return end - begin + 1;
}

QString xToolsPingAssistant::ip(int row)
{
    if (row >= 0 && row < ui->tableWidget->rowCount()) {
        QTableWidgetItem *item = ui->tableWidget->item(row, 0);
        if (item) {
            return item->text();
        }
    }

    return "";
}

void xToolsPingAssistant::onPlayTimerTimeout()
{
    if (m_pausing) {
        return;
    }

    int activeCount = QThreadPool::globalInstance()->activeThreadCount();
    int maxCount = QThreadPool::globalInstance()->maxThreadCount();
    if (activeCount < maxCount) {
        const QString ip = this->ip(m_currentRow);
        Q_ASSERT_X(!ip.isEmpty(), __FUNCTION__, "The ip is empty!");

        PingTask *pingTask = new PingTask(this, ip, m_timeout);
        QThreadPool::globalInstance()->start(pingTask);

        m_currentRow += 1;
    }

    if (m_currentRow == ui->tableWidget->rowCount()) {
    } else {
        m_playTimer->start();
    }

    updateProgressStatus();
}

void xToolsPingAssistant::onPingStarted(const QString &ip)
{
    int row = this->row(ip);
    QTableWidgetItem *item = ui->tableWidget->item(row, 1);
    if (item) {
        item->setText(tr("Scanning"));
        item->setBackground(QBrush(Qt::yellow));
    }

    updateProgressStatus();
}

void xToolsPingAssistant::onPingFinished(const QString &ip,
                                         bool isOnline,
                                         const QString &description)
{
    int row = this->row(ip);
    QTableWidgetItem *statusItem = ui->tableWidget->item(row, 1);
    if (statusItem) {
        statusItem->setText(isOnline ? tr("Online") : tr("Offline"));
        statusItem->setBackground(isOnline ? QBrush(Qt::green) : QBrush(Qt::red));
        statusItem->setData(Qt::UserRole, isOnline);
    }

    for (int i = 0; i < m_preScanResult.count(); i++) {
        QPair<QString, bool> p = m_preScanResult.at(i);
        if ((p.first == ip) && (p.second == false)) {
            if (isOnline) {
                QTableWidgetItem *compareItem = ui->tableWidget->item(row, 2);
                compareItem->setText("+");
                break;
            }
        }
        if ((p.first == ip) && (p.second == true)) {
            if (!isOnline) {
                QTableWidgetItem *compareItem = ui->tableWidget->item(row, 2);
                compareItem->setText("-");
                break;
            }
        }

        QTableWidgetItem *compareItem = ui->tableWidget->item(row, 2);
        compareItem->setText("=");
    }

    QTableWidgetItem *descriptionItem = ui->tableWidget->item(row, 3);
    if (descriptionItem) {
        descriptionItem->setText(description);
    }

    m_finishedCount += 1;
    m_progressBar->setValue(m_finishedCount);
    if (m_finishedCount == ui->tableWidget->rowCount()) {
        QMessageBox::information(
            this,
            tr("Scanning Finished"),
            tr("Scanning finished, you can screen out the result by the toolbar."));
        m_playAction->setEnabled(true);
        m_pauseAction->setEnabled(false);
        m_stopAction->setEnabled(false);
        m_toolBar->setEnabled(true);

        m_beginIpLineEdit->setEnabled(true);
        m_endIpLineEdit->setEnabled(true);
        m_timeoutSpinBox->setEnabled(true);
        m_progressBar->hide();
    }

    updateRowVisible();
    updateProgressStatus();
}
