/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKRESPONSERTOOL_HH
#define SAKRESPONSERTOOL_HH

#include <QMutex>
#include <QVariant>
#include <QAbstractTableModel>

#include "SAKTabelModelTool.hh"

class SAKResponserTool : public SAKTabelModelTool
{
    Q_OBJECT
    Q_PROPERTY(QVariant tableModel READ tableModel CONSTANT)
    Q_PROPERTY(QStringList headers READ headers CONSTANT)

    Q_PROPERTY(QString itemEnable READ itemEnable CONSTANT)
    Q_PROPERTY(QString itemDescription READ itemDescription CONSTANT)
    Q_PROPERTY(QString itemOption READ itemOption CONSTANT)

    Q_PROPERTY(QString itemReferenceTextFormat READ itemReferenceTextFormat CONSTANT)
    Q_PROPERTY(QString itemReferenceEscapeCharacter READ itemReferenceEscapeCharacter CONSTANT)
    Q_PROPERTY(QString itemReferencePrefix READ itemReferencePrefix CONSTANT)
    Q_PROPERTY(QString itemReferenceSuffix READ itemReferenceSuffix CONSTANT)
    Q_PROPERTY(QString itemReferenceCrcEnable READ itemReferenceCrcEnable CONSTANT)
    Q_PROPERTY(QString itemReferenceCrcAlgorithm READ itemReferenceCrcAlgorithm CONSTANT)
    Q_PROPERTY(QString itemReferenceCrcStartIndex READ itemReferenceCrcStartIndex CONSTANT)
    Q_PROPERTY(QString itemReferenceCrcEndIndex READ itemReferenceCrcEndIndex CONSTANT)
    Q_PROPERTY(QString itemReferenceText READ itemReferenceText CONSTANT)

    Q_PROPERTY(QString itemResponseTextFormat READ itemResponseTextFormat CONSTANT)
    Q_PROPERTY(QString itemResponseEscapeCharacter READ itemResponseEscapeCharacter CONSTANT)
    Q_PROPERTY(QString itemResponseInterval READ itemResponseInterval CONSTANT)
    Q_PROPERTY(QString itemResponsePrefix READ itemResponsePrefix CONSTANT)
    Q_PROPERTY(QString itemResponseSuffix READ itemResponseSuffix CONSTANT)
    Q_PROPERTY(QString itemResponseCrcEnable READ itemResponseCrcEnable CONSTANT)
    Q_PROPERTY(QString itemResponseCrcAlgorithm READ itemResponseCrcAlgorithm CONSTANT)
    Q_PROPERTY(QString itemResponseCrcStartIndex READ itemResponseCrcStartIndex CONSTANT)
    Q_PROPERTY(QString itemResponseCrcEndIndex READ itemResponseCrcEndIndex CONSTANT)
    Q_PROPERTY(QString itemResponseText READ itemResponseText CONSTANT)
public:
    struct ResponserItem {
        bool itemEnable;
        QString itemDescription{"Demo"};
        int itemOption;

        int itemReferenceTextFormat;
        int itemReferenceEscapeCharacter;
        int itemReferencePrefix;
        QString itemReferenceText;
        int itemReferenceSuffix;
        bool itemReferenceCrcEnable;
        int itemReferenceCrcAlgorithm;
        int itemReferenceCrcStartIndex;
        int itemReferenceCrcEndIndex;

        int itemResponseTextFormat;
        int itemResponseEscapeCharacter;
        int itemResponsePrefix;
        QString itemResponseText;
        int itemResponseSuffix;
        int itemResponseInterval;
        bool itemResponseCrcEnable;
        int itemResponseCrcAlgorithm;
        int itemResponseCrcStartIndex;
        int itemResponseCrcEndIndex;
    };

    struct ResponserData {
        ResponserItem data;
        int elapsedTime{0};
    };

    struct ResponserItemKeys {
        const QString itemEnable{"Enable"};
        const QString itemDescription{"Description"};
        const QString itemOption{"Option"};
        const QString itemReferenceTextFormat{"RefFormat"};
        const QString itemReferenceEscapeCharacter{"RefEscape"};
        const QString itemReferencePrefix{"RefPrefix"};
        const QString itemReferenceSuffix{"RefSuffix"};
        const QString itemReferenceCrcEnable{"RefCrcEnable"};
        const QString itemReferenceCrcAlgorithm{"RefAlgorithm"};
        const QString itemReferenceCrcStartIndex{"RefStart"};
        const QString itemReferenceCrcEndIndex{"RefEnd"};
        const QString itemReferenceText{"ReferenceData"};
        const QString itemResponseTextFormat{"ResFormat"};
        const QString itemResponseEscapeCharacter{"Escape"};
        const QString itemResponsePrefix{"ResPrefix"};
        const QString itemResponseSuffix{"ResSuffix"};
        const QString itemResponseInterval{"ResInterval"};
        const QString itemResponseCrcEnable{"ResCrcEnable"};
        const QString itemResponseCrcAlgorithm{"ResAlgorithm"};
        const QString itemResponseCrcStartIndex{"ResStart"};
        const QString itemResponseCrcEndIndex{"ResEnd"};
        const QString itemResponseText{"ResponseData"};
    };

public:
    explicit SAKResponserTool(QObject *parent = nullptr);
    Q_INVOKABLE virtual QVariant itemContext(int index) final;

    virtual void inputBytes(const QByteArray &bytes,
                            const QVariant &context = QJsonObject()) final;

protected:
    virtual void run() final;

    virtual int rowCount(const QModelIndex &parent
                         = QModelIndex()) const final;
    virtual int columnCount(const QModelIndex &parent
                            = QModelIndex()) const final;
    virtual QVariant data(const QModelIndex &index,
                          int role = Qt::DisplayRole) const final;
    virtual bool setData(const QModelIndex &index,
                         const QVariant &value,
                         int role = Qt::EditRole) final;
    virtual bool insertRows(int row, int count,
                            const QModelIndex &parent = QModelIndex()) final;
    virtual bool removeRows(int row, int count,
                            const QModelIndex &parent = QModelIndex()) final;
    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const final;

private:
    struct InputContext {
        QByteArray bytes;
        QVariant context;
    };

private:
    QList<InputContext> mInputContextList;
    QMutex mInputContextListMutex;

    QList<ResponserData> mItems;
    QMutex mItemsMutex;
    const int mDescriptionColumnIndex{0};
    const int mFormatColumnIndex{1};
    const int mItemTextColumnIndex{2};
    struct ResponserItemKeys mDataKeys;
    const int mTableColumnCount{22};

private:
    QVariant columnDisplayRoleData(const ResponserData &item, int column) const;
    QByteArray referenceBytes(const ResponserItem &item) const;
    QByteArray responseBytes(const ResponserItem &item) const;
    void try2output(const QByteArray &bytes, QObject *threadInnerObject);

private:
    QString itemEnable();
    QString itemDescription();
    QString itemOption();

    QString itemReferenceTextFormat();
    QString itemReferenceEscapeCharacter();
    QString itemReferencePrefix();
    QString itemReferenceSuffix();
    QString itemReferenceCrcEnable();
    QString itemReferenceCrcAlgorithm();
    QString itemReferenceCrcStartIndex();
    QString itemReferenceCrcEndIndex();
    QString itemReferenceText();

    QString itemResponseTextFormat();
    QString itemResponseEscapeCharacter();
    QString itemResponseInterval();
    QString itemResponsePrefix();
    QString itemResponseSuffix();
    QString itemResponseCrcEnable();
    QString itemResponseCrcAlgorithm();
    QString itemResponseCrcStartIndex();
    QString itemResponseCrcEndIndex();
    QString itemResponseText();
};

#endif // SAKRESPONSERTOOL_H
