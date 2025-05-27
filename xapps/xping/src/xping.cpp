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
#include <QProcess>
#include <QQuickTextDocument>

#include "common/xtools.h"

xPing::xPing(int argc, char **argv)
    : Application(argc, argv)
{}

xPing::~xPing() {}

QString xPing::version()
{
    return xPing::applicationVersion();
}

QString xPing::bytes2string(const QByteArray &bytes, int format)
{
    return ::bytes2string(bytes, format);
}

QByteArray xPing::string2bytes(const QString &txt, int format)
{
    return ::string2bytes(txt, format);
}

QByteArray xPing::arrayAppendArray(const QByteArray &array1, const QByteArray &array2)
{
    return ::arrayAppendArray(array1, array2);
}

QString xPing::cookedEscapeCharacter(const QString &txt, int esc)
{
    return ::cookedEscapeCharacter(txt, esc);
}

QByteArray xPing::cookedAffixes(int affixes)
{
    return ::cookedAffixes(affixes);
}

void xPing::setQuickTextDocumentMaximumBlockCount(QVariant textDocument, int count)
{
    if (textDocument.canConvert<QQuickTextDocument *>()) {
        QQuickTextDocument *doc = textDocument.value<QQuickTextDocument *>();
        if (doc) {
            QTextDocument *textDoc = doc->textDocument();
            if (textDoc) {
                textDoc->setMaximumBlockCount(count);
            }
        }
    }
}

QString xPing::dateTimeString(const QString &format)
{
    return ::dateTimeString(format);
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
        QStringList list = allString.split(" ", xSkipEmptyParts);
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

QStringList xPing::supportedLanguages2()
{
    return QStringList();
}

QString xPing::language2()
{
    return QString();
}

QString xPing::onlineText()
{
    return TableModel::cookedStatus(TableModel::RawStatusOnline);
}

QString xPing::offlineText()
{
    return TableModel::cookedStatus(TableModel::RawStatusOffline);
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
