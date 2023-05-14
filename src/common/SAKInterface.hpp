/******************************************************************************
 * Copyright 2023 wuuhaii(wuuhaii@outlook.com). All rights reserved.
 *****************************************************************************/
#ifndef EDINTERFACE_HPP
#define EDINTERFACE_HPP

#include <QObject>
#include <QVariant>

class EDInterface : public QObject
{
    Q_OBJECT
public:
    explicit EDInterface(QObject *parent = nullptr);

    Q_INVOKABLE static QString array2String(const QByteArray &array,
                                            int format);
    Q_INVOKABLE static QByteArray string2array(const QString &input,
                                               int format);
    Q_INVOKABLE static void setMaximumBlockCount(QVariant quickTextDocument,
                                                 int maximum);
    Q_INVOKABLE static QString dateTimeString(const QString &format);
    Q_INVOKABLE static QByteArray arrayAppendArray(const QByteArray &a1,
                                                   const QByteArray &a2);
    Q_INVOKABLE static void tableModel2jsonFile(QVariant tableModel,
                                                const QString &fileName);
    Q_INVOKABLE static void jsonFile2tableModel(QVariant tableModel,
                                                const QString &fileName);
    Q_INVOKABLE static void jsonArray2TableModel(QVariant tableModel,
                                                 const QJsonArray &array);
    Q_INVOKABLE static QString cookedFileName(const QString &fileName);
    Q_INVOKABLE static void clearTableModel(QVariant tableModel);
    Q_INVOKABLE static void removeTableModelRow(QVariant tableModel, int row);

    Q_INVOKABLE static QString string2hexString(const QString &str);
    Q_INVOKABLE static QString hexString2String(const QString &str);
    Q_INVOKABLE static void setAppFont(const QString &fontFamily);
    Q_INVOKABLE static QString buildDateTime(const QString &format);
    Q_INVOKABLE static QString dateFormat();
    Q_INVOKABLE static QString timeFormat();
    Q_INVOKABLE static void setTextToClipboard(const QString &text);
};

#endif // EDINTERFACE_H
