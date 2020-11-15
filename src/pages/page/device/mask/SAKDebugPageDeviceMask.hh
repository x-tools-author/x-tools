/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGEDEVICEMASK_HH
#define SAKDEBUGPAGEDEVICEMASK_HH

#include <QMutex>
#include <QWidget>
#include <QSettings>

namespace  Ui{
    class SAKDebugPageDeviceMask;
}

class SAKDebugPage;
class SAKDebugPageDeviceMask : public QWidget
{
    Q_OBJECT
public:
    explicit SAKDebugPageDeviceMask(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKDebugPageDeviceMask();

    struct ParametersContext{
        quint8 rxMask;
        quint8 txMask;
        bool enableMask;
    };
    ParametersContext parametersContext();
private:
    SAKDebugPage *mDebugPage;
    ParametersContext mParametersContext;
    QMutex mParametersContextMutex;
    QSettings *mSettings;
    QString mSettingsKeyRxMask;
    QString mSettingsKeyTxMask;
private:
    Ui::SAKDebugPageDeviceMask *ui;
private slots:
    void on_rxMaskSpinBox_valueChanged(int arg1);
    void on_txMaskSpinBox_valueChanged(int arg1);
    void on_enableMaskCheckBox_clicked();
};

#endif // SAKDEBUGPAGEDEVICEMASK_HH
