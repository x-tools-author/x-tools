/****************************************************************************************
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERINPUTCRCSETTINGDIALOG_HH
#define SAKDEBUGGERINPUTCRCSETTINGDIALOG_HH

#include <QMutex>
#include <QDialog>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QSettings>

namespace Ui {
    class SAKDebuggerInputCrcSettings;
}

/// @brief crc settings dialog
class SAKDebuggerInputCrcSettings : public QDialog
{
    Q_OBJECT
public:
    SAKDebuggerInputCrcSettings(QString settingsGroup,
                              QSettings *settings,
                              QWidget *parent = Q_NULLPTR);
    ~SAKDebuggerInputCrcSettings();

    struct SAKStructCrcParametersContext {
        bool append;
        bool bigEndian;
        int parameterMoldel;
        int startByte;  // The first byte is one
        int endByte;    // The last byte is one
    };

    /**
     * @brief parametersContext: get the parameters context
     * @return parameters context
     */
    SAKStructCrcParametersContext parametersContext();
private:
    SAKStructCrcParametersContext mParametersContext;
    QMutex mParametersContextMutex;
    QSettings *mSettings;

    struct {
        QString append;
        QString bigEndian;
        QString parameterMoldel;
        QString startByte;
        QString endByte;
    } mSettingsKeyContext;
private:
    Ui::SAKDebuggerInputCrcSettings *mUi;
signals:
    void crcParametersChanged();
};

#endif
