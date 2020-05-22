/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKDEBUGGERINPUTTEXTDATAPRESETITEM_HH
#define SAKDEBUGGERINPUTTEXTDATAPRESETITEM_HH

#include <QObject>

/// @brief 数据预设条目
class SAKDebuggerInputTextDataPresetItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString itemID READ itemID CONSTANT)
    Q_PROPERTY(QString itemName READ itemName WRITE setItemName NOTIFY itemNameChanged)
    Q_PROPERTY(QString itemFormat READ itemFormat WRITE setItemFormat NOTIFY itemFormatChanged)
    Q_PROPERTY(QString itemText READ itemText WRITE setItemText NOTIFY itemTextChanged)
    Q_PROPERTY(QString itemGroup READ itemGroup WRITE setItemGroup NOTIFY itemGroupChanged)
    Q_PROPERTY(QStringList formats READ formats CONSTANT)
public:
    SAKDebuggerInputTextDataPresetItem(QObject *parent = Q_NULLPTR);
    SAKDebuggerInputTextDataPresetItem(QString id, QString name, QString format, QString text, QString group, QObject *parent = Q_NULLPTR);
    ~SAKDebuggerInputTextDataPresetItem();
private:
    /// @brief 预设数据唯一标识
    QString _itemID;
    QString itemID();

    /// @brief 预设数据名称
    QString _itemName;
    QString itemName();
    void setItemName(QString name);

    /// @brief 预设数据格式
    QString _itemFormat;
    QString itemFormat();
    void setItemFormat(QString format);

    /// @brief 预设文本
    QString _itemText;
    QString itemText();
    void setItemText(QString text);

    /// @brief 分组名称
    QString _itemGroup;
    QString itemGroup();
    void setItemGroup(QString group);

    /// @brief 文本格式列表
    QStringList formats();
signals:
    void itemNameChanged();
    void itemFormatChanged();
    void itemTextChanged();
    void itemGroupChanged();
};

#endif
