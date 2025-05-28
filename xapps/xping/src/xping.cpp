/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xping.h"

#include <thread>

#include <QHostAddress>
#include <QPainter>
#include <QProcess>
#include <QQuickTextDocument>
#include <QStandardPaths>
#include <QTranslator>

#include "settings.h"

xPing::xPing(int &argc, char **argv)
    : QApplication(argc, argv)
{
    m_proxyModel = new SortFilterProxyModel(this);
}

xPing::~xPing() {}

Settings *xPing::settings()
{
    QStandardPaths::StandardLocation type = QStandardPaths::AppConfigLocation;
    QString path = QStandardPaths::writableLocation(type);
    QString fileName = QString("%1/%2.ini").arg(path, applicationName());
    static Settings settings(fileName, QSettings::IniFormat);

    return &settings;
}

QString xPing::settingsPath()
{
    Settings *settings = xPing::settings();
    QString path = settings->fileName();
    return path.left(path.lastIndexOf('/'));
}

void ping(xPing *app, const QString &ip)
{
    QProcess p;
    p.start("ping", QStringList() << "-a" << ip);
    bool ret = p.waitForFinished();
    if (!ret) {
        p.terminate();
    }

    QByteArray all = p.readAll();
    QString allString = QString::fromUtf8(all);
    QString description = ip;
    if (allString.contains("TTL=")) {
        QStringList list = allString.split(" ", Qt::SkipEmptyParts);
        if (list.length() >= 2) {
            description = list.at(2);
        }
        app->updateTableModel(ip, true, description);
    } else {
        app->updateTableModel(ip, false, description);
    }
}

void xPing::startPing(const QString &startIp, const QString &endIp)
{
    QString error = checkParameters(startIp, endIp);
    if (!error.isEmpty()) {
        qWarning() << error;
        return;
    }

    quint32 from = QHostAddress(startIp).toIPv4Address();
    quint32 to = QHostAddress(endIp).toIPv4Address();
    m_total = to - from + 1;

    if (m_oldTableModel) {
        m_oldTableModel->deleteLater();
        m_oldTableModel = nullptr;
    }

    if (m_tableModel) {
        m_oldTableModel = m_tableModel;
    }

    m_tableModel = new TableModel(from, to);
    m_proxyModel->setSourceModel(m_tableModel);
    m_proxyModel->setFilterKeyColumn(2);
    m_proxyModel->setFilterRegularExpression(m_filterText);

    for (int i = from; i <= to; ++i) {
        QHostAddress address(i);
        QString ip = address.toString();
        auto task = new std::thread(ping, this, ip);
        task->detach();
    }

    m_active = m_total;
    m_finished = 0;

    emit totalChanged();
    emit activeChanged();
    emit finishedChanged();
}

QString xPing::checkParameters(const QString &startIp, const QString &endIp)
{
    if (startIp.isEmpty() || endIp.isEmpty()) {
        return tr("Please input the start ip and end ip.");
    }

    const QString startIpError = tr("The start ip is invalid.");
    const QString endIpError = tr("The end ip is invalid.");

    if (startIp.split(".").length() != 4) {
        return startIpError;
    }

    if (endIp.split(".").length() != 4) {
        return endIpError;
    }

    QHostAddress startAddress(startIp);
    if (startAddress.isNull()) {
        return startIpError;
    }

    QHostAddress endAddress(endIp);
    if (endAddress.isNull()) {
        return endIpError;
    }

    if (startIp.left(startIp.lastIndexOf('.')) != endIp.left(endIp.lastIndexOf('.'))) {
        return tr("The start ip and end ip should be in the same network segment.");
    }

    if (QHostAddress(startIp).toIPv4Address() > QHostAddress(endIp).toIPv4Address()) {
        return tr("The start ip should be less than or equal to the end ip.");
    }

    return "";
}

void xPing::updateTableModel(const QString &ip, bool isOnline, const QString &description)
{
    if (!m_tableModel) {
        return;
    }

    m_updateTableModelMutex.lock();
    updateTableModelInner(ip, isOnline, description);

    m_active -= 1;
    emit activeChanged();

    m_finished += 1;
    emit finishedChanged();

    if (m_finished == m_total) {
        if (beepEnable()) {
            xPing::beep();
        }
        emit pingFinished();
    }

    m_updateTableModelMutex.unlock();
}

void xPing::setFilterText(const QString &text)
{
    m_filterText = text;
    m_proxyModel->setFilterKeyColumn(2);
    m_proxyModel->setFilterRegularExpression(m_filterText);
}

void xPing::setFilterTextDelta()
{
    m_proxyModel->setFilterKeyColumn(3);
    m_proxyModel->setFilterRegularExpression(QString("^[+-]$"));
}

QString xPing::version()
{
    return xPing::applicationVersion();
}

QString xPing::onlineText()
{
    return TableModel::cookedStatus(TableModel::RawStatusOnline);
}

QString xPing::offlineText()
{
    return TableModel::cookedStatus(TableModel::RawStatusOffline);
}

QString xPing::language()
{
    QSettings *settings = xPing::settings();
    QString language = settings->value(SettingsKey().language, "en").toString();
    return language;
}

void setupLanguage(const QString &qmFile)
{
    QTranslator *translator = new QTranslator();
    if (!translator->load(qmFile)) {
        auto info = QString("The language file(%1) can not be loaded, English will be used.")
                        .arg(qmFile);
        qWarning() << info;
        return;
    }

    if (!qApp->installTranslator(translator)) {
        qWarning() << "The language has been setup, English will be used.";
    } else {
        qInfo() << "The language has been setup, current language file is:" << qmFile;
    }
}

void xPing::setupLanguage(const QString &code)
{
    QSettings *settings = xPing::settings();
    QString defaultLanguage = QLocale::system().name();
    QString language = code;
    if (language.isEmpty()) {
        language = settings->value(SettingsKey().language, defaultLanguage).toString();
    }

    QString appPath = QApplication::applicationDirPath();
    QString qtQmFile = QString("%1/translations/qt_%2.qm").arg(appPath, language);
    ::setupLanguage(qtQmFile);

    QString xToolsQmFile = QString(":/res/translations/xPing_%1.qm").arg(language);
    ::setupLanguage(xToolsQmFile);
}

QSplashScreen *xPing::splashScreen()
{
    if (!qApp) {
        return Q_NULLPTR;
    }

    static QSplashScreen *splashScreen = Q_NULLPTR;
    if (!splashScreen) {
        QFont font = qApp->font();
        font.setPixelSize(52);

        QFontMetrics fontMetrics(font);
        const QString displayName = applicationName();
        int width = fontMetrics.boundingRect(displayName).width() * 1.2;

        QPixmap pixMap(width < 600 ? 600 : width, 260);
        pixMap.fill(QColor(0x1f1f1f));

        QPainter painter(&pixMap);
        painter.setPen(QColor(Qt::white));
        painter.setFont(font);
        painter.drawText(pixMap.rect(), Qt::AlignHCenter | Qt::AlignVCenter, displayName);
        painter.setPen(QColor(0x1f2c9f));
        painter.drawRect(pixMap.rect() - QMargins(1, 1, 1, 1));

        splashScreen = new QSplashScreen(pixMap);
        splashScreen->setDisabled(true);
    }

    return splashScreen;
}

void xPing::showSplashScreenMessage(const QString &msg)
{
    QSplashScreen *splashScreen = xPing::splashScreen();
    if (!splashScreen) {
        return;
    }

#if defined(QT_OS_ANDROID)
    splashScreen->showFullScreen();
#else
    splashScreen->show();
#endif
    splashScreen->showMessage(msg, Qt::AlignBottom | Qt::AlignLeft, Qt::white);
    QApplication::processEvents();
}

void xPing::updateTableModelInner(const QString &ip, bool isOnline, const QString &description)
{
    for (int i = 0; i < m_tableModel->rowCount(); i++) {
        auto numberIndex = m_tableModel->index(i, 0);
        auto ipIndex = m_tableModel->index(i, 1);
        auto statusIndex = m_tableModel->index(i, 2);
        auto compareIndex = m_tableModel->index(i, 3);
        auto descriptionIndex = m_tableModel->index(i, 4);
        if (!ipIndex.isValid() || !statusIndex.isValid() || !compareIndex.isValid()
            || !descriptionIndex.isValid()) {
            continue;
        }

        QString value = ipIndex.data().toString();
        if (value == ip) {
            int status = isOnline ? TableModel::RawStatusOnline : TableModel::RawStatusOffline;
            m_tableModel->setData(numberIndex, 0, Qt::EditRole);
            m_tableModel->setData(statusIndex, status, Qt::EditRole);
            m_tableModel->setData(descriptionIndex, description, Qt::DisplayRole);

            if (m_oldTableModel) {
                updateTableModelInnerCompare(ip, isOnline);
            }

            break;
        }
    }
}

void xPing::updateTableModelInnerCompare(const QString &ip, bool isOnline)
{
    for (int i = 0; i < m_oldTableModel->rowCount(); i++) {
        auto oldIpIndex = m_oldTableModel->index(i, 1);
        auto oldIp = oldIpIndex.data().toString();
        if (oldIp != ip) {
            continue;
        }

        auto statusIndex = m_tableModel->index(i, 2);
        auto oldStatusIndex = m_oldTableModel->index(i, 2);
        auto compareIndex = m_tableModel->index(i, 3);

#if 0
        if (i % 3 == 0) {
            m_tableModel->setData(compareIndex, "+", Qt::DisplayRole);
        } else if (i % 3 == 1) {
            m_tableModel->setData(compareIndex, "-", Qt::DisplayRole);
        } else {
            m_tableModel->setData(compareIndex, "=", Qt::DisplayRole);
        }

        return;
#endif

        if (oldStatusIndex.data() == statusIndex.data()) {
            int tmp = oldStatusIndex.data(Qt::EditRole).toInt();
            if (tmp == TableModel::RawStatusOnline || tmp == TableModel::RawStatusOffline) {
                m_tableModel->setData(compareIndex, TableModel::RawCompareEqual, Qt::EditRole);
            } else {
                m_tableModel->setData(compareIndex, TableModel::RawCompareUnknown, Qt::EditRole);
            }
        } else {
            if (oldStatusIndex.data(Qt::EditRole).toInt() == TableModel::RawStatusOnline) {
                m_tableModel->setData(compareIndex, TableModel::RawCompareSubtract, Qt::EditRole);
            } else if (oldStatusIndex.data(Qt::EditRole).toInt() == TableModel::RawStatusOffline) {
                m_tableModel->setData(compareIndex, TableModel::RawCompareAdd, Qt::EditRole);
            } else {
                m_tableModel->setData(compareIndex, TableModel::RawCompareUnknown, Qt::EditRole);
            }
        }
        break;
    }
}

bool xPing::beepEnable()
{
    //return xToolsSettings::instance()->value("beepEnable").toBool();
    return false;
}

void xPing::setBeepEnable(bool enable)
{
    //xToolsSettings::instance()->setValue("beepEnable", enable);
    emit beepEnableChanged();
}
