/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
    ~SAKReadWriteSettingsWidget();
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
