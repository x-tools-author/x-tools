/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOTHERAUTORESPONSEITEM_HH
#define SAKOTHERAUTORESPONSEITEM_HH

#include <QTimer>
#include <QRegExp>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QRegExpValidator>

namespace Ui {
    class SAKOtherAutoResponseItem;
}

class SAKDebugPage;
/// @brief 自动回复条目
class SAKOtherAutoResponseItem:public QWidget
{
    Q_OBJECT
public:
    SAKOtherAutoResponseItem(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    SAKOtherAutoResponseItem(SAKDebugPage *debugPage,
                              quint64 id,
                              QString name,
                              QString referenceData,
                              QString responseData,
                              bool enabled,
                              quint32 referenceFormat,
                              quint32 responseFormat,
                              quint32 option,
                              QWidget *parent = Q_NULLPTR);
    ~SAKOtherAutoResponseItem();

    /**
     * @brief setAllAutoResponseDisable 禁止所有自动回复
     * @param disAbel 该值为true时，禁止所有回复，否则更具回复示例的使能判断是否自动回复
     */
    void setAllAutoResponseDisable(bool disable);

    quint64 parameterID();
    QString parameterName();
    QString parameterRefernceData();
    QString parameterResponseData();
    bool parameterEnable();
    quint32 parameterReferenceFormat();
    quint32 parameterResponseFormat();
    quint32 parameterOption();
private:
    bool forbiddenAllAutoResponse;
    SAKDebugPage *debugPage;
    quint64 id;

    struct DelayWritingInfo{
        quint64 expectedTimestamp;
        QByteArray data;
    };
    QTimer delayToWritingTimer;
    QList<DelayWritingInfo*> delayWritingInfoList;
private:
    void setLineEditFormat(QLineEdit *lineEdit, int format);
    void bytesRead(QByteArray bytes);
    QByteArray string2array(QString str, int format);
    bool response(QByteArray receiveData, QByteArray referenceData, int option);
    void initUi();
    void initDelayWritingTimer();
    void delayToWritBytes();
private:
    Ui::SAKOtherAutoResponseItem *ui;
    QLineEdit *remarkLineEdit;
    QLineEdit *referenceLineEdit;
    QLineEdit *responseLineEdit;
    QCheckBox *enableCheckBox;
    QComboBox *optionComboBox;
    QComboBox *referenceDataFromatComboBox;
    QComboBox *responseDataFormatComboBox;
    QPushButton *updatePushButton;
    QCheckBox *delayResponseCheckBox;
    QLineEdit *delayResponseLineEdit;
private slots:
    void on_referenceDataFromatComboBox_currentTextChanged();
    void on_responseDataFormatComboBox_currentTextChanged();
    void on_updatePushButton_clicked();
};

#endif
