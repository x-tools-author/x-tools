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
/// @brief Auto response item
class SAKOtherAutoResponseItem:public QWidget
{
    Q_OBJECT
public:
    SAKOtherAutoResponseItem(SAKDebugPage *mDebugPage, QWidget *parent = Q_NULLPTR);
    SAKOtherAutoResponseItem(SAKDebugPage *mDebugPage,
                             quint64 mID,
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
     * @brief setAllAutoResponseDisable: Set enable flag
     * @param disable: true-disable all auto response item
     */
    void setAllAutoResponseDisable(bool disable);

    // Get parameters
    quint64 itemID();
    QString itemDescription();
    QString itemRefernceText();
    QString itemResponseText();
    bool itemEnable();
    quint32 itemReferenceFormat();
    quint32 itemResponseFormat();
    quint32 itemOption();
private:
    bool mForbiddenAllAutoResponse;
    SAKDebugPage *mDebugPage;
    quint64 mID;

    // delay response
    struct DelayWritingInfo{
        quint64 expectedTimestamp;
        QByteArray data;
    };
    QTimer mTimestampChecker;
    QList<DelayWritingInfo*> mWaitForWrittenInfoList;
private:
    void setLineEditFormat(QLineEdit *lineEdit, int format);
    void bytesRead(QByteArray bytes);
    QByteArray string2array(QString str, int format);
    bool response(QByteArray receiveData, QByteArray referenceData, int option);
    void initUi();
    void initDelayWritingTimer();
    void delayToWritBytes();
private:
    Ui::SAKOtherAutoResponseItem *mUi;
    QLineEdit *mDescriptionLineEdit;
    QLineEdit *mReferenceLineEdit;
    QLineEdit *mResponseLineEdit;
    QCheckBox *enableCheckBox;
    QComboBox *mOptionComboBox;
    QComboBox *mReferenceDataFromatComboBox;
    QComboBox *mResponseDataFormatComboBox;
    QCheckBox *mDelayResponseCheckBox;
    QLineEdit *mDelayResponseLineEdit;
private slots:
    void on_referenceDataFromatComboBox_currentTextChanged();
    void on_responseDataFormatComboBox_currentTextChanged();
};

#endif
