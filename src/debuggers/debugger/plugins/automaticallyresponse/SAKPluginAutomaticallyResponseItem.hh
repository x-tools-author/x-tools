/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKOTHERAUTOMATICALLYRESPONSEITEM_HH
#define SAKOTHERAUTOMATICALLYRESPONSEITEM_HH

#include <QTimer>
#include <QWidget>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>

namespace Ui {
    class SAKPluginAutomaticallyResponseItem;
}

class SAKDebugger;
/// @brief Auto response item
class SAKPluginAutomaticallyResponseItem : public QWidget
{
    Q_OBJECT
public:
    struct SAKStructAutomaticallyResponseItemContext {
        quint64 id;
        QString name;
        QString referenceData;
        QString responseData;
        bool enable;
        int referenceFormat;
        int responseFormat;
        int option;
        bool delay;
        int interval;
    };
    typedef struct SAKStructAutomaticallyResponseItemContext ITEM_CTX;
public:
    SAKPluginAutomaticallyResponseItem(QWidget *parent = Q_NULLPTR);
    SAKPluginAutomaticallyResponseItem(quint64 mID,
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
    ~SAKPluginAutomaticallyResponseItem();


    SAKStructAutomaticallyResponseItemContext context();
    void onBytesRead(const QByteArray &bytes);
private:
    bool mForbiddenAllAutoResponse;
    SAKDebugger *mDebugPage;
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
    Ui::SAKPluginAutomaticallyResponseItem *mUi;


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

    void responseBytes(const QByteArray &bytes);
};

#endif
