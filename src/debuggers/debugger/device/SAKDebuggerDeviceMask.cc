/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QTimer>
#include "SAKDebuggerDeviceMask.hh"
#include "ui_SAKDebuggerDeviceMask.h"

SAKDebuggerDeviceMask::SAKDebuggerDeviceMask(QSettings *settings,
                                             QString settingGroup,
                                             QWidget *parent)
    :QDialog(parent)
    ,mUi(new Ui::SAKDebuggerDeviceMask)
{
    mUi->setupUi(this);


    if (settings) {
        mSettingsKeyCtx.rxMask = settingGroup + "/" + "rxMask";
        mSettingsKeyCtx.txMask = settingGroup + "/" + "txMask";

        int rxMask = settings->value(mSettingsKeyCtx.rxMask).toInt();
        mUi->rxMaskSpinBox->setValue(rxMask);
#if QT_VERSION >= QT_VERSION_CHECK(5,7,0)
    connect(mUi->rxMaskSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
#else
    connect(mUi->rxMaskSpinBox,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
#endif
        this, [=](){
            settings->setValue(mSettingsKeyCtx.rxMask, mUi->rxMaskSpinBox->value());
            emit parametersChanged();
        });

        int txMask = settings->value(mSettingsKeyCtx.txMask).toInt();
        mUi->txMaskSpinBox->setValue(txMask);
#if QT_VERSION >= QT_VERSION_CHECK(5,7,0)
    connect(mUi->txMaskSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
#else
    connect(mUi->txMaskSpinBox,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
#endif
         this, [=](){
            settings->setValue(mSettingsKeyCtx.txMask, mUi->txMaskSpinBox->value());
            emit parametersChanged();
        });
    }


    connect(mUi->rxMaskCheckBox, &QCheckBox::clicked,
            this, [=](){emit parametersChanged();});
    connect(mUi->txMaskCheckBox, &QCheckBox::clicked,
            this, [=](){emit parametersChanged();});
    setModal(true);
}

SAKDebuggerDeviceMask::~SAKDebuggerDeviceMask()
{
    delete mUi;
}

SAKDebuggerDeviceMask::SAKStructMaskContext SAKDebuggerDeviceMask::parametersContext()
{
    SAKStructMaskContext ctx;
    ctx.enableRx = mUi->rxMaskCheckBox->isChecked();
    ctx.enableTx = mUi->txMaskCheckBox->isChecked();
    ctx.rx = mUi->rxMaskSpinBox->value();
    ctx.tx = mUi->txMaskSpinBox->value();

    return ctx;
}
