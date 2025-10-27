/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "tsdictionary.h"

#include <QLocale>
#include <QMetaEnum>

TsDictionary::TsDictionary(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_flags = {"en", "zh_CN", "zh_TW", "ar", "cs", "da", "de", "es", "fa", "fi", "fr", "he",
               "uk", "it",    "ja",    "ko", "lt", "nl", "pl", "pt", "ru", "sk", "sl", "sv"};
    std::sort(m_flags.begin(), m_flags.end());
}

TsDictionary::~TsDictionary() {}

void TsDictionary::addItem(const QString &sourceText)
{
    TsDictionaryItem item;
    item.sourceText = sourceText;
    for (int i = 0; i < m_flags.count(); ++i) {
        item.translations.append(qMakePair(m_flags.at(i), QString()));
    }
    m_items.append(item);

    beginInsertRows(QModelIndex(), m_items.count() - 1, m_items.count() - 1);
    endInsertRows();

    int row = m_items.count() - 1;
    emit dataChanged(index(row, 0), index(row, m_flags.count()));
}

void TsDictionary::removeItem(int row)
{
    if (row < 0 || row >= m_items.count()) {
        return;
    }

    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
}

int TsDictionary::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

int TsDictionary::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_flags.count() + 1;
}

QVariant TsDictionary::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int column = index.column();
    int flagIndex = column - 1;
    if (row < 0 || row >= m_items.count()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        if (column == 0) {
            return m_items.at(row).sourceText;
        }

        if (flagIndex >= 0 && flagIndex < m_items.at(row).translations.count()) {
            QString flag = m_flags.at(flagIndex);
            const QList<QPair<QString, QString>> &translations = m_items.at(row).translations;
            for (const QPair<QString, QString> &pair : translations) {
                if (pair.first == flag) {
                    return pair.second;
                }
            }
        }
    }

    return QVariant();
}

bool TsDictionary::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    int column = index.column();
    int flagIndex = column - 1;
    if (row < 0 || row >= m_items.count()) {
        return false;
    }

    if (flagIndex < 0 || flagIndex >= m_flags.count()) {
        return false;
    }

    if (column == 0) {
        m_items[row].sourceText = value.toString();
        emit dataChanged(index, index);
    } else {
        TsDictionaryItem &item = m_items[row];
        QString flag = m_flags.at(flagIndex);
        for (int i = 0; i < item.translations.count(); ++i) {
            if (item.translations[i].first == flag) {
                item.translations[i].second = value.toString();
                emit dataChanged(index, index);
                return true;
            }
        }
    }

    return false;
}

QVariant TsDictionary::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (section == 0) {
            if (role == Qt::DisplayRole) {
                return tr("Source Text");
            }
        } else {
            if (role == Qt::DisplayRole) {
                int index = section - 1;
                if (index >= 0 && index < m_flags.count()) {
                    QString flag = m_flags.at(index);
                    QLocale locale(flag);
                    QString name = locale.nativeLanguageName();
                    QString flags = compatibleLanguageFlags(flag).join(", ");
                    flags = flags.trimmed();
                    QString str = QString("%1\n%2").arg(name, flags);
                    return str;
                }
            }
        }
    } else if (orientation == Qt::Vertical) {
        if (role == Qt::DisplayRole) {
            return section + 1;
        }
    }

    return QVariant();
}

Qt::ItemFlags TsDictionary::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    auto flags = QAbstractTableModel::flags(index);
    flags |= Qt::ItemIsEditable;
    return flags;
}

QStringList TsDictionary::languageFlags() const
{
    return m_flags;
}

QStringList TsDictionary::compatibleLanguageFlags(const QString &flag) const
{
    QLocale locale(flag);
    return locale.uiLanguages();
}

QJsonObject TsDictionary::save() const
{
    return QJsonObject();
}

void TsDictionary::load(const QJsonObject &obj)
{
    Q_UNUSED(obj);
}