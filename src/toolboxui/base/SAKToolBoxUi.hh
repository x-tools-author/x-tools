/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTOOLBOXUI_HH
#define SAKTOOLBOXUI_HH

#include <QWidget>
#include <QLoggingCategory>

#include "SAKToolBox.hh"

namespace Ui {
class SAKToolBoxUi;
}

class SAKToolBoxUiParameters;
class SAKToolBoxUi : public QWidget
{
    Q_OBJECT
public:
    explicit SAKToolBoxUi(QWidget *parent = nullptr);
    ~SAKToolBoxUi();

protected:
    virtual QWidget *controller() = 0;
    void init();

private:
    SAKToolBoxUiParameters *mToolBoxUiParameters{nullptr};

private:
    void initUi();
    void initUiCommunication();
    void initUiInput();
    void initUiOutput();

    void initSettings();
    void initSettingsCommunication();
    void initSettingsInput();
    void initSettingsOutput();

    void initSignals();
    void initSignalsCommunication();
    void initSignalsInput();
    void initSignalsOutput();

private:
    SAKToolBox *mToolBox;
    const QLoggingCategory mLoggingCategory{"SAK.SAKToolBoxUi"};

private:
    Ui::SAKToolBoxUi *ui;

    void onPushButtonCommunicationSettingsClicked();
    void onPushButtonCommunicationOpenClicked();

    void onPushButtonInputSettingsClicked();

    void onPushButtonOutputSettingsClicked();
};

#endif // SAKTOOLBOXUI_HH
