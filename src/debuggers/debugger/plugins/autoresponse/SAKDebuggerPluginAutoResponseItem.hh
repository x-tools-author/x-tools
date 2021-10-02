/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERPLUGINAUTORESPONSEITEM_HH
#define SAKDEBUGGERPLUGINAUTORESPONSEITEM_HH

#include <QTimer>
#include <QWidget>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>

#include "SAKBaseListWidgetItemWidget.hh"

namespace Ui {
    class SAKDebuggerPluginAutoResponseItem;
}

class SAKDebuggerPluginAutoResponseItem : public SAKBaseListWidgetItemWidget
{
    Q_OBJECT
public:
    struct SAKStructItemContext {
        quint64 id;
        QString description;
        QString referenceData;
        QString responseData;
        bool enable;
        int referenceFormat;
        int responseFormat;
        int option;
        bool enableDelay;
        int delayTime;
    };

    enum SAKEnumAutomaticallyResponseOption {
        ReadDataIsEqualToReference,
        ReadDataContainsReferenceData,
        ReadDataDoesNotContainReferenceData
    };
    Q_ENUM(SAKEnumAutomaticallyResponseOption);


public:
    SAKDebuggerPluginAutoResponseItem(QWidget *parent = Q_NULLPTR);
    SAKDebuggerPluginAutoResponseItem(SAKStructItemContext ctx,
                                      QWidget *parent = Q_NULLPTR);
    ~SAKDebuggerPluginAutoResponseItem();


    SAKStructItemContext context();
    void onBytesRead(QByteArray bytes) final;


private:
    Ui::SAKDebuggerPluginAutoResponseItem *mUi;


private:
    void setupItem();
    bool needToResponse(QByteArray receiveData, QByteArray referenceData, int option);
    void blockUiComponentsSignals(bool block);


signals:
    void descriptionChanged(quint64 id, const QString &description);
    void referenceTextChanged(quint64 id, const QString &text);
    void responseTextChanged(quint64 id, const QString &text);
    void enableChanged(quint64 id, bool enable);
    void referenceFormatChanged(quint64 id, int format);
    void responseFromatChanged(quint64 id, int format);
    void optionChanged(quint64 id, int option);
    void enableDelayChanged(quint64 id, bool delay);
    void delayTimeChanged(quint64 id, int interval);
};

#endif
