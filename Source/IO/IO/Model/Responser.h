/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAbstractTableModel>
#include <QMutex>
#include <QVariant>

#include "AbstractModel.h"

#define SAK_STR_PROPERTY(name) Q_PROPERTY(QString name READ name CONSTANT)

namespace xTools {

class Responser : public AbstractModel
{
    Q_OBJECT

    Q_PROPERTY(QString itemEnable READ itemEnable CONSTANT)
    Q_PROPERTY(QString itemDescription READ itemDescription CONSTANT)
    Q_PROPERTY(QString itemOption READ itemOption CONSTANT)

    SAK_STR_PROPERTY(itemReferenceTextFormat)
    SAK_STR_PROPERTY(itemReferenceEscapeCharacter)
    SAK_STR_PROPERTY(itemReferencePrefix)
    SAK_STR_PROPERTY(itemReferenceSuffix)
    SAK_STR_PROPERTY(itemReferenceCrcEnable)
    SAK_STR_PROPERTY(itemReferenceCrcBigEndian)
    SAK_STR_PROPERTY(itemReferenceCrcAlgorithm)
    SAK_STR_PROPERTY(itemReferenceCrcStartIndex)
    SAK_STR_PROPERTY(itemReferenceCrcEndIndex)
    SAK_STR_PROPERTY(itemReferenceText)

    SAK_STR_PROPERTY(itemResponseTextFormat)
    SAK_STR_PROPERTY(itemResponseEscapeCharacter)
    SAK_STR_PROPERTY(itemResponsePrefix)
    SAK_STR_PROPERTY(itemResponseSuffix)
    SAK_STR_PROPERTY(itemResponseCrcEnable)
    SAK_STR_PROPERTY(itemResponseCrcBigEndian)
    SAK_STR_PROPERTY(itemResponseCrcAlgorithm)
    SAK_STR_PROPERTY(itemResponseCrcStartIndex)
    SAK_STR_PROPERTY(itemResponseCrcEndIndex)
    SAK_STR_PROPERTY(itemResponseDelay)
    SAK_STR_PROPERTY(itemResponseText)

public:
    struct ResponserItem
    {
        bool itemEnable;
        QString itemDescription{"Demo"};
        int itemOption;

        int itemReferenceTextFormat;
        int itemReferenceEscapeCharacter;
        int itemReferencePrefix;
        QString itemReferenceText;
        int itemReferenceSuffix;
        bool itemReferenceCrcEnable;
        bool itemReferenceCrcBigEndian;
        int itemReferenceCrcAlgorithm;
        int itemReferenceCrcStartIndex;
        int itemReferenceCrcEndIndex;

        int itemResponseTextFormat;
        int itemResponseEscapeCharacter;
        int itemResponsePrefix;
        QString itemResponseText;
        int itemResponseSuffix;
        bool itemResponseCrcEnable;
        bool itemResponseCrcBigEndian;
        int itemResponseCrcAlgorithm;
        int itemResponseCrcStartIndex;
        int itemResponseCrcEndIndex;
        int itemResponseDelay;
    };

    struct ResponserData
    {
        ResponserItem data;
        int elapsedTime{0};
    };

    struct ResponserItemKeys
    {
        const QString itemEnable{"Enable"};
        const QString itemDescription{"Description"};
        const QString itemOption{"Option"};
        const QString itemReferenceTextFormat{"RefFormat"};
        const QString itemReferenceEscapeCharacter{"RefEscape"};
        const QString itemReferencePrefix{"RefPrefix"};
        const QString itemReferenceSuffix{"RefSuffix"};
        const QString itemReferenceCrcEnable{"RefCrcEnable"};
        const QString itemReferenceCrcBigEndian{"RefCrcBigEndian"};
        const QString itemReferenceCrcAlgorithm{"RefAlgorithm"};
        const QString itemReferenceCrcStartIndex{"RefStart"};
        const QString itemReferenceCrcEndIndex{"RefEnd"};
        const QString itemReferenceText{"ReferenceData"};
        const QString itemResponseTextFormat{"ResFormat"};
        const QString itemResponseEscapeCharacter{"Escape"};
        const QString itemResponsePrefix{"ResPrefix"};
        const QString itemResponseSuffix{"ResSuffix"};
        const QString itemResponseCrcEnable{"ResCrcEnable"};
        const QString itemResponseCrcBigEndian{"ResCrcBigEndian"};
        const QString itemResponseCrcAlgorithm{"ResAlgorithm"};
        const QString itemResponseCrcStartIndex{"ResStart"};
        const QString itemResponseCrcEndIndex{"ResEnd"};
        const QString itemResponseDelay{"ResDelay"};
        const QString itemResponseText{"ResponseData"};
    };

public:
    explicit Responser(QObject *parent = nullptr);

    void inputBytes(const QByteArray &bytes) override;

protected:
    void run() override;

    // clang-format off
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    // clang-format on

private:
    QVector<QByteArray> m_inputBytesList;
    QMutex m_inputBytesListMutex;

    QVector<ResponserData> m_iItems;
    QMutex m_itemsMutex;
    const int m_descriptionColumnIndex{0};
    const int m_formatColumnIndex{1};
    const int m_itemTextColumnIndex{2};
    struct ResponserItemKeys m_dataKeys;
    const int m_tableColumnCount{24};

private:
    QVariant columnDisplayRoleData(const ResponserData &item, int column) const;
    QByteArray referenceBytes(const ResponserItem &item) const;
    QByteArray responseBytes(const ResponserItem &item) const;
    void try2output(const QByteArray &bytes, QObject *receiver);

private:
    QString itemEnable();
    QString itemDescription();
    QString itemOption();

    QString itemReferenceTextFormat();
    QString itemReferenceEscapeCharacter();
    QString itemReferencePrefix();
    QString itemReferenceSuffix();
    QString itemReferenceCrcEnable();
    QString itemReferenceCrcBigEndian();
    QString itemReferenceCrcAlgorithm();
    QString itemReferenceCrcStartIndex();
    QString itemReferenceCrcEndIndex();
    QString itemReferenceText();

    QString itemResponseTextFormat();
    QString itemResponseEscapeCharacter();
    QString itemResponsePrefix();
    QString itemResponseSuffix();
    QString itemResponseCrcEnable();
    QString itemResponseCrcBigEndian();
    QString itemResponseCrcAlgorithm();
    QString itemResponseCrcStartIndex();
    QString itemResponseCrcEndIndex();
    QString itemResponseDelay();
    QString itemResponseText();
};

} // namespace xTools
