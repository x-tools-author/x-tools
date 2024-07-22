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
#include <QTimer>
#include <QVariant>

#include "xToolsTableModelTool.h"

class xToolsEmitterTool : public xToolsTableModelTool
{
    Q_OBJECT
    Q_PROPERTY(QString itemEnable READ itemEnable CONSTANT)
    Q_PROPERTY(QString itemDescription READ itemDescription CONSTANT)
    Q_PROPERTY(QString itemTextFormat READ itemTextFormat CONSTANT)
    Q_PROPERTY(QString itemEscapeCharacter READ itemEscapeCharacter CONSTANT)
    Q_PROPERTY(QString itemInterval READ itemInterval CONSTANT)
    Q_PROPERTY(QString itemPrefix READ itemPrefix CONSTANT)
    Q_PROPERTY(QString itemSuffix READ itemSuffix CONSTANT)
    Q_PROPERTY(QString itemCrcEnable READ itemCrcEnable CONSTANT)
    Q_PROPERTY(QString itemCrcBigEndian READ itemCrcBigEndian CONSTANT)
    Q_PROPERTY(QString itemCrcAlgorithm READ itemCrcAlgorithm CONSTANT)
    Q_PROPERTY(QString itemCrcStartIndex READ itemCrcStartIndex CONSTANT)
    Q_PROPERTY(QString itemCrcEndIndex READ itemCrcEndIndex CONSTANT)
    Q_PROPERTY(QString itemText READ itemText CONSTANT)
public:
    struct DataKeys
    {
        const QString itemEnable{"Enable"};
        const QString itemDescription{"Description"};
        const QString itemTextFormat{"Format"};
        const QString itemEscapeCharacter{"Escape"};
        const QString itemPrefix{"Prefix"};
        const QString itemSuffix{"Suffix"};
        const QString itemInterval{"Interval"};
        const QString itemCrcEnable{"CrcEnable"};
        const QString itemCrcBigEndian{"BigEndian"};
        const QString itemCrcAlgorithm{"Algorithm"};
        const QString itemCrcStartIndex{"Start"};
        const QString itemCrcEndIndex{"End"};
        const QString itemText{"Data"};
    };

public:
    struct Data
    {
        bool itemEnable;
        QString itemDescription{"Demo"};
        int itemTextFormat;
        int itemEscapeCharacter;
        int itemPrefix;
        QString itemText;
        int itemSuffix;
        int itemInterval;

        bool itemCrcEnable;
        bool itemCrcBigEndian;
        int itemCrcAlgorithm;
        int itemCrcStartIndex;
        int itemCrcEndIndex;
    };

    struct EmitterItem
    {
        Data data;
        int elapsedTime{0};
    };

public:
    explicit xToolsEmitterTool(QObject *parent = Q_NULLPTR);
    virtual void inputBytes(const QByteArray &bytes) override;

public:
    virtual QString cookHeaderString(const QString &str) override;
    virtual QVariant itemContext(int index) final;

protected:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index,
                         const QVariant &value,
                         int role = Qt::EditRole) override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual QVariant headerData(int section,
                                Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const override;

    virtual void run() final;

private:
    QVector<EmitterItem> mItems;
    QMutex mItemsMutex;
    const int mDescriptionColumnIndex{0};
    const int mFormatColumnIndex{1};
    const int mItemTextColumnIndex{2};
    DataKeys mDataKeys;
    const int mTableColumnCount{13};
    QTimer *mEmittingTimer;
    const int mScanInterval{5};

private:
    void try2emit();
    QByteArray itemBytes(const xToolsEmitterTool::Data &item);
    QVariant columnDisplayRoleData(const EmitterItem &item, int column) const;

private:
    QString itemEnable();
    QString itemDescription();
    QString itemTextFormat();
    QString itemEscapeCharacter();
    QString itemInterval();
    QString itemPrefix();
    QString itemSuffix();
    QString itemCrcEnable();
    QString itemCrcBigEndian();
    QString itemCrcAlgorithm();
    QString itemCrcStartIndex();
    QString itemCrcEndIndex();
    QString itemText();
};
