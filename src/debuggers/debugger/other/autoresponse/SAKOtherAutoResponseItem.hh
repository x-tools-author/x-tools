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
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>

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
                             bool delay,
                             int interval,
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
    bool delay();
    quint32 interval();
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
    void commonInitializing();
    void initDelayWritingTimer();
    void delayToWritBytes();
    void blockUiSignals(bool block);
private:
    Ui::SAKOtherAutoResponseItem *mUi;
    QLineEdit *mDescriptionLineEdit;
    QLineEdit *mReferenceLineEdit;
    QLineEdit *mResponseLineEdit;
    QCheckBox *mEnableCheckBox;
    QComboBox *mOptionComboBox;
    QComboBox *mReferenceDataFromatComboBox;
    QComboBox *mResponseDataFormatComboBox;
    QCheckBox *mDelayResponseCheckBox;
    QLineEdit *mDelayResponseLineEdit;
private slots:
    void on_descriptionLineEdit_textChanged(const QString &text);
    void on_referenceLineEdit_textChanged(const QString &text);
    void on_responseLineEdit_textChanged(const QString &text);
    void on_enableCheckBox_clicked();
    void on_optionComboBox_currentTextChanged(const QString &text);
    void on_referenceDataFromatComboBox_currentTextChanged(const QString &text);
    void on_responseDataFormatComboBox_currentTextChanged(const QString &text);
    void on_delayResponseCheckBox_clicked();
    void on_delayResponseLineEdit_textChanged(const QString &text);
signals:
    void descriptionChanged(const QString &description);
    void referenceTextChanged(const QString &text);
    void responseTextChanged(const QString &text);
    void enableChanged(bool enable);
    void optionChanged(int option);
    void referenceFormatChanged(int format);
    void responseFromatChanged(int format);
    void delayChanged(bool delay);
    void intervalChanged(int interval);
};

#endif
