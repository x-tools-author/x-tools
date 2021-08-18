/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERPLUGINTIMEDSENDINGITEM_HH
#define SAKDEBUGGERPLUGINTIMEDSENDINGITEM_HH

#include <QTimer>
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>

#include "SAKBaseListWidgetItemWidget.hh"

namespace Ui {
    class SAKDebuggerPluginTimedSendingItem;
}

class SAKDebuggerPluginTimedSendingItem : public SAKBaseListWidgetItemWidget
{
    Q_OBJECT
public:
    struct SAKStructItemContext {
        quint64 id;
        bool enable;
        QString description;
        int interval;
        int format;
        int suffix;
        QString data;
    };


public:
    SAKDebuggerPluginTimedSendingItem(QWidget *parent = Q_NULLPTR);
    SAKDebuggerPluginTimedSendingItem(SAKStructItemContext ctx,
                                      QWidget *parent = Q_NULLPTR);
    ~SAKDebuggerPluginTimedSendingItem();
    const SAKStructItemContext context();


private:
    QTimer mWriteTimer;
    SAKStructItemContext mContext;


private:
    void write();
    void commonInitializing();
    void blockUiCommpentsSignals(bool block);


private:
    Ui::SAKDebuggerPluginTimedSendingItem *mUi;


signals:
    void enableChanged(quint64 id, bool enable);
    void descriptionChanged(quint64 id, QString description);
    void intervalChanged(quint64 id, int interval);
    void formatChanged(quint64 id, int format);
    void suffixChanged(quint64 id, int suffix);
    void dataChanged(quint64 id, QString text);
};

#endif
