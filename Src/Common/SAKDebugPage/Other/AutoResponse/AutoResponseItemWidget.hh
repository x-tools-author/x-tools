/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#ifndef AUTORESPONSEITEM_HH
#define AUTORESPONSEITEM_HH

#include "SAKDebugPage.hh"

#include <QRegExp>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QRegExpValidator>

class SAKDebugPage;

namespace Ui {
    class AutoResponseItemWidget;
}

class AutoResponseItemWidget:public QWidget
{
    Q_OBJECT
public:
    AutoResponseItemWidget(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~AutoResponseItemWidget();

    /**
     * @brief setAllAutoResponseDisable 禁止所有自动回复
     * @param disAbel 该值为true时，禁止所有回复，否则更具回复示例的使能判断是否自动回复
     */
    void setAllAutoResponseDisable(bool disAbel);
private:
    Ui::AutoResponseItemWidget *ui;
    QLineEdit   *remarkLineEdit;
    QLineEdit   *referenceLineEdit;
    QLineEdit   *responseLineEdit;
    QCheckBox   *enableCheckBox;
    QComboBox   *optionComboBox;
    QComboBox   *referenceDataFromatComboBox;
    QComboBox   *responseDataFormatComboBox;

    /// 禁止所有自动回复标志
    bool forbiddenAllAutoResponse;
    SAKDebugPage *_debugPage;
private:
    /// 设置输入框文本格式(详情EDBaseApi::EDTextFormat)
    void setLineEditFormat(QLineEdit *lineEdit, int format);
    /*
     * 设置输入框文本格式
     */
    void setLineEditFormatBin(QLineEdit *lineEdit);
    void setLineEditFormatOct(QLineEdit *lineEdit);
    void setLineEditFormatDec(QLineEdit *lineEdit);
    void setLineEditFormatHex(QLineEdit *lineEdit);
    void setLineEditFormatAscii(QLineEdit *lineEdit);
    void setLineEditFormatUtf8(QLineEdit *lineEdit);
    void setLineEditFormatLocal(QLineEdit *lineEdit);

    void handleReceiceData(QByteArray data, SAKDebugPage::OutputParameters parameters);
private slots:
    void on_referenceDataFromatComboBox_currentTextChanged();
    void on_responseDataFormatComboBox_currentTextChanged();
signals:
    void requestWrite(QByteArray data);
};

#endif
