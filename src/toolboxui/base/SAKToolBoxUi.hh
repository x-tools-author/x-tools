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
    void initUiDevice();
    void initUiInput();
    void initUiOutput();

    void initSettings();
    void initSettingsDevice();
    void initSettingsInput();
    void initSettingsOutput();

    void initSignals();
    void initSignalsDevice();
    void initSignalsInput();
    void initSignalsOutput();

private:
    SAKToolBox *mToolBox;

private:
    Ui::SAKToolBoxUi *ui;

    void onPushButtonInputSetttingsClicked();
};

#endif // SAKTOOLBOXUI_HH
