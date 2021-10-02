/******************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKDEBUGGERINPUTDATAPRESETITEM_HH
#define SAKDEBUGGERINPUTDATAPRESETITEM_HH

#include <QTimer>
#include <QWidget>
#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>

#include "SAKBaseListWidgetItemWidget.hh"

namespace Ui {
    class SAKDebuggerInputDataPresetItem;
}

// Preset data parameters editting widget
class SAKDebuggerInputDataPresetItem : public SAKBaseListWidgetItemWidget
{
    Q_OBJECT
public:
    struct SAKStructItemContext {
        quint64 id;
        QString description;
        int format;
        QString data;
    };
public:
    SAKDebuggerInputDataPresetItem(QWidget *parent = Q_NULLPTR);
    SAKDebuggerInputDataPresetItem(SAKStructItemContext context,
                           QWidget *parent = Q_NULLPTR);
    ~SAKDebuggerInputDataPresetItem();
    SAKStructItemContext context();


private:
    Ui::SAKDebuggerInputDataPresetItem *mUi;


private:
    void initializingItem();
    void blockUiComponentsSignals(bool block);


signals:
    void descriptionChanged(quint64 id, const QString &text);
    void formatChanged(quint64 id, quint32 format);
    void dataChanged(quint64 id, const QString &text);
};

#endif
