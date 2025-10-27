/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>
#include <QJsonObject>
#include <QString>

class TsDictionary : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TsDictionary(QObject *parent = nullptr);
    ~TsDictionary() override;

    void addItem(const QString &sourceText);
    void removeItem(int row);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QStringList languageFlags() const;
    QStringList compatibleLanguageFlags(const QString &flag) const;
    QJsonObject save() const;
    void load(const QJsonObject &obj);

private:
    struct TsDictionaryItem
    {
        QString sourceText;
        QList<QPair<QString, QString>> translations; // <language flag, translated text>
    };
    QList<TsDictionaryItem> m_items;
    QStringList m_flags;
};