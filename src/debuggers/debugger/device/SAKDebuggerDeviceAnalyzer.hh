/****************************************************************************************
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERDEVICEANALYZER_HH
#define SAKDEBUGGERDEVICEANALYZER_HH

#include <QDialog>
#include <QSettings>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
    class SAKDebuggerDeviceAnalyzer;
}

/// @brief Parameters editing widget
class SAKDebuggerDeviceAnalyzer:public QDialog
{
    Q_OBJECT
public:
    SAKDebuggerDeviceAnalyzer(QSettings *settings,
                              const QString &settingsGroup,
                              QWidget *parent = Q_NULLPTR);
    ~SAKDebuggerDeviceAnalyzer();

    struct SAKStructAnalyzerContext {
        bool enable;
        bool fixedLength;
        int length;
        QByteArray startFlags;
        QByteArray endFlags;
    };

    SAKStructAnalyzerContext parametersContext();
private:
    struct SAKStructSettingsKeyContext {
        QString enable;

        QString fixedLength;
        QString frameLength ;

        QString startFlags;
        QString endFlags;
    }mSettingsKeyContext;
private:
    Ui::SAKDebuggerDeviceAnalyzer *mUi;
signals:
    void parametersChanged();
    void clearTemp();
};

#endif
