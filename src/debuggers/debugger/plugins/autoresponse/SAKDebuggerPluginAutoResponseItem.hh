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

namespace Ui {
    class SAKDebuggerPluginAutoResponseItem;
}

/// @brief Auto response item
class SAKDebuggerPluginAutoResponseItem : public QWidget
{
    Q_OBJECT
public:
    struct SAKStructItemContext {
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
    void onBytesRead(const QByteArray &bytes);


private:
    quint64 mID;
    Ui::SAKDebuggerPluginAutoResponseItem *mUi;


private:
    void setupItem();
    bool response(QByteArray receiveData,
                  QByteArray referenceData,
                  int option);


signals:
    void descriptionChanged(quint64 id, const QString &description);
    void referenceTextChanged(quint64 id, const QString &text);
    void responseTextChanged(quint64 id, const QString &text);
    void enableChanged(quint64 id, bool enable);
    void optionChanged(quint64 id, int option);
    void referenceFormatChanged(quint64 id, int format);
    void responseFromatChanged(quint64 id, int format);
    void delayChanged(quint64 id, bool delay);
    void intervalChanged(quint64 id, int interval);

    void responseBytes(const QByteArray &bytes);
};

#endif
