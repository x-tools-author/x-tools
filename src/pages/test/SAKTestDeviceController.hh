/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTESTDEVICECONTROLLER_HH
#define SAKTESTDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKTestDeviceController;
}

class SAKDebugPage;
class SAKTestDeviceController:public SAKDebugPageController
{
    Q_OBJECT
public:
    struct ParametersContext {
        bool openFailed;
        bool readCyclic;
        int readInterval;
        bool writeCyclic;
        int writtingInterval;
        QString errorString;
    };

    SAKTestDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKTestDeviceController();

    QVariant parameters() final;
    void setUiEnable(bool opened) final;
private:
    ParametersContext mParameters;
    QMutex mParametersMutex;
private slots:
    void on_openFailedCheckBox_clicked();
    void on_errorStringLineEdit_textChanged(const QString &arg1);
    void on_readCyclicCheckBox_clicked();
    void on_readIntervalLineEdit_textChanged(const QString &arg1);
    void on_writeCyclicCheckBox_clicked();
    void on_writtenIntervalLineEdit_textChanged(const QString &arg1);
private:
    Ui::SAKTestDeviceController *mUi;
};

Q_DECLARE_METATYPE(SAKTestDeviceController::ParametersContext);
#endif
