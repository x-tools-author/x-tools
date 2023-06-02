/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QFile>
#include <QDebug>
#include <QLocale>
#include <QDateTime>
#include <QClipboard>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextDocument>
#include <QGuiApplication>
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
#include <QQuickTextDocument>
#endif
#include <QAbstractTableModel>

#include "SAKInterface.hh"
#include "SAKDataStructure.hh"

SAKInterface::SAKInterface(QObject *parent)
    : QObject{parent}
{

}

QString SAKInterface::array2String(const QByteArray &array, int format)
{
    auto cookedArray =
            [](const QByteArray &array, int base, int len)->QString{
        QString str, numStr;
        for (int i = 0; i < array.length(); i++) {
            if (base == 10 || base == 8) {
                numStr = QString::number(array.at(i), base);
                str.append(QString("%1 ").arg(numStr));
            } else {
                numStr = QString::number(quint8(array.at(i)), base);
                str.append(QString("%1 ").arg(numStr, len, '0'));
            }
        }
        return str;
    };
    
    if (SAKDataStructure::TextFormatBin == format) {
        return cookedArray(array, 2, 8);
    } else if (SAKDataStructure::TextFormatOct == format) {
        return cookedArray(array, 8, 3);
    } else if (SAKDataStructure::TextFormatDec == format) {
        return cookedArray(array, 10, 3);
    } else if (SAKDataStructure::TextFormatHex == format) {
        return cookedArray(array, 16, 2);
    } else if (SAKDataStructure::TextFormatAscii == format) {
        return QString::fromLatin1(array);
    } else {
        return QString::fromUtf8(array);
    }
}

QByteArray SAKInterface::string2array(const QString &input, int format)
{
    QByteArray data;
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    auto behavior = QString::SkipEmptyParts;
#else
    auto behavior = Qt::SkipEmptyParts;
#endif
    
    if (format == SAKDataStructure::TextFormatBin){
        QStringList strList = input.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 2);
            data.append(reinterpret_cast<char*>(&value), 1);
        }
    } else if (format == SAKDataStructure::TextFormatOct){
        QStringList strList = input.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 8);
            data.append(reinterpret_cast<char*>(&value), 1);
        }
    } else if (format == SAKDataStructure::TextFormatDec){
        QStringList strList = input.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 10);
            data.append(reinterpret_cast<char*>(&value), 1);
        }
    } else if (format == SAKDataStructure::TextFormatHex){
        QStringList strList = input.split(' ', behavior);
        for (int i = 0; i < strList.length(); i++) {
            QString str = strList.at(i);
            qint8 value = QString(str).toInt(Q_NULLPTR, 16);
            data.append(reinterpret_cast<char*>(&value), 1);
        }
    } else if (format == SAKDataStructure::TextFormatAscii){
        data = input.toLatin1();
    } else {
        data = input.toUtf8();
    }

    return data;
}

void SAKInterface::setMaximumBlockCount(QVariant quickTextDocument, int maximum)
{
    auto obj = quickTextDocument.value<QObject*>();
    if (obj) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
        auto quickTextDoc = qobject_cast<QQuickTextDocument*>(obj);
        if (quickTextDoc) {
            auto textDoc = quickTextDoc->textDocument();
            textDoc->setMaximumBlockCount(maximum);
        }
#else
        Q_UNUSED(quickTextDocument)
        Q_UNUSED(maximum)
#endif
    }
}

QString SAKInterface::dateTimeString(const QString &format)
{
    return QDateTime::currentDateTime().toString(format);
}

QByteArray SAKInterface::arrayAppendArray(const QByteArray &a1,
                                         const QByteArray &a2)
{
    return a1 + a2;
}

void SAKInterface::tableModel2jsonFile(QVariant tableModel,
                                  const QString &fileName)
{
    QFile file(cookedFileName(fileName));
    if (!file.open(QFile::WriteOnly)) {
        qWarning() << file.errorString();
        return;
    }

    if (!tableModel.canConvert<QAbstractTableModel*>()) {
        qWarning() << "Unvalid parameter(tableModel)";
        return;
    }

    QJsonArray items;
    QJsonObject item;
    auto cookedTableModel = tableModel.value<QAbstractTableModel *>();
    int rowCount = cookedTableModel->rowCount();
    int columnCount = cookedTableModel->columnCount();
    for (int row = 0; row < rowCount; row++) {
        for (int column = 0; column < columnCount; column++) {
            auto index = cookedTableModel->index(row, column);
            auto value = cookedTableModel->data(index, Qt::DisplayRole);
            auto key = cookedTableModel->headerData(column, Qt::Horizontal);
            item.insert(key.toString(), value.toJsonValue());
        }
        items.append(item);
    }

    QJsonDocument jsonDoc(items);
    QByteArray data = jsonDoc.toJson(QJsonDocument::Indented);
    QTextStream ts(&file);
    ts << data;

    file.close();
}

void SAKInterface::jsonFile2tableModel(QVariant tableModel,
                                  const QString &fileName)
{
    QFile file(cookedFileName(fileName));
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << file.errorString();
        return;
    }

    if (!tableModel.canConvert<QAbstractTableModel*>()) {
        qWarning() << "Unvalid parameter(tableModel)";
        return;
    }

    QByteArray json = file.readAll();
    file.close();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json);
    QJsonArray items = jsonDoc.array();
    jsonArray2TableModel(tableModel, items);
}

void SAKInterface::jsonArray2TableModel(QVariant tableModel,
                                       const QJsonArray &array)
{
    QJsonArray items = array;
    QJsonObject item;
    auto cookedTableModel = tableModel.value<QAbstractTableModel *>();
    int rowCount = cookedTableModel->rowCount();
    int columnCount = cookedTableModel->columnCount();
    if (rowCount > 0) {
        cookedTableModel->removeRows(0, rowCount);
    }

    cookedTableModel->insertRows(0, items.count());
    for (int row = 0; row < items.count(); row++) {
        item = items.at(row).toObject();
        for (int column = 0; column < columnCount; column++) {
            auto index = cookedTableModel->index(row, column);
            auto key = cookedTableModel->headerData(column, Qt::Horizontal);
            auto value = item.value(key.toString());
            if (!cookedTableModel->setData(index, value, Qt::EditRole)) {
                qWarning() << "Can not set data.";
            }
        }
    }
}

QString SAKInterface::cookedFileName(const QString &fileName)
{
    QString cookedFileName = fileName;
#ifdef Q_OS_WIN
    cookedFileName = cookedFileName.remove("file:///");
#endif

    return cookedFileName;
}

void SAKInterface::clearTableModel(QVariant tableModel)
{
    if (!tableModel.canConvert<QAbstractTableModel*>()) {
        qWarning() << "Unvalid parameter(tableModel)";
        return;
    }

    auto cookedTableModel = tableModel.value<QAbstractTableModel *>();
    int rowCount = cookedTableModel->rowCount();
    if (rowCount > 0) {
        cookedTableModel->removeRows(0, rowCount);
    }
}

void SAKInterface::removeTableModelRow(QVariant tableModel, int row)
{
    if (!tableModel.canConvert<QAbstractTableModel*>()) {
        qWarning() << "Unvalid parameter(tableModel)";
        return;
    }

    auto cookedTableModel = tableModel.value<QAbstractTableModel *>();
    int rowCount = cookedTableModel->rowCount();
    if (rowCount > 0) {
        cookedTableModel->removeRows(row, 1);
    }
}

QString SAKInterface::string2hexString(const QString &str)
{
    return QString::fromLatin1(str.toLocal8Bit().toHex());
}

QString SAKInterface::hexString2String(const QString &str)
{
    QByteArray arr = QByteArray::fromHex(str.toLatin1());
    return QString::fromUtf8(arr);
}

void SAKInterface::setAppFont(const QString &fontFamily)
{
    qGuiApp->setFont(fontFamily);
}

QString SAKInterface::buildDateTime(const QString &format)
{
    QDate date = QDate::fromString(__DATE__, "MMM dd yyyy");
    QTime time = QTime::fromString(__TIME__, "hh:mm:ss");
    return QDateTime(date, time).toString(format);
}

QString SAKInterface::dateFormat()
{
    return QLocale::system().dateFormat();
}

QString SAKInterface::timeFormat()
{
    return QLocale::system().timeFormat();
}

void SAKInterface::setTextToClipboard(const QString &text)
{
    QGuiApplication::clipboard()->setText(text);
}
