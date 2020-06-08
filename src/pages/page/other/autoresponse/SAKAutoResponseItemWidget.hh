/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com
 */
#ifndef SAKAUTORESPONSEITEMWIDGET_HH
#define SAKAUTORESPONSEITEMWIDGET_HH

#include "SAKDebugPage.hh"

#include <QRegExp>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QRegExpValidator>

namespace Ui {
    class SAKAutoResponseItemWidget;
}

class SAKDebugPage;
/// @brief 自动回复条目
class SAKAutoResponseItemWidget:public QWidget
{
    Q_OBJECT
public:
    SAKAutoResponseItemWidget(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKAutoResponseItemWidget();

    /**
     * @brief setAllAutoResponseDisable 禁止所有自动回复
     * @param disAbel 该值为true时，禁止所有回复，否则更具回复示例的使能判断是否自动回复
     */
    void setAllAutoResponseDisable(bool disable);
private:
    bool forbiddenAllAutoResponse;
    SAKDebugPage *debugPage;
private:
    /// @brief 设置输入框文本格式(SAKDataStruct::SAKEnumTextInputFormat)
    void setLineEditFormat(QLineEdit *lineEdit, int format);
    /// @brief 读取数据后执行该函数，参数为已读取的数据
    void bytesRead(QByteArray bytes);
    QByteArray string2array(QString str, int format);
    /// @brief 判断是否需要回复
    bool response(QByteArray receiveData, QByteArray referenceData, int option);
private:
    Ui::SAKAutoResponseItemWidget *ui;
    QLineEdit   *remarkLineEdit;
    QLineEdit   *referenceLineEdit;
    QLineEdit   *responseLineEdit;
    QCheckBox   *enableCheckBox;
    QComboBox   *optionComboBox;
    QComboBox   *referenceDataFromatComboBox;
    QComboBox   *responseDataFormatComboBox;
private slots:
    void on_referenceDataFromatComboBox_currentTextChanged();
    void on_responseDataFormatComboBox_currentTextChanged();
};

#endif
