/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDPRESTORERTOOL_H
#define EDPRESTORERTOOL_H

#include <QMutex>
#include <QVariant>
#include <QAbstractTableModel>

#include "EDBaseTool.hpp"

class EDPrestorerTableModel : public QAbstractTableModel
{
    Q_OBJECT
    friend class EDPrestorerTool;
public:
    EDPrestorerTableModel(QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public:
    struct EDPrestoreItem {
        QString itemDescription{"Demo"};
        int itemTextFormat;
        int itemEscapeCharacter;
        int itemPrefix;
        QString itemText;
        int itemSuffix;

        bool itemCrcEnable;
        int itemCrcAlgorithm;
        int itemCrcStartIndex;
        int itemCrcEndIndex;
    };

private:
    QList<EDPrestoreItem> mItems;
    QMutex mItemsMutex;
    QStringList mHeaders;

private:
    QByteArray itemBytes(const EDPrestoreItem &item);

private:
    struct EDEmitterDataKeys {
        const QString itemDescription{"Description"};
        const QString itemTextFormat{"Format"};
        const QString itemEscapeCharacter{"Escape"};
        const QString itemPrefix{"Prefix"};
        const QString itemSuffix{"Suffix"};
        const QString itemCrcEnable{"CrcEnable"};
        const QString itemCrcAlgorithm{"Algorithm"};
        const QString itemCrcStartIndex{"Start"};
        const QString itemCrcEndIndex{"End"};
        const QString itemText{"Data"};
    } mDataKeys;
    const int mTableColumnCount{10};

private:
    QVariant columnDisplayRoleData(const EDPrestoreItem &item, int column) const;
};

class EDPrestorerTool : public EDBaseTool
{
    Q_OBJECT
    Q_PROPERTY(QVariant tableModel READ tableModel CONSTANT)
    Q_PROPERTY(QStringList headers READ headers CONSTANT)

    Q_PROPERTY(QString itemDescription READ itemDescription CONSTANT)
    Q_PROPERTY(QString itemTextFormat READ itemTextFormat CONSTANT)
    Q_PROPERTY(QString itemEscapeCharacter READ itemEscapeCharacter CONSTANT)
    Q_PROPERTY(QString itemPrefix READ itemPrefix CONSTANT)
    Q_PROPERTY(QString itemSuffix READ itemSuffix CONSTANT)
    Q_PROPERTY(QString itemCrcEnable READ itemCrcEnable CONSTANT)
    Q_PROPERTY(QString itemCrcAlgorithm READ itemCrcAlgorithm CONSTANT)
    Q_PROPERTY(QString itemCrcStartIndex READ itemCrcStartIndex CONSTANT)
    Q_PROPERTY(QString itemCrcEndIndex READ itemCrcEndIndex CONSTANT)
    Q_PROPERTY(QString itemText READ itemText CONSTANT)

    Q_PROPERTY(QStringList descriptions READ descriptions NOTIFY descriptionsChanged);
public:
    explicit EDPrestorerTool(QObject *parent = nullptr);

    Q_INVOKABLE void addItem(const QString &jsonCtx, int index = -1);
    Q_INVOKABLE QVariant itemContext(int index);
    Q_INVOKABLE QVariant itemsContext();
    Q_INVOKABLE void send(int index);

protected:
    virtual bool initialize(QString &errStr) final;
    virtual void outputBytesHandler() final;
    virtual void uninitialize() final;

private:
    QTimer *mEmittingTimer;
    const int mScanInterval{5};
    QList<int> mIndexs;
    QMutex mIndexsMutex;

private:
    EDPrestorerTableModel *mTableModel{nullptr};
    QVariant tableModel(){return QVariant::fromValue<EDPrestorerTableModel*>(mTableModel);}

    QStringList mHeaders;
    QStringList headers(){return mHeaders;}

    QString itemDescription(){return mTableModel->mDataKeys.itemDescription;}
    QString itemTextFormat(){return mTableModel->mDataKeys.itemTextFormat;}
    QString itemEscapeCharacter(){return mTableModel->mDataKeys.itemEscapeCharacter;}
    QString itemPrefix(){return mTableModel->mDataKeys.itemPrefix;}
    QString itemSuffix(){return mTableModel->mDataKeys.itemSuffix;}
    QString itemCrcEnable(){return mTableModel->mDataKeys.itemCrcEnable;}
    QString itemCrcAlgorithm(){return mTableModel->mDataKeys.itemCrcAlgorithm;}
    QString itemCrcStartIndex(){return mTableModel->mDataKeys.itemCrcStartIndex;}
    QString itemCrcEndIndex(){return mTableModel->mDataKeys.itemCrcEndIndex;}
    QString itemText(){return mTableModel->mDataKeys.itemText;}

    QStringList descriptions();
    Q_SIGNAL void descriptionsChanged();
};

#endif // EDPRESETDATATOOL_NEW_H
