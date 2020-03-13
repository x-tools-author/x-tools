/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
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

class SAKDebugPage;

namespace Ui {
    class SAKAutoResponseItemWidget;
}

class SAKAutoResponseItemWidget:public QWidget
{
    Q_OBJECT
public:
    SAKAutoResponseItemWidget(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKAutoResponseItemWidget();

    enum AutoResponseOption{
        Equivalence,    // 相等
        Contain,        // 包含
        Notcontain,     // 不包含
    };

    /**
     * @brief setAllAutoResponseDisable 禁止所有自动回复
     * @param disAbel 该值为true时，禁止所有回复，否则更具回复示例的使能判断是否自动回复
     */
    void setAllAutoResponseDisable(bool disAbel);
private:
    Ui::SAKAutoResponseItemWidget *ui;
    QLineEdit   *remarkLineEdit;
    QLineEdit   *referenceLineEdit;
    QLineEdit   *responseLineEdit;
    QCheckBox   *enableCheckBox;
    QComboBox   *optionComboBox;
    QComboBox   *referenceDataFromatComboBox;
    QComboBox   *responseDataFormatComboBox;

    /// 禁止所有自动回复标志
    bool forbiddenAllAutoResponse;
    SAKDebugPage *debugPage;
private:
    /// 设置输入框文本格式(详情SAKGlobal::EDTextFormat)
    void setLineEditFormat(QLineEdit *lineEdit, int format);

    void dataRead(QByteArray data);
    QByteArray string2array(QString str, int format);
    bool response(QByteArray receiveData, QByteArray referenceData, int option);    
private slots:
    void on_referenceDataFromatComboBox_currentTextChanged();
    void on_responseDataFormatComboBox_currentTextChanged();
signals:
    void requestWrite(QByteArray data);
};

#endif
