/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKREADWRITESETTINGSWIDGET_HH
#define SAKREADWRITESETTINGSWIDGET_HH

#include <QDialog>
#include <QComboBox>


namespace Ui {
class SAKReadWriteSettingsWidget;
};

class SAKDebugPage;
class SAKReadWriteSettingsWidget:public QDialog
{
    Q_OBJECT
public:
    SAKReadWriteSettingsWidget(SAKDebugPage *debugPage, QWidget* parent = Q_NULLPTR);
private:
    Ui::SAKReadWriteSettingsWidget *ui = Q_NULLPTR;

    QComboBox *readParametersComboBox;
    QComboBox *writeParametersComboBox;
    QComboBox *intervalParameterComboBox;
private slots:
    void on_readParametersComboBox_currentTextChanged(const QString &text);
    void on_writeParametersComboBox_currentTextChanged(const QString &text);
    void on_intervalParameterComboBox_currentTextChanged(const QString &text);

private:
    SAKDebugPage *debugPage;
    int waitForReadyReadTime;
    int waitForBytesWrittenTime;
    int runIntervalTime;

    void setReadWriteParameters();
};

#endif
