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
#ifndef SAKDEBUGGERINPUTTEXTDATAPRESETCONTROLLER_HH
#define SAKDEBUGGERINPUTTEXTDATAPRESETCONTROLLER_HH

#include <QObject>
#include <QVariantList>

class SAKDebuggerTextInput;
class SAKDebuggerInputTextDataPresetItem;
/// @brief 为预设数据功能提供一些接口
class SAKDebuggerInputTextDataPresetController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList dataPresetItems READ dataPresetItems NOTIFY dataPresetItemsChanged)
    Q_PROPERTY(QStringList groups READ groups NOTIFY groupsChanged)
private:
    SAKDebuggerInputTextDataPresetController(QObject *parent = Q_NULLPTR);
    ~SAKDebuggerInputTextDataPresetController();
public:
    friend class SAKDebuggerTextInput;

    /**
     * @brief createDataPresetItem 穿件一个数据条目
     */
    Q_INVOKABLE void createDataPresetItem();

    /**
     * @brief addGroup 添加分组
     * @param name 分组名称
     */
    Q_INVOKABLE void addGroup(QString name);

    /**
     * @brief removeGroup 移除分组
     * @param name 分组名称
     */
    Q_INVOKABLE void removeGroup(QString name);
private:
    QVariantList _dataPresetItems;
    QVariantList dataPresetItems();

    /// @brief 预设数据分组
    QStringList _groups;
    QStringList groups();
signals:
    void dataPresetItemsChanged();
    void groupsChanged();
};

#endif
