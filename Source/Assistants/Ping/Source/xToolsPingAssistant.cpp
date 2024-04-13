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
    PingTask(xToolsPingAssistant *main_window, const QString &ip)
    {
        main_window_ = main_window;
        ip_ = ip;
    }

    void run() override
    {
        QProcess p;
        p.start("ping", QStringList() << "-a" << ip_);
        main_window_->emitPingStarted(ip_);
        bool ret = p.waitForFinished();
        if (!ret) {
            p.terminate();
        }

        QByteArray all = p.readAll();
        QString all_string = QString::fromUtf8(all);
        QString description = ip_;
        if (all_string.contains("TTL=")) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QStringList list = all_string.split(" ", Qt::SkipEmptyParts);
#else
            QStringList list = all_string.split(" ", QString::SkipEmptyParts);
#endif
            if (list.length() >= 2) {
                description = list.at(2);
            }
            main_window_->emitPingFinished(ip_, true, description);
        } else {
            main_window_->emitPingFinished(ip_, false, description);
        }
    }

private:
    xToolsPingAssistant *main_window_;
    QString ip_;
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

    m_progressStatus = new QLabel(this);
    QString txt = tr("Total: --, Active: --, Finished: --, Remain: --");
    m_progressStatus->setText(txt);
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
    QHBoxLayout *h_box_layout = new QHBoxLayout(w);
    h_box_layout->setContentsMargins(0, 0, 0, 0);
    m_toolBar->addWidget(w);

    h_box_layout->addWidget(new QLabel(tr("Start address")));
    QLineEdit *begin_ip = new QLineEdit(this);
    begin_ip->setText(m_settings->value(m_keyCtx.beginIp).toString());
    m_beginIp = begin_ip->text().trimmed();
    h_box_layout->addWidget(begin_ip);
    connect(begin_ip, &QLineEdit::textChanged, this, [=](const QString &txt) {
        this->m_beginIp = txt;
        m_settings->setValue(this->m_keyCtx.beginIp, txt);
    });

    h_box_layout->addWidget(new QLabel(tr("End address")));
    QLineEdit *end_ip = new QLineEdit(this);
    end_ip->setText(m_settings->value(m_keyCtx.endIp).toString());
    m_endIp = end_ip->text().trimmed();
    h_box_layout->addWidget(end_ip);
    connect(end_ip, &QLineEdit::textChanged, this, [=](const QString &txt) {
        this->m_endIp = txt;
        m_settings->setValue(this->m_keyCtx.endIp, txt);
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

    int index = m_beginIp.lastIndexOf('.');
    QString begin_ip_left = m_beginIp.left(index + 1);
    QString begin_ip_right = m_beginIp.right(m_beginIp.length() - index - 1);
    int begin_ip = begin_ip_right.toInt();

    for (int i = 0; i < count; i++) {
        QString ip = begin_ip_left + QString::number(begin_ip + i);
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

    m_toolBar->setEnabled(false);

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

    m_pausing = false;
    m_playTimer->stop();
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

    int begin_ip_index = m_beginIp.lastIndexOf('.');
    QString part_of_begin_ip = m_beginIp.left(begin_ip_index);

    int end_ip_index = m_endIp.lastIndexOf('.');
    QString part_of_end_ip = m_endIp.left(end_ip_index);

    if (part_of_begin_ip != part_of_end_ip) {
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

    int active_count = QThreadPool::globalInstance()->activeThreadCount();
    int max_count = QThreadPool::globalInstance()->maxThreadCount();
    if (active_count < max_count) {
        const QString ip = this->ip(m_currentRow);
        if (ip.isEmpty()) {
            qDebug() << __FUNCTION__;
        } else {
            PingTask *ping_task = new PingTask(this, ip);
            QThreadPool::globalInstance()->start(ping_task);
        }

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

void xToolsPingAssistant::onPingFinished(const QString &ip, bool is_online, const QString &description)
{
    int row = this->row(ip);
    QTableWidgetItem *status_item = ui->tableWidget->item(row, 1);
    if (status_item) {
        status_item->setText(is_online ? tr("Online") : tr("Offline"));
        status_item->setBackground(is_online ? QBrush(Qt::green) : QBrush(Qt::red));
        status_item->setData(Qt::UserRole, is_online);
    }

    for (int i = 0; i < m_preScanResult.count(); i++) {
        QPair<QString, bool> p = m_preScanResult.at(i);
        if ((p.first == ip) && (p.second == false)) {
            if (is_online) {
                QTableWidgetItem *compare_item = ui->tableWidget->item(row, 2);
                compare_item->setText("+");
                break;
            }
        }
    }

    QTableWidgetItem *description_item = ui->tableWidget->item(row, 3);
    if (description_item) {
        description_item->setText(description);
    }

    m_finishedCount += 1;
    if (m_finishedCount == ui->tableWidget->rowCount()) {
        QMessageBox::information(
            this,
            tr("Scanning Finished"),
            tr("Scanning finished, you can screen out the result by the toolbar."));
        m_playAction->setEnabled(true);
        m_pauseAction->setEnabled(false);
        m_stopAction->setEnabled(false);
        m_toolBar->setEnabled(true);
    }

    updateRowVisible();
    updateProgressStatus();
}
